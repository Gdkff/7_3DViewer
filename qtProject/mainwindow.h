#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QTimer>

#include "gl_viewer.h"
#include "qgifglobal.h"
#include "qgifimage.h"
#include "qgifimage_p.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  int pos_mouse_x = 0, pos_mouse_y = 0;
  int pos_rotate_mouse_x = 0, pos_rotate_mouse_y = 0;
  std::string path_model = "";
  //  int load_setup = 0;
  int frame_count = 0;
  QGifImage *gif = nullptr;
  QTimer *timer = nullptr;
  QString save_ptr = "";

 private slots:

  void on_actionImport_Model_triggered();

  void on_widht_lines_valueChanged(int value);

  void on_widht_vertexes_slider_valueChanged(int value);
  void on_projections_combobox_currentIndexChanged(int index);

  void on_actionSave_settings_triggered();
  void on_actionLoad_settings_triggered();

  void rotate_x_foo();
  void rotate_y_foo();
  void rotate_z_foo();

  void writeSettings(QString path);
  void closeEvent(QCloseEvent *event);
  void readSettings(QString str);
  void import_model(std::string path_model);

  void on_action_bmp_triggered();

  void move_x_foo(double value);
  void move_y_foo(double value);
  void move_z_foo(double value);

  void on_rotate_x_spinBox_valueChanged(int arg1);
  void on_rotate_y_spinBox_valueChanged(int arg1);
  void on_rotate_z_spinBox_valueChanged(int arg1);

  void on_move_x_spinBox_valueChanged(double arg1);

  void on_move_z_spinBox_valueChanged(double arg1);

  void width_lines_change_foo();
  void on_width_line_spinBox_valueChanged(int arg1);
  void on_width_vertex_spinBox_valueChanged(int arg1);
  void width_vertex_change_foo();

  void on_actionSave_Gif_triggered();

  void make_gif();
  void record_gif_file();

  void formColor(int value, float *r, float *g, float *b);

  void on_vertex_color_slider_valueChanged(int value);

  void on_edge_color_slider_valueChanged(int value);

  void on_back_color_slider_valueChanged(int value);
  void on_move_y_spinBox_valueChanged(double arg1);

  void on_zoom_spinBox_valueChanged(double arg1);

  // void on_comboBox_activated(int index);
  void on_type_vertex_comboBox_activated(int index);
  void on_type_egde_comboBox_activated(int index);

  void update_settings();
  void clear_settings();

 private:
  Ui::MainWindow *ui;
};
#endif  // MAINWINDOW_H
