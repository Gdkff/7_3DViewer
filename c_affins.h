#ifndef AFFINS_H
#define AFFINS_H
#include "c_read_obj.h"

void move_x(matrix_t *matrix, double a);
void move_y(matrix_t *matrix, double a);
void move_z(matrix_t *matrix, double a);

void rotation_by_ox(matrix_t *A, double angle, double offset_y, double offset_z);
void rotation_by_oy(matrix_t *A, double angle, double offset_x, double offset_z);
void rotation_by_oz(matrix_t *A, double angle, double offset_x, double offset_y);


void zoom(matrix_t *A, double zoom, double current_zoom);

#endif // AFFINS_H
