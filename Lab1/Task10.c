#include <stdio.h>
#include <stdlib.h>

struct dynamic_matrix {
    double **matrix;
    int rows;
    int columns;
};

typedef struct dynamic_matrix dynamic_matrix;

int init_matrix(dynamic_matrix *matrix, int rows, int columns) {
    matrix->matrix = (double **)malloc(rows * sizeof(double *));
    if (matrix->matrix == NULL) return 0;
    for (int i = 0; i < rows; i++) {
        matrix->matrix[i] = (double *)malloc(columns * sizeof(double));
        if (matrix->matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matrix->matrix[j]);
            }
            free(matrix->matrix);
            return 0;
        }
    }
    matrix->rows = rows;
    matrix->columns = columns;
    return 1;
}

void fill_random(dynamic_matrix *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->columns; j++) {
            matrix->matrix[i][j] = rand() % 201 - 100;
        }
    }
}

void free_matrix(dynamic_matrix *matrix) {
    if(matrix->matrix == NULL) return;
    for (int i = 0; i < matrix->rows; i++) {
        free(matrix->matrix[i]);
    }
    free(matrix->matrix);
    matrix->matrix = NULL;
}

dynamic_matrix matrix_equation(const dynamic_matrix *matrix1, const dynamic_matrix *matrix2) {
    if(matrix1->columns != matrix2->rows) {
        dynamic_matrix empty = {NULL, 0, 0};
        return empty;
    }
    dynamic_matrix result;
    if(!init_matrix(&result, matrix1->rows, matrix2->columns)) {
        dynamic_matrix empty = {NULL, 0, 0};
        return empty;
    }
    for(int i = 0; i < matrix1->rows; i++) {
        for(int j = 0; j < matrix2->columns; j++) {
            result.matrix[i][j] = 0;
            for(int k = 0; k < matrix1->columns; k++) {
                result.matrix[i][j] += matrix1->matrix[i][k] * matrix2->matrix[k][j];
            }
        }
    }
    return result;
}

double find_determinant(const dynamic_matrix *mat) {
    if(mat->rows != mat->columns) return 0;
    int i, j, k;
    double tmp, det = 1;
    for(i = 0; i < mat->rows; i++) {
        for(j = i + 1; j < mat->rows; j++) {
            tmp = mat->matrix[j][i] / mat->matrix[i][i];
            for(k = i; k < mat->rows; k++) {
                mat->matrix[j][k] -= mat->matrix[i][k] * tmp;
            }
        }
    }
    for(i = 0; i < mat->rows; i++) {
        det *= mat->matrix[i][i];
    }
    return det;
}

void print_matrix(const dynamic_matrix *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->columns; j++) {
            printf("%lf ", matrix->matrix[i][j]);
        }
        printf("\n");
    }
}

int main(){
    int action;
    dynamic_matrix matrix1;
    dynamic_matrix matrix2;
    dynamic_matrix eq_matrix;
    double determinant;
    int r, c;
    while(1){
        printf("Меню:\n");
        printf("1.Сгенерировать матрицу в слот 1\n");
        printf("2.Сгенерировать матрицу в слот 2\n");
        printf("3.Умножить матрицу из слота 1 на матрицу из слота 2\n");
        printf("4.Вычислить определитель произведения матриц\n");
        printf("5.Освободить слот\n");
        printf("6.Завершить программу\n");
        scanf("%d", &action);
        switch(action){
            case 1:
                printf("Введите количество строк матрицы:\n");
                scanf("%d", &r);
                printf("Введите количество столбцов матрицы:\n");
                scanf("%d", &c);
                if(!init_matrix(&matrix1, r, c)) printf("Error initializing matrix");
                else fill_random(&matrix1);
                break;
            case 2:
                printf("Введите количество строк матрицы:\n");
                scanf("%d", &r);
                printf("Введите количество столбцов матрицы:\n");
                scanf("%d", &c);
                if(!init_matrix(&matrix2, r, c)) printf("Error initializing matrix");
                else fill_random(&matrix2);
                break;
            case 3:
                eq_matrix = matrix_equation(&matrix1, &matrix2);
                if(eq_matrix.matrix == NULL){
                    printf("Error equationing matrix\n");
                    break;
                }
                print_matrix(&eq_matrix);
                break;
            case 4:
                determinant = find_determinant(&eq_matrix);
                printf("Определитель: %f\n", determinant);
                break;
            case 5:
                printf("Введите номер слота для освобождения:\n");
                int slot_to_free;
                scanf("%d", &slot_to_free);
                if(slot_to_free == 1){
                    free_matrix(&matrix1);
                    printf("Освобождено\n");
                }
                else if(slot_to_free == 2){
                    free_matrix(&matrix1);
                    printf("Освобождено\n");
                }
                else printf("Освободить можно только слот 1, или слот 2\n");
                break;
            case 6:
                free_matrix(&matrix1);
                free_matrix(&matrix2);
                free_matrix(&eq_matrix);
                exit(0);
                break;
            default:
                printf("Неверная функция\n");
                break;
        }
    }
}
