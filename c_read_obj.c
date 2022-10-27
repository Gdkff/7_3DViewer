#include "c_read_obj.h"

/// @mainpage 3DViewer_v1_0
/// @section
/// Загрузка данных из obj-файла, просмотр 3D модели с возможностью управления
/// положением, настройкой параметров отрисовки, сохранения скриншотов и
/// GIF-анимации
/// @author assassin && pecangoo
/// @date 10-2022

/// @brief Определение типа строки face и количества входящих в face точек
/// @details line_type = 1: только Вершины (пример: "f 1 2 3");    line_type =
/// 2: Вершины / Текстурные координаты (пример: "f 1/1 2/2 3/3");    line_type =
/// 3: Вершины // Нормали (пример: "f 1//1 2//2 3//3");    line_type = 4:
/// Вершины / Текстурные координаты / Нормали (пример: "f 1/1/1 2/2/2 3/3/3")
/// @param line - указатель на строку face
/// @param type_line - указатель на int, куда помещается значение типа строки
/// @param num_vertex - указатель на int, куда помещается количество точек
/// @return Номер ошибки
int get_type_line_and_num_vertex(char *line, int *type_line, int *num_vertex) {
  int count_slashs = 0, err = 0;
  *num_vertex = 0;
  *type_line = 0;
  for (int i = 1; line[i]; i++) {
    if (line[i - 1] == ' ' && strchr("0123456789", line[i])) (*num_vertex)++;
  }
  for (int i = 1; line[i]; i++) {
    if (line[i] == '/') {
      count_slashs++;
      if (line[i - 1] == '/') {
        *type_line = 3;
        break;
      }
    }
  }
  if (!*type_line) {
    if (count_slashs == 0)
      *type_line = 1;
    else if (*num_vertex == count_slashs)
      *type_line = 2;
    else if (count_slashs / (*num_vertex) == 2)
      *type_line = 4;
    if (*type_line == 0)
      err = 3;  // некорректная строка грани (не идентифицируется вариант
                // значений точек в грани)
  }
  return err;
}

/// @brief Запись вершин из строки face в массив вершин
/// @details В зависимости от определенного типа строки face достает из строки
/// номера вершин и записывает их в массив. Массив подготавливается для
/// отрисовки линиями, т.е. если входящие вершины: 1 2 3 4, в масиив запишется:
/// 1 2 2 3 3 4 4 1.
/// @param line - указатель на строку face
/// @param type - тип строки face
/// @param num_vertex - указатель на int, куда помещаем количество точек
/// @param vertexes - указатель на массив вершин
/// @return Номер ошибки
int fill_face_struct(char *line, int type, int num_of_vertexes,
                     unsigned int *vertexes) {
  int err = 0, check = 0, count = 0;
  unsigned n = 0, vert, txtr, nrml;
  char temp_point[30];
  for (int i = 2; line[i]; i++) {
    if (strchr("0123456789/", line[i])) {
      while (strchr("0123456789/", line[i])) {
        temp_point[n++] = line[i++];
      }
      temp_point[n] = '\0';
      switch (type) {
        case 1:
          check = sscanf(temp_point, "%d", &vert);
          count++;
          if (check != 1)
            err = 4;  // некорректная строка грани (не считаны номера точек в
                      // грани)
          break;
        case 2:
          check = sscanf(temp_point, "%d/%d", &vert, &txtr);
          count++;
          if (check != 2)
            err = 4;  // некорректная строка грани (не считаны номера точек в
                      // грани)
          break;
        case 3:
          check = sscanf(temp_point, "%d//%d", &vert, &nrml);
          count++;
          if (check != 2)
            err = 4;  // некорректная строка грани (не считаны номера точек в
                      // грани)
          break;
        case 4:
          check = sscanf(temp_point, "%d/%d/%d", &vert, &txtr, &nrml);
          count++;
          if (check != 3)
            err = 4;  // некорректная строка грани (не считаны номера точек в
                      // грани)
          break;
        default:
          break;
      }
      n = 0;

      if (count == 1) {
        vertexes[0] = vert - 1;
      } else if (count <= num_of_vertexes) {
        vertexes[(count - 1) * 2 - 1] = vert - 1;
        vertexes[(count - 1) * 2] = vert - 1;
      }
    }
  }
  vertexes[(count - 1) * 2 + 1] = vertexes[0];
  return err;
}

