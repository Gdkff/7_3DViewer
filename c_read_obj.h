#ifndef C_READ_OBJ_H
#define C_READ_OBJ_H

#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// @brief Структура с матрицей для хранения массива координат вершин
typedef struct Matrix {
  double *matrix; /*!< указатель на матрицу*/
  int cols;       /*!< количество столбцов в матрице*/
  int rows;       /*!< количество строк в матрице*/
} matrix_t;

/// @brief Структура хранящая номера вершин одной face
typedef struct facets {
  unsigned int *vertexes; /*!< указатель на массив вершин одной face*/
  int numbers_of_vertexes_in_facets; /*!< количество вершин в массиве*/
} polygon_t;

/// @brief Основная структура, хранящая данные из obj-файла
typedef struct data {
  unsigned long count_of_v; /*!< количество вершин (vertex)*/
  unsigned long count_of_f; /*!< количество полигонов (face)*/
  matrix_t matrix_3d; /*!< структура с массивом координат vertex*/
  polygon_t *polygons; /*!< массив указателей на структуры face*/
  double x_min; /*!< минимальное значение по Х*/
  double y_min; /*!< минимальное значение по Y*/
  double z_min; /*!< минимальное значение по Z*/
  double x_max; /*!< максиальное значение по Х*/
  double y_max; /*!< максиальное значение по Y*/
  double z_max; /*!< максиальное значение по Z*/
} struct_data;

int get_type_line_and_num_vertex(char *line, int *type_line, int *num_vertex);
int fill_face_struct(char *line, int type, int num_of_vertexes,
                     unsigned int *vertexes);
int face_line_read(char *line, unsigned *count, struct_data *data);
int read_obj_file(const char *file_name, struct_data *data);
void clean_data_struct(struct_data *data);
int show_data_stuct(struct_data data);
void central_aligh(struct_data *A);
int start_parcer(struct_data *data, char *filename);
#endif  // C_READ_OBJ_H
