#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10

void fillArray(int *arr) {
    srand(time(0));
    for (int i = 0; i < SIZE; i++) {
        arr[i] = rand() % 100;
    }
}

void findMinMax(int *arr, int *min, int *max) {
    *min = *max = arr[0];
    for (int i = 1; i < SIZE; i++) {
        if (arr[i] < *min) {
            *min = arr[i];
        }
        if (arr[i] > *max) {
            *max = arr[i];
        }
    }
}

void swapMinMax(int *arr, int min, int max) {
    int minIndex, maxIndex;

    for (int i = 0; i < SIZE; i++) {
        if (arr[i] == min) {
            minIndex = i;
        }
        if (arr[i] == max) {
            maxIndex = i;
        }
    }

    int temp = arr[minIndex];
    arr[minIndex] = arr[maxIndex];
    arr[maxIndex] = temp;
}

int main() {
    int arr[SIZE];
    int min, max;
    fillArray(arr);
    printf("Array filled with random numbers: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    findMinMax(arr, &min, &max);
    printf("Min of an array: %d\nMax of an array: %d\n", min, max);
    swapMinMax(arr, min, max);
    printf("New array after swapping min and max: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}