/// @brief Запись вершин из строки face в основную структуру
/// @details Определяется тип строки и количество точек. В соответствии с
/// количеством точек выделяется динамическая память под массив вершин face.
/// Массив заполняется номерами вершин.
/// @param line - указатель на строку face
/// @param count - номер полигона
/// @param data - указатель на основную структуру
/// @return Номер ошибки
int face_line_read(char *line, unsigned *count, struct_data *data) {
  int num_vertex_in_line, type_line;
  int err = get_type_line_and_num_vertex(line, &type_line, &num_vertex_in_line);
  if (!err) {
    data->polygons[*count].vertexes =
        (unsigned int *)malloc(sizeof(unsigned int) * num_vertex_in_line * 2);
    data->polygons[*count].numbers_of_vertexes_in_facets =
        num_vertex_in_line * 2;
    err = fill_face_struct(line, type_line, num_vertex_in_line,
                           data->polygons[*count].vertexes);
  }
  return err;
}

/// @brief Загрузка данных из obj-файла в основную структуру
/// @param file_name - строка с расположением obj-файла
/// @param data - указатель на основную структуру
/// @return Номер ошибки
int read_obj_file(const char *file_name, struct_data *data) {
  setlocale(LC_ALL, "C");
  int err = 0;
  data->x_min = INFINITY;
  data->y_min = INFINITY;
  data->z_min = INFINITY;
  data->x_max = -INFINITY;
  data->y_max = -INFINITY;
  data->z_max = -INFINITY;
  data->matrix_3d.rows = 3;
  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    err = 5;  // ошибка открытия файла
  }
  if (!err) {
    char temp_str[5120];

    unsigned count_vertexes = 0, count_faces = 0;

    while (fgets(temp_str, sizeof(temp_str), file)) {
      if (temp_str[0] == 'v' && temp_str[1] == ' ') count_vertexes++;
      if (temp_str[0] == 'f' && temp_str[1] == ' ') count_faces++;
    }
    data->count_of_v = count_vertexes;
    data->count_of_f = count_faces;
    fseek(file, 0, SEEK_SET);
    // printf("count_vertexes: %d, count_faces: %d\n", count_vertexes,
    //        count_faces);
    data->matrix_3d.cols = count_vertexes * 3;
    data->matrix_3d.matrix =
        (double *)malloc(sizeof(double) * count_vertexes * 3);
    // сделать обработку ошибок при выделении памяти

    data->polygons = (polygon_t *)malloc(sizeof(polygon_t) * count_faces);
    // сделать обработку ошибок при выделении памяти

    unsigned i = 0;  // счетчик координат вершин
    unsigned j = 0;  // счетчик фейсов
    while (fgets(temp_str, sizeof(temp_str), file)) {
      if (temp_str[0] == 'v' && temp_str[1] == ' ') {
        sscanf(temp_str, "v %lf %lf %lf", &(data->matrix_3d.matrix[i]),
               &(data->matrix_3d.matrix[i + 1]),
               &(data->matrix_3d.matrix[i + 2]));
        if (data->matrix_3d.matrix[i] > data->x_max)
          data->x_max = data->matrix_3d.matrix[i];
        if (data->matrix_3d.matrix[i + 1] > data->y_max)
          data->y_max = data->matrix_3d.matrix[i + 1];
        if (data->matrix_3d.matrix[i + 2] > data->z_max)
          data->z_max = data->matrix_3d.matrix[i + 2];
        if (data->matrix_3d.matrix[i] < data->x_min)
          data->x_min = data->matrix_3d.matrix[i];
        if (data->matrix_3d.matrix[i + 1] < data->y_min)
          data->y_min = data->matrix_3d.matrix[i + 1];
        if (data->matrix_3d.matrix[i + 2] < data->z_min)
          data->z_min = data->matrix_3d.matrix[i + 2];
        i += 3;
      }
      if (temp_str[0] == 'f' && temp_str[1] == ' ') {
        face_line_read(temp_str, &j, data);
        j++;
      }
    }

    fclose(file);
  }
  return err;
}

/// @brief Очистка основной структуры, обнуление значений
/// @details Освобождение выделенной динамической памяти для массивов вершин
/// face и матрицы координат вершин. Обнуление количественных значений.
/// @param data - указатель на основную структуру
void clean_data_struct(struct_data *data) {
  if (data->count_of_f != 0) {
    for (unsigned i = 0; i < data->count_of_f; i++) {
          free(data->polygons[i].vertexes);
    }
       free(data->polygons);
       free(data->matrix_3d.matrix);

    data->count_of_f = 0;
    data->count_of_v = 0;
    data->matrix_3d.cols = 0;
    data->matrix_3d.rows = 0;
  }
}

