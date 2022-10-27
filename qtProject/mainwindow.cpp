#include "mainwindow.h"

#include <unistd.h>

#include <QFileDialog>
#include <QPixmap>
#include <QSettings>
#include <QTimer>
#include <iostream>

#include "./ui_mainwindow.h"

extern "C" {
#include "../c_affins.h"
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->openGLWidget->model_data.count_of_v = 0;
  ui->recording_label->setVisible(false);
  //  readSettings();
}

MainWindow::~MainWindow() {
  // writeSettings();
  delete ui;
}

/*
void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if (!(event->pos().x() > 0 && event->pos().x() < 500) ||
            !(event->pos().y() > 0 && event->pos().y() < 500))
            return;

     if (ui->rotate_checkbox->isChecked()) { // Rotate on
         if (pos_rotate_mouse_x != 0) {
            float value_rotate_x = (pos_rotate_mouse_x  -
event->pos().x())/200.0;
           // qDebug() << value_rotate_x;
            rotation_by_oy(&(ui->openGLWidget->model_data.matrix_3d),
value_rotate_x*(-1),
                           &(ui->openGLWidget->x_offset),
                           &(ui->openGLWidget->z_offset));

            ui->openGLWidget->update();
             pos_rotate_mouse_x = event->pos().x();
         } else {
             pos_rotate_mouse_x = event->pos().x();
         }

         if (pos_rotate_mouse_y != 0) {
            float value_rotate_y = (pos_rotate_mouse_y  -
event->pos().y())/200.0;
           // qDebug() << value_rotate_x;
            rotation_by_ox(&(ui->openGLWidget->model_data.matrix_3d),
value_rotate_y,
                           &(ui->openGLWidget->y_offset),
                           &(ui->openGLWidget->z_offset));

            ui->openGLWidget->update();
             pos_rotate_mouse_y = event->pos().y();
         } else {
             pos_rotate_mouse_y = event->pos().y();
         }

     } else { // Move on
         if (pos_mouse_x != 0) {
            float value_rotate_x = (pos_mouse_x - event->pos().x())/200.0;
           // qDebug() << value_rotate_x;
            move_x(&(ui->openGLWidget->model_data.matrix_3d),
value_rotate_x*(-1)); ui->openGLWidget->update(); pos_mouse_x =
event->pos().x(); } else { pos_mouse_x = event->pos().x();
         }
         if (pos_mouse_y != 0) {
            float value_rotate_y = ( pos_mouse_y - event->pos().y())/200.0;
           // qDebug() << value_rotate_x;
            move_y(&(ui->openGLWidget->model_data.matrix_3d), value_rotate_y);
            ui->openGLWidget->update();
             pos_mouse_y = event->pos().y();
         } else {
             pos_mouse_y = event->pos().y();
         }
     }
}
*/

void MainWindow::on_actionImport_Model_triggered() {
  clear_settings();
  QString str = QFileDialog::getOpenFileName(0, "Открыть", "", "*.obj");
  path_model = str.toStdString();
  if (str.length() > 4) import_model(path_model);
}

void MainWindow::import_model(std::string path_model) {
  // if (ui->openGLWidget->model_data.count_of_f != 0)
  //    clean_data_struct(&(ui->openGLWidget->model_data));

  ui->openGLWidget->load_model(path_model);

  // std::cout << "143: " << ui->openGLWidget->project_name << std::endl;
  // ui->model_name->setText(QChar(ui->openGLWidget->project_name[0]));
  ui->polygons_value_label->setText(
      QString::number(ui->openGLWidget->model_data.count_of_f));
  ui->vertex_value_label->setText(
      QString::number(ui->openGLWidget->model_data.count_of_v));

  /// QString file_name = QFileDialog::getOpenFileName(this, "Выберите файл",
  /// "", tr("obj Files (*.obj)"));
  char f_name_char[256] = {0};
  strcpy(f_name_char, path_model.c_str());

  int i = strlen(f_name_char) - 1;
  for (; i >= 0; i--) {
    if (f_name_char[i] == '/') break;
  }
  i++;
  int n = 0;
  for (; f_name_char[i] != '\0'; i++) {
    f_name_char[n++] = f_name_char[i];
  }
  f_name_char[n - 4] = '\0';
  this->setWindowTitle(f_name_char);
  ui->model_name->setText(f_name_char);
}

void MainWindow::on_type_egde_comboBox_activated(int index) {
  // std::cout << index << std::endl;
  ui->openGLWidget->strip = index;
  ;
  ui->openGLWidget->update();
}

