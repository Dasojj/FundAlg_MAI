#include <stdio.h>
#include <stdlib.h>

int **create_matrix(int lines, int columns){
    if(lines > 10 || lines < 1) return NULL;
    if(columns > 10 || columns < 1) return NULL;
    int **arr = (int**)malloc(lines * sizeof(int*));
    if (!arr) {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return NULL;
    }
    for(int i = 0; i < lines; i++){
        arr[i] = (int*)malloc(columns * sizeof(int));
        if (!arr[i]) {
            fprintf(stderr, "malloc() failed: insufficient memory!\n");
            return NULL;
        }
        for(int j = 0; j < columns; j++)
            arr[i][j] = rand() % 201 - 100;
    }
    return arr;
}
int **create_empty_matrix(int lines, int columns){
    int **arr = (int**)malloc(lines * sizeof(int*));
    if (!arr) {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return NULL;
    }
    for(int i = 0; i < lines; i++){
        arr[i] = (int*)malloc(columns * sizeof(int));
        if (!arr[i]) {
            fprintf(stderr, "malloc() failed: insufficient memory!\n");
            return NULL;
        }
    }
    return arr;
}


void free_arr(int ***arr, int lines){
    for(int i = 0; i < lines; i++){
        free((*arr)[i]);
    }
    free(*arr);
}

int **matrix_equation(int **arr1, int **arr2, int l1, int c1, int l2, int c2){
    if(c1 != l2){
        printf("Эти матрицы невозможно умножить\n");
        return NULL;
    }
    int **C = create_empty_matrix(l1, c2);
    for(int i = 0; i < l1; i++){
        for(int j = 0; j < c2; j++){
            C[i][j] = 0;
            for(int k = 0; k < c1; k++)
                C[i][j] += arr1[i][k] * arr2[k][j];
        }
    }
    return C;
}

double find_determinant(int **arr, int n){
    double det;
    int subj, s;
    int **subarr;
    switch(n){
        case 1:
            return arr[0][0];
            break;
        case 2:
            return arr[0][0] * arr[1][1] - arr[0][1] * arr[1][0];
            break;
        default:
            subarr = (int**)malloc(n-1 * sizeof(int*));
            det = 0;
            s = 1;
            for(int i = 0; i < n; i++){
                subj = 0;
                for(int j = 0; j < n; j++)
                    if(i != j) subarr[subj++] = arr[j] + 1;
                det += s * arr[i][0] * find_determinant(subarr, n - 1);
                s = -s;
            }
            for(int i = 0; i < n; i++) free(subarr[i]);
            free(subarr);
            return det;
            break;
    }
}

int main(){
    int action;
    int **matrix1 = NULL;
    int **matrix2 = NULL;
    int **eq_matrix = NULL;
    double determinant;
    int l1, c1, l2, c2;
    while(1){
        printf("Меню:\n");
        printf("1.Сгенерировать матрицу в слот 1\n");
        printf("2.Сгенерировать матрицу в слот 2\n");
        printf("3.Умножить матрицу из слота 1 на матрицу из слота 2\n");
        printf("4.Вычислить определитель произведения матриц\n");
        printf("5.Освободить слот\n");
        scanf("%d", &action);
        switch(action){
            case 1:
                printf("Введите количество строк матрицы:\n");
                scanf("%d", &l1);
                printf("Введите количество столбцов матрицы:\n");
                scanf("%d", &c1);
                matrix1 = create_matrix(l1, c1);
                break;
            case 2:
                printf("Введите количество строк матрицы:\n");
                scanf("%d", &l2);
                printf("Введите количество столбцов матрицы:\n");
                scanf("%d", &c2);
                matrix2 = create_matrix(l2, c2);
                break;
            case 3:
                eq_matrix = matrix_equation(matrix1, matrix2, l1, c1, l2, c2);
                for(int i = 0; i < l1; i++){
                    for(int j = 0; j < c2; j++){
                        printf("%d ", eq_matrix[i][j]);
                    }
                    printf("\n");
                }
                break;
            case 4:
                if(l1 != c2) printf("Матрица не квадратная - определитель не может быть найден\n");
                else{
                    determinant = find_determinant(eq_matrix, l1);
                    printf("Определитель: %f\n", determinant);
                }
                break;
            case 5:
                printf("Введите номер слота для освобождения:\n");
                int slot_to_free;
                scanf("%d", &slot_to_free);
                if(slot_to_free == 1){
                    free_arr(&matrix1, l1);
                    printf("Освобождено\n");
                }
                else if(slot_to_free == 2){
                    free_arr(&matrix2, l2);
                    printf("Освобождено\n");
                }
                else printf("Освободить можно только слот 1, или слот 2\n");
                break;
            default:
                printf("Неверная функция\n");
                break;
        }
    }
}
