#include <stdio.h>
#include <stdlib.h>

int *find_min_max(int *arr, int size){
    int min = arr[0];
    int max = arr[0];
    int *inds = (int*)malloc(sizeof(int)*2);
    inds[0] = 0;
    inds[1] = 0;
    for(int i = 1; i < size; i++){
        if(arr[i] > max){
            max = arr[i];
            inds[1] = i;
        }
        if(arr[i] < min){
            min = arr[i];
            inds[0] = i;
        }
    }
    return inds;
}

int main(){
    int arr[10];
    for(int i = 0; i < 10; i++){
        arr[i] = rand()%100;
        printf("%d ", arr[i]);
    }
    printf("\n");
    int *inds = find_min_max(arr, 10);
    printf("%d %d", arr[inds[0]], arr[inds[1]]);
    printf("\n");
    int temp = arr[inds[0]];
    arr[inds[0]] = arr[inds[1]];
    arr[inds[1]] = temp;
    for(int i = 0; i < 10; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}