void MainWindow::on_type_vertex_comboBox_activated(int index) {
  ui->openGLWidget->point_setup = index;
  ui->openGLWidget->update();
}

void MainWindow::on_projections_combobox_currentIndexChanged(int index) {
  ui->openGLWidget->projection = index;
  ui->openGLWidget->update();
}

void MainWindow::on_actionSave_settings_triggered() {
  QString str =
      QFileDialog::getSaveFileName(ui->openGLWidget, NULL, NULL, "INI (*.ini)");
  std::cout << "172: " << str.toStdString() << std::endl;
  if (str.length() > 4) writeSettings(str);
}

void MainWindow::on_actionLoad_settings_triggered() {
  QString str =
      QFileDialog::getOpenFileName(ui->openGLWidget, NULL, NULL, "INI (*.ini)");
  std::cout << "172: " << str.toStdString() << std::endl;
  if (str.length() > 4) readSettings(str);
}

void MainWindow::closeEvent(QCloseEvent *event) {
  // writeSettings();
  QMainWindow::closeEvent(event);
}

void MainWindow::writeSettings(QString path) {
  // QString path = QDir::homePath() + "/qsettingsexample.ini";
  QSettings settings(path, QSettings::IniFormat);

  // PATH TO FILE
  settings.setValue("model_path", QString::fromStdString(path_model));

  // Color settings INT
  settings.beginGroup("color");
  settings.setValue("color_edge", ui->edge_color_slider->value());
  settings.setValue("color_screen", ui->back_color_slider->value());
  settings.setValue("color_vertex", ui->vertex_color_slider->value());
  settings.endGroup();

  // Rotate settings INT
  settings.beginGroup("rotate");
  settings.setValue("rotate_x", ui->rotate_x_spinBox->value());
  settings.setValue("rotate_y", ui->rotate_y_spinBox->value());
  settings.setValue("rotate_z", ui->rotate_z_spinBox->value());
  settings.endGroup();

  // Move settings DOUBLE
  settings.beginGroup("move");
  settings.setValue("move_x", ui->move_x_spinBox->value());
  settings.setValue("move_y", ui->move_y_spinBox->value());
  settings.setValue("move_z", ui->move_z_spinBox->value());
  settings.endGroup();

  // OTHER
  settings.beginGroup("other");
  settings.setValue("zoom", ui->zoom_spinBox->value());  // DOUBLE
  settings.setValue("type_edge",
                    ui->type_egde_comboBox->currentIndex());  // INT
  settings.setValue("type_projection",
                    ui->projections_combobox->currentIndex());  // INT
  settings.setValue("type_vertex",
                    ui->type_vertex_comboBox->currentIndex());            // INT
  settings.setValue("width_lines", ui->widht_lines->value());             // INT
  settings.setValue("width_vertex", ui->widht_vertexes_slider->value());  // INT
  settings.endGroup();
}

void MainWindow::readSettings(QString path) {
  clear_settings();  // Очищает ли структуру. Если да, надо условие.
  // QString path = QDir::homePath() + "/qsettingsexample.ini";
  QSettings settings(path, QSettings::IniFormat);

  import_model(settings.value("model_path").toString().toStdString());

  ui->edge_color_slider->setValue(settings.value("color/color_edge").toInt());
  // std::cout << "263: " << settings.value("color/color_edge").toInt() <<
  // std::endl;
  ui->back_color_slider->setValue(settings.value("color/color_screen").toInt());
  ui->vertex_color_slider->setValue(
      settings.value("color/color_vertex").toInt());

  ui->rotate_x_spinBox->setValue(settings.value("rotate/rotate_x").toInt());
  ui->rotate_y_spinBox->setValue(settings.value("rotate/rotate_y").toInt());
  ui->rotate_z_spinBox->setValue(settings.value("rotate/rotate_z").toInt());

  ui->move_x_spinBox->setValue(settings.value("move/move_x").toFloat());
  ui->move_y_spinBox->setValue(settings.value("move/move_y").toFloat());
  ui->move_z_spinBox->setValue(settings.value("move/move_z").toFloat());

  ui->zoom_spinBox->setValue(settings.value("other/zoom").toFloat());

  ui->type_egde_comboBox->setCurrentIndex(
      settings.value("other/type_edge").toInt());
  ui->projections_combobox->setCurrentIndex(
      settings.value("other/type_projection").toInt());
  ui->type_vertex_comboBox->setCurrentIndex(
      settings.value("other/type_vertex").toInt());
  ui->widht_lines->setValue(settings.value("other/width_lines").toInt());
  ui->widht_vertexes_slider->setValue(
      settings.value("other/width_vertex").toInt());

  update_settings();
}

