#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    int lenA, lenB;
    printf("Введите длину массива А:\n");
    scanf("%d", &lenA);
    printf("Введите длину массива B:\n");
    scanf("%d", &lenB);
    int *A = (int*)malloc(lenA * sizeof(int));
    int *B = (int*)malloc(lenB * sizeof(int));
    int *C = (int*)malloc(lenA * sizeof(int));
    for(int i = 0; i < lenA; i++){
        A[i] = rand() % 101;
        printf("%d ", A[i]);
    }
    printf("\n");
    for(int i = 0; i < lenB; i++){
        B[i] = rand() % 101;
        printf("%d ", B[i]);
    }
    printf("\n");
    for(int i = 0; i < lenA; i++){
        if(i < lenB) C[i] = A[i] + B[i];
        else C[i] = A[i] + B[lenB - 1];
        printf("%d ", C[i]);
    }
    printf("\n");
    return 0;
}
