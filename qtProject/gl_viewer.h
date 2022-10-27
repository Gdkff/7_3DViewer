
#ifndef GL_VIEWER_H
#define GL_VIEWER_H
#define GL_SILENCE_DEPRECATION
#include <string.h>

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QOpenGLWidget>
#include <iostream>
//#include <OpenGL/gl.h>

extern "C" {
#include "../c_read_obj.h"
}

class gl_viewer : public QOpenGLWidget {
 public:
  gl_viewer(QWidget *parent = nullptr);
  struct_data model_data;
  void init_test();
  void load_model(std::string path);

  GLint point_setup = 0;

  GLfloat red_edge = 1, green_edge = 0, blue_edge = 0;
  GLfloat red_screen = 0, green_screen = 0, blue_screen = 0;
  GLfloat red_vertex = 0, green_vertex = 0, blue_vertex = 0;
  GLfloat width_lines = 1;
  GLfloat width_vertexes = 1;
  GLdouble w_h_ratio;
  int out_width, out_height, proection_ortho = 1;
  int projection = 0;
  int strip = false;

  float x_offset = 0;
  float y_offset = 0;
  float z_offset = 0;

  float rotate_x = 0;
  float rotate_y = 0;
  float rotate_z = 0;
  float move_x = 0;
  float move_y = 0;
  float move_z = 0;

  double zoom = 1;

  int load_setup = 0;

  int current_rotate_x = 0, current_rotate_y = 0, current_rotate_z = 0;
  double current_move_x = 0, current_move_y = 0, current_move_z = 0;
  char project_name[80] = {0};

 private:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
};

#endif  // GL_VIEWER_H