void MainWindow::update_settings() {
  // clear_settings();

  formColor(ui->edge_color_slider->value(), &(ui->openGLWidget->red_edge),
            &(ui->openGLWidget->green_edge), &(ui->openGLWidget->blue_edge));

  formColor(ui->back_color_slider->value(), &(ui->openGLWidget->red_screen),
            &(ui->openGLWidget->green_screen),
            &(ui->openGLWidget->blue_screen));

  // qDebug() << ui->openGLWidget->red_screen;
  // qDebug() << ui->openGLWidget->green_screen;
  // qDebug() << ui->openGLWidget->blue_screen;

  // std::cout << "312: " << ui->back_color_slider->value()  << std::endl;

  formColor(ui->vertex_color_slider->value(), &(ui->openGLWidget->red_vertex),
            &(ui->openGLWidget->green_vertex),
            &(ui->openGLWidget->blue_vertex));

  //   std::cout << "319: " << ui->vertex_color_slider->value() << std::endl;
  ui->openGLWidget->show();
  ui->openGLWidget->update();
  // rotate_x_foo();
  // rotate_y_foo();
  // rotate_z_foo();

  // move_x_foo(1);
  // move_y_foo(1);
  // move_z_foo(1);

  zoom(&(ui->openGLWidget->model_data.matrix_3d), ui->zoom_spinBox->value(),
       ui->openGLWidget->zoom);
  ui->openGLWidget->zoom = ui->zoom_spinBox->value();

  ui->openGLWidget->width_lines = ui->width_line_spinBox->value();
  ui->openGLWidget->width_vertexes = ui->width_vertex_spinBox->value();

  ui->openGLWidget->projection = ui->projections_combobox->currentIndex();
  ui->openGLWidget->strip = ui->type_egde_comboBox->currentIndex();
  ui->openGLWidget->point_setup = ui->type_vertex_comboBox->currentIndex();

  ui->openGLWidget->width_lines = ui->widht_lines->value();
  ui->openGLWidget->width_vertexes = ui->widht_vertexes_slider->value();

  // sleep(1);
  ui->openGLWidget->show();
  ui->openGLWidget->update();
}

void MainWindow::clear_settings() {
  /// Очищает все настроечные переменный в open_gl

  ui->openGLWidget->red_edge = 1;
  ui->openGLWidget->green_edge = 0;
  ui->openGLWidget->blue_edge = 0;
  ui->openGLWidget->red_screen = 0;
  ui->openGLWidget->green_screen = 0;
  ui->openGLWidget->blue_screen = 0;
  ui->openGLWidget->red_vertex = 0;
  ui->openGLWidget->green_vertex = 0;
  ui->openGLWidget->blue_vertex = 0;

  ui->edge_color_slider->setValue(0);
  ui->back_color_slider->setValue(0);
  ui->vertex_color_slider->setValue(0);
  //__________
  ui->openGLWidget->width_lines = 1;
  ui->openGLWidget->width_vertexes = 1;
  ui->openGLWidget->projection = 0;
  ui->openGLWidget->strip = false;
  ui->widht_lines->setValue(0);
  ui->widht_vertexes_slider->setValue(0);
  ui->projections_combobox->setCurrentIndex(0);
  ui->type_egde_comboBox->setCurrentIndex(0);
  ui->type_vertex_comboBox->setCurrentIndex(0);

  ui->openGLWidget->x_offset = 0;
  ui->openGLWidget->y_offset = 0;
  ui->openGLWidget->z_offset = 0;

  ui->openGLWidget->rotate_x = 0;
  ui->openGLWidget->rotate_y = 0;
  ui->openGLWidget->rotate_z = 0;
  ui->openGLWidget->move_x = 0;
  ui->openGLWidget->move_y = 0;
  ui->openGLWidget->move_z = 0;
  ui->rotate_x_spinBox->setValue(0);
  ui->rotate_y_spinBox->setValue(0);
  ui->rotate_z_spinBox->setValue(0);
  ui->move_x_spinBox->setValue(0);
  ui->move_y_spinBox->setValue(0);
  ui->move_z_spinBox->setValue(0);
  ui->zoom_spinBox->setValue(1.0);

  ui->openGLWidget->zoom = 1;
  ui->openGLWidget->current_rotate_x = 0;
  ui->openGLWidget->current_rotate_y = 0;
  ui->openGLWidget->current_rotate_z = 0;
  ui->openGLWidget->current_move_x = 0;
  ui->openGLWidget->current_move_y = 0;
  ui->openGLWidget->current_move_z = 0;
}

