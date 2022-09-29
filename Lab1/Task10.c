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
        a[i] = (int*)malloc(columns * sizeof(int));
        if (!a[i]) {
            fprintf(stderr, "malloc() failed: insufficient memory!\n");
            return NULL;
        }
        for(int j = 0; j < columns; j++)
            a[i][j] = rand() % 201 - 100;
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
        printf("Эти матрицы невозможно умножить");
        return NULL;
    }
    int C[l1][c2];
    for(i = 0; i < l1; i++){
        for(j = 0; j < c2; j++){
            C[i][j] = 0;
            for(k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
    }
}

int main(){
    int action;
    printf("Меню:\n");
    printf("1.Сгенерировать матрицу в слот 1\n");
    printf("2.Сгенерировать матрицу в слот 2\n");
    printf("3.Умножить матрицу из слота 1 на матрицу из слота 2\n");
    printf("4.Вычислить определитель произведения матриц\n");
    printf("5.Освободить слот\n");
    scanf("%d", &action);
    int **matrix1 = NULL;
    int **matrix2 = NULL;
    int l, c;
    switch(action){
        case 1:
            printf("Введите количество строк матрицы:\n");
            scanf("%d", l);
            printf("Введите количество столбцов матрицы:\n");
            scanf("%d", c);
            matrix1 = create_matrix(l, c);
            break;
        case 2:
            printf("Введите количество строк матрицы:\n");
            scanf("%d", l);
            printf("Введите количество столбцов матрицы:\n");
            scanf("%d", c);
            matrix2 = create_matrix(l, c);
            break;
        case 3:
            
            break;
        case 4:
            
            break;
        case 5:
            printf("Введите номер слота для освобождения:\n");
            int slot_to_free;
            scanf("%d", &slot_to_free);
            if(slot_to_free == 1){
                free_arr(matrix1, l);
                printf("Освобождено");
            }
            else if(slot_to_free == 2){
                free_arr(matrix1, l);
                printf("Освобождено");
            }
            else printf("Освободить можно только слот 1, или слот 2");
            break;
        default:
            
            break;
    }
}
