#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int min, max, n, i, choice;
    printf("Enter the range of numbers (min max): ");
    scanf("%d%d", &min, &max);
    printf("Enter the number of elements in the array: ");
    scanf("%d", &n);
    int arr[n];
    srand(time(0));
    printf("Original array: ");
    for (i = 0; i < n; i++) {
        arr[i] = (rand() % (max - min + 1)) + min;
        printf("%d ", arr[i]);
    }
    printf("\n");
    printf("Enter 1 to swap min and max elements, 2 to create new array with unique elements: ");
    scanf("%d", &choice);
    if (choice == 1) {
        int min_index, max_index;
        min_index = max_index = 0;
        for (i = 1; i < n; i++) {
            if (arr[i] < arr[min_index]) {
                min_index = i;
            }
            if (arr[i] > arr[max_index]) {
                max_index = i;
            }
        }
        swap(&arr[min_index], &arr[max_index]);
        printf("Array with min and max elements swapped: ");
        for (i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
    } else if (choice == 2) {
        int j, unique_count = 0;
        int unique_arr[n];
        for(i = 0; i < n; i++) {
            int isDuplicate = 0;
            for (j = 0; j < unique_count; j++) {
                if (arr[i] == unique_arr[j]) {
                    isDuplicate = 1;
                    break;
                }
            }
            if (!isDuplicate) {
                unique_arr[unique_count++] = arr[i];
            }
        }
        printf("New array with unique elements: ");
        for (i = 0; i < unique_count; i++) {
            printf("%d ", unique_arr[i]);
        }
    }
    printf("\n");
}