void MainWindow::on_action_bmp_triggered() {
  // QPixmapfullScreen = QPixmap::grabWindow(QApplication::desktop()->winId());

  QString str = QFileDialog::getSaveFileName(ui->openGLWidget, NULL, NULL,
                                             "JPEG (*.jpeg) ;; BMP (*.bmp)");

  ui->openGLWidget->grabFramebuffer().save(str, NULL, 100);
  // QWidget::gra
  //    std::cout << str << std::endl;
}

// ROTATE X
void MainWindow::rotate_x_foo() {
  int rotate =
      (ui->openGLWidget->current_rotate_x - ui->rotate_x_spinBox->value()) *
      (-1);
  rotation_by_ox(&(ui->openGLWidget->model_data.matrix_3d), rotate * 3.14 / 180,
                 ui->move_y_spinBox->value(), ui->move_z_spinBox->value());
  // std::cout << "345:  " << rotate << std::endl;
  ui->openGLWidget->update();
  ui->openGLWidget->current_rotate_x = ui->rotate_x_spinBox->value();
}

void MainWindow::on_rotate_x_spinBox_valueChanged(int arg1) {
  arg1++;
  rotate_x_foo();
}
//______//

// ROTATE Y

void MainWindow::rotate_y_foo() {
  int rotate =
      (ui->openGLWidget->current_rotate_y - ui->rotate_y_spinBox->value()) *
      (-1);
  rotation_by_oy(&(ui->openGLWidget->model_data.matrix_3d), rotate * 3.14 / 180,
                 ui->move_x_spinBox->value(), ui->move_z_spinBox->value());

  ui->openGLWidget->update();
  ui->openGLWidget->current_rotate_y = ui->rotate_y_spinBox->value();
}
void MainWindow::on_rotate_y_spinBox_valueChanged(int arg1) {
  arg1++;
  rotate_y_foo();
}
/////////////

// ROTATE Z

void MainWindow::rotate_z_foo() {
  int rotate =
      (ui->openGLWidget->current_rotate_z - ui->rotate_z_spinBox->value()) *
      (-1);
  rotation_by_oz(&(ui->openGLWidget->model_data.matrix_3d), rotate * 3.14 / 180,
                 ui->move_x_spinBox->value(), ui->move_y_spinBox->value());
  // std::cout << "345:  " << rotate << std::endl;
  ui->openGLWidget->update();
  ui->openGLWidget->current_rotate_z = ui->rotate_z_spinBox->value();
}

void MainWindow::on_rotate_z_spinBox_valueChanged(int arg1) {
  arg1++;
  rotate_z_foo();
}
//____________________________//

// MOVE_X
void MainWindow::move_x_foo(double value) {
  value++;
  double move =
      (ui->openGLWidget->current_move_x - ui->move_x_spinBox->value()) * (-1);
  // std::cout << "439: " << move << std::endl;
  move_x(&(ui->openGLWidget->model_data.matrix_3d), move);
  ui->openGLWidget->update();
  ui->openGLWidget->current_move_x = ui->move_x_spinBox->value();
}

void MainWindow::on_move_x_spinBox_valueChanged(double arg1) {
  // ui->move_x_slider->setValue(arg1);
  move_x_foo(arg1);
}
//

// MOVE_Y

void MainWindow::move_y_foo(double value) {
  value++;
  double move =
      (ui->openGLWidget->current_move_y - ui->move_y_spinBox->value()) * (-1);
  //   std::cout << "439: " << move << std::endl;
  move_y(&(ui->openGLWidget->model_data.matrix_3d), move);
  ui->openGLWidget->update();
  ui->openGLWidget->current_move_y = ui->move_y_spinBox->value();
}

void MainWindow::on_move_y_spinBox_valueChanged(double arg1) {
  move_y_foo(arg1);
}

//

// MOVE_Z

