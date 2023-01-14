#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 5

int comp(const void *i, const void *j) {
    return *(int *)i - *(int *)j;
}

int binary_search(int *arr, int low, int high, int x) {
    if (high >= low) {
        int mid = (high + low) / 2;
        if (arr[mid] == x) {
            return mid;
        }
        if (arr[mid] > x) {
            return binary_search(arr, low, mid - 1, x);
        }
        return binary_search(arr, mid + 1, high, x);
    }
    return low;
}

void closest_element(int *A, int *B, int *C, int sizeA, int sizeB) {
    int i;
    qsort(B, sizeB, sizeof(int), comp);
    for (i = 0; i < sizeA; i++) {
        int closest_index = binary_search(B, 0, sizeB - 1, A[i]);
        if (closest_index == 0 || closest_index == sizeB - 1) {
            C[i] = A[i] + B[closest_index];
        } else if (abs(A[i] - B[closest_index - 1]) < abs(A[i] - B[closest_index])) {
                C[i] = A[i] + B[closest_index - 1];
        } else {
                C[i] = A[i] + B[closest_index];
        }
    }
}

int main() {
    srand(time(0));
    int sizeB = rand() % 6 + 3;
    int A[SIZE], B[sizeB], C[SIZE];
    printf("Array A: ");
    for(int i = 0; i < SIZE; i++){
        A[i] = rand() % 9 + 1;
        printf("%d ", A[i]);
    }
    printf("\nArray B: ");
    for(int i = 0; i < sizeB; i++){
        B[i] = rand() % 9 + 1;
        printf("%d ", B[i]);
    }
    closest_element(A, B, C, SIZE, sizeB);
    printf("\nResult C array: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", C[i]);
    }
    printf("\n");
    return 0;
}

