
#include "c_affins.h"

#include <math.h>

#include "c_read_obj.h"

void move_x(matrix_t *matrix, double a) {
  /// Move X
  /*!
     Move matrix x
  */

  for (int i = 0; i < matrix->rows; i++) {
    matrix->matrix[i * 3] += a;
  }
}

void move_y(matrix_t *matrix, double a) {
  /// Move Y
  /*!
     Move matrix y
  */
  for (int i = 0; i < matrix->rows; i++) {
    matrix->matrix[(i * 3) + 1] += a;
  }
}

void move_z(matrix_t *matrix, double a) {
  /// Move Z
  /*!
     Move matrix z
  */
  for (int i = 0; i < matrix->rows; i++) {
    matrix->matrix[(i * 3) + 2] += a;
  }
}

void rotation_by_ox(matrix_t *A, double angle, double offset_y,
                    double offset_z) {
  /// Rotate OX
  /*!
     Rotate matrix ox
  */
  for (int i = 0; i < A->rows; i++) {
    //  printf("Y_ Start: %f\n", A->matrix[(i*3)+1]);
    A->matrix[(i * 3) + 1] -= offset_y;
    A->matrix[(i * 3) + 2] -= offset_z;
    // printf("Y_ After_ Offset: %f\n", A->matrix[(i * 3) + 1]);
    double temp_y = A->matrix[(i * 3) + 1];
    double temp_z = A->matrix[(i * 3) + 2];
    A->matrix[(i * 3) + 1] = cos(angle) * temp_y + sin(angle) * temp_z;
    A->matrix[(i * 3) + 2] = sin(angle) * (-temp_y) + cos(angle) * temp_z;

    // offset_y += A->matrix[(i*3)+1] - temp_y;
    // offset_z += A->matrix[(i*3)+2] - temp_z;

    // printf("35 offset_y: %f\n", offset_y);
    fflush(stdout);
    A->matrix[(i * 3) + 1] += offset_y;
    A->matrix[(i * 3) + 2] += offset_z;
    // printf("Y_ Last: %f\n", A->matrix[(i * 3) + 1]);
  }
}

void rotation_by_oy(matrix_t *A, double angle, double offset_x,
                    double offset_z) {
  /// Rotate OY
  /*!
     Rotate matrix oy
  */
  for (int i = 0; i < A->rows; i++) {
    A->matrix[(i * 3) + 0] -= offset_x;
    A->matrix[(i * 3) + 2] -= offset_z;
    double temp_x = A->matrix[(i * 3) + 0];
    double temp_z = A->matrix[(i * 3) + 2];
    A->matrix[(i * 3) + 0] = cos(angle) * temp_x + sin(angle) * temp_z;
    A->matrix[(i * 3) + 2] = sin(angle) * (-temp_x) + cos(angle) * temp_z;
    // printf("52 offset_x: %f\n", *offset_x);
    // printf("52 offset_z: %f\n", *offset_z);
    fflush(stdout);
    A->matrix[(i * 3) + 0] += offset_x;
    A->matrix[(i * 3) + 2] += offset_z;
  }
}

void rotation_by_oz(matrix_t *A, double angle, double offset_x,
                    double offset_y) {
  /// Rotate OZ
  /*!
     Rotate matrix oz
  */
  for (int i = 0; i < A->rows; i++) {
    A->matrix[(i * 3) + 0] -= offset_x;
    A->matrix[(i * 3) + 1] -= offset_y;
    double temp_x = A->matrix[(i * 3) + 0];
    double temp_y = A->matrix[(i * 3) + 1];
    A->matrix[(i * 3) + 0] = cos(angle) * temp_x - sin(angle) * temp_y;
    A->matrix[(i * 3) + 1] = sin(angle) * (temp_x) + cos(angle) * temp_y;
    A->matrix[(i * 3) + 0] += offset_x;
    A->matrix[(i * 3) + 1] += offset_y;
  }
}

void zoom(matrix_t *A, double zoom, double current_zoom) {
  /// ZOOM
  /*!
     zoom matrix
  */
  if (zoom > 0) {
    for (int i = 0; i < A->rows; i++) {
      A->matrix[(i * 3) + 0] = A->matrix[(i * 3) + 0] / current_zoom * zoom;
      A->matrix[(i * 3) + 1] = A->matrix[(i * 3) + 1] / current_zoom * zoom;
      A->matrix[(i * 3) + 2] = A->matrix[(i * 3) + 2] / current_zoom * zoom;
    }
  }
}