void MainWindow::move_z_foo(double value) {
  value++;
  double move =
      (ui->openGLWidget->current_move_z - ui->move_z_spinBox->value()) * (-1);
  // std::cout << "439: " << move << std::endl;
  move_z(&(ui->openGLWidget->model_data.matrix_3d), move);
  ui->openGLWidget->update();
  ui->openGLWidget->current_move_z = ui->move_z_spinBox->value();
}

void MainWindow::on_move_z_spinBox_valueChanged(double arg1) {
  // ui->move_z_slider->setValue(ui->move_z_slider->value());
  move_z_foo(arg1);
}
//

// WIDTH_LINES_CHANDEX
void MainWindow::on_widht_lines_valueChanged(int value) {
  value++;
  width_lines_change_foo();
}

void MainWindow::width_lines_change_foo() {
  ui->openGLWidget->width_lines = ui->widht_lines->value();
  ui->width_line_spinBox->setValue(ui->widht_lines->value());
  ui->openGLWidget->update();
}

void MainWindow::on_width_line_spinBox_valueChanged(int arg1) {
  ui->widht_lines->setValue(arg1);
  width_lines_change_foo();
}
//

// WIDTH_VERTEXES_change

void MainWindow::on_widht_vertexes_slider_valueChanged(int value) {
  value++;
  width_vertex_change_foo();
}

void MainWindow::width_vertex_change_foo() {
  ui->openGLWidget->width_vertexes = ui->widht_vertexes_slider->value();
  ui->width_vertex_spinBox->setValue(ui->widht_vertexes_slider->value());
  ui->openGLWidget->update();
}

void MainWindow::on_width_vertex_spinBox_valueChanged(int arg1) {
  ui->widht_vertexes_slider->setValue(arg1);
  width_vertex_change_foo();
}

void MainWindow::on_actionSave_Gif_triggered() {
  save_ptr =
      QFileDialog::getSaveFileName(ui->openGLWidget, NULL, NULL, "GIF (*.gif)");
  make_gif();
}

void MainWindow::record_gif_file() {
  ++frame_count;
  gif->addFrame(ui->openGLWidget->grabFramebuffer(), 100);
  if (frame_count == 50) {
    timer->stop();
    ui->recording_label->setVisible(false);
    gif->save(save_ptr);
    delete gif;
    gif = nullptr;
  }
}

void MainWindow::make_gif() {
  gif = new QGifImage();
  frame_count = 0;
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(record_gif_file()));
  ui->recording_label->setVisible(true);
  timer->start(100);
}

void MainWindow::formColor(int value, float *r, float *g, float *b) {
  if (value < 32) {
    *r = value * 8;
  } else if (value < 64) {
    *r = 255;
    *g = (value - 32) * 8;
  } else if (value < 96) {
    *r = 255 - (value - 64) * 8;
    *g = 255;
  } else if (value < 128) {
    *g = 255;
    *b = (value - 96) * 8;
  } else if (value < 160) {
    *g = 255 - (value - 128) * 8;
    *b = 255;
  } else if (value < 192) {
    *r = (value - 160) * 8;
    *b = 255;
  } else if (value < 224) {
    *r = 255;
    *g = (value - 192) * 8;
    *b = 255;
  } else {
    *r = 255;
    *g = 255;
    *b = 255;
  }
  *r /= 255.0;
  *g /= 255.0;
  *b /= 255.0;
}

void MainWindow::on_vertex_color_slider_valueChanged(int value) {
  value++;
  formColor(ui->vertex_color_slider->value(), &(ui->openGLWidget->red_vertex),
            &(ui->openGLWidget->green_vertex),
            &(ui->openGLWidget->blue_vertex));
  ui->openGLWidget->update();
}

void MainWindow::on_edge_color_slider_valueChanged(int value) {
  value++;
  formColor(ui->edge_color_slider->value(), &(ui->openGLWidget->red_edge),
            &(ui->openGLWidget->green_edge), &(ui->openGLWidget->blue_edge));
  ui->openGLWidget->update();
}

void MainWindow::on_back_color_slider_valueChanged(int value) {
  value++; value--;
  formColor(ui->back_color_slider->value(), &(ui->openGLWidget->red_screen),
            &(ui->openGLWidget->green_screen),
            &(ui->openGLWidget->blue_screen));
  ui->openGLWidget->update();
}

void MainWindow::on_zoom_spinBox_valueChanged(double arg1) {
  zoom(&(ui->openGLWidget->model_data.matrix_3d), arg1, ui->openGLWidget->zoom);
  ui->openGLWidget->update();
  ui->openGLWidget->zoom = ui->zoom_spinBox->value();
}
