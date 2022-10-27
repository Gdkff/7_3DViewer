#include "gl_viewer.h"

gl_viewer::gl_viewer(QWidget *parent) : QOpenGLWidget(parent) {}

void gl_viewer::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
}

void gl_viewer::resizeGL(int w, int h) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  w_h_ratio = (GLfloat)w / h;
  out_width = w;
  out_height = h;

  if (projection == 1) {
    glFrustum(-0.7 * w_h_ratio, 0.7 * w_h_ratio, -0.7, 0.7, 2, 20);
  } else if (projection == 0) {
    glOrtho(-1.0 * w_h_ratio, 1.0 * w_h_ratio, -1, 1, 0, 20);
  }
  glTranslatef(0, 0, -3);
  glViewport(0, 0, (GLint)(w * 2), (GLint)h * 2);
}

void gl_viewer::paintGL() {
  if (model_data.count_of_v != 0) {
    resizeGL(out_width, out_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColor3f(red_edge, green_edge, blue_edge);
    glClearColor(red_screen, green_screen, blue_screen, 0);

    glVertexPointer(3, GL_DOUBLE, 0, model_data.matrix_3d.matrix);

    glLineWidth(width_lines);

    if (strip == 1) {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x00F0);
    }

    for (unsigned long i = 0; i < model_data.count_of_f; i++) {
      glDrawElements(GL_LINES,
                     model_data.polygons[i].numbers_of_vertexes_in_facets,
                     GL_UNSIGNED_INT, model_data.polygons[i].vertexes);
    }

    if (strip == 1) glDisable(GL_LINE_STIPPLE);

    if (point_setup) {
      glPointSize(width_vertexes);
      glColor3f(red_vertex, green_vertex, blue_vertex);
      if (point_setup == 1) glEnable(GL_POINT_SMOOTH);
      glDrawArrays(GL_POINTS, 0, model_data.count_of_v);
      if (point_setup == 1) glDisable(GL_POINT_SMOOTH);
    }
  }
}

void gl_viewer::init_test() {
  char file_name[256] = "/Users/svetislavdobromirov/Downloads/cubes.obj";
  start_parcer(&model_data, file_name);
  update();
}

void gl_viewer::load_model(std::string path) {
  char file_name[256] = {0};
  strcpy(file_name, path.c_str());
  //clean_data_struct(&model_data);
  start_parcer(&model_data, file_name);
  update();
}