/// @brief Очистка основной структуры, обнуление значений
/// @details Освобождение выделенной динамической памяти для массивов вершин
/// face и матрицы координат вершин. Обнуление количественных значений.
/// @param data - указатель на основную структуру
int start_parcer(struct_data *data, char *filename) {
  int ret = 0;
  ret = read_obj_file(filename, data);
  data->matrix_3d.rows = data->count_of_v;  // Количество рядов в струкуре
                                            // соотвествует количеству строчек
  data->matrix_3d.cols = 3;
  central_aligh(data);
  // show_data_stuct(*data);
  return ret;
}

/// @brief Вывод в sdout данных основной структуры
/// @return Номер ошибки
int show_data_stuct(struct_data data) {
  int ret = 0;
  printf("Кол-во точек: %ld\n", data.count_of_v);
  printf("Кол-во полигонов: %ld\n", data.count_of_f);
  printf("Кол-во colums: %d\n", data.matrix_3d.cols);
  printf("Кол-во rows: %d\n", data.matrix_3d.rows);
  int ki = 0;
  for (unsigned long i = 0; i < ((unsigned long)data.matrix_3d.cols) *
                                    (unsigned long)data.matrix_3d.rows;
       i++) {
    printf("%ld: ", i);
    printf("%f ", data.matrix_3d.matrix[i]);
    ki++;
    if (ki == 3) {
      printf("\n");
      ki = 0;
    }
  }

  for (unsigned long i = 0; i < data.count_of_f; i++) {
    printf("%ld: ", i + 1);
    for (int k = 0; k < data.polygons[i].numbers_of_vertexes_in_facets; k++) {
      printf("%u ", data.polygons[i].vertexes[k]);
    }
    printf("\n");
  }
  return ret;
}

/// @brief Центровка модели по осям x и y по началу координат, уменьшение модели
/// до границ [-0.9, 0.9] по x и y.
/// @param A - указатель на основную структуру
void central_aligh(struct_data *A) {
  double offset_x = (A->x_max + A->x_min) / 2;
  double offset_y = (A->y_max + A->y_min) / 2;
  double offset_z = (A->z_max + A->z_min) / 2;
  double max_lenght = 0;
  if (A->x_max - A->x_min > max_lenght) {
    max_lenght = A->x_max - A->x_min;
  }
  if (A->y_max - A->y_min > max_lenght) {
    max_lenght = A->y_max - A->y_min;
  }
  double zoom = 2 / max_lenght * 0.9;
  A->x_min = INFINITY;
  A->y_min = INFINITY;
  A->z_min = INFINITY;
  A->x_max = -INFINITY;
  A->y_max = -INFINITY;
  A->z_max = -INFINITY;
  for (int i = 0; i < A->matrix_3d.rows; i++) {
    A->matrix_3d.matrix[(i * 3) + 0] -= offset_x;
    A->matrix_3d.matrix[(i * 3) + 1] -= offset_y;
    A->matrix_3d.matrix[(i * 3) + 2] -= offset_z;
    A->matrix_3d.matrix[(i * 3) + 0] *= zoom;
    A->matrix_3d.matrix[(i * 3) + 1] *= zoom;
    A->matrix_3d.matrix[(i * 3) + 2] *= zoom;
    if (A->matrix_3d.matrix[(i * 3) + 0] > A->x_max)
      A->x_max = A->matrix_3d.matrix[(i * 3) + 0];
    if (A->matrix_3d.matrix[(i * 3) + 1] > A->y_max)
      A->y_max = A->matrix_3d.matrix[(i * 3) + 1];
    if (A->matrix_3d.matrix[(i * 3) + 2] > A->z_max)
      A->z_max = A->matrix_3d.matrix[(i * 3) + 2];
    if (A->matrix_3d.matrix[(i * 3) + 0] < A->x_min)
      A->x_min = A->matrix_3d.matrix[(i * 3) + 0];
    if (A->matrix_3d.matrix[(i * 3) + 1] < A->y_min)
      A->y_min = A->matrix_3d.matrix[(i * 3) + 1];
    if (A->matrix_3d.matrix[(i * 3) + 2] < A->z_min)
      A->z_min = A->matrix_3d.matrix[(i * 3) + 2];
  }
}
