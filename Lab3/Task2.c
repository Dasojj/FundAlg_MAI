#include <stdio.h>
#include <stdlib.h>

void findNumbersWithOnes(int k, int l, int **result, int *resultLen) {
    int upperBound = 1 << k;
    int count = 0;
    for (int i = 0; i < upperBound; i++) {
        int numOnes = 0;
        int temp = i;
        while (temp) {
            numOnes += temp & 1;
            temp >>= 1;
        }
        if (numOnes == l) {
            count++;
        }
    }
    *result = (int *)malloc(count * sizeof(int));
    if (!result) {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return;
    }
    *resultLen = count;
    count = 0;
    for (int i = 0; i < upperBound; i++) {
        int numOnes = 0;
        int temp = i;
        while (temp) {
            numOnes += temp & 1;
            temp >>= 1;
        }
        if (numOnes == l) {
            (*result)[count] = i;
            count++;
        }
    }
}

void findNumbersWithConsecutiveOnes(int k, int l, int **result, int *resultLen) {
    int upperBound = 1 << k;
    int count = 0;
    for (int i = 0; i < upperBound; i++) {
        int numOnes = 0;
        int temp = i;
        for (int j=0;j<k;j++){
            if ((temp&1)==1){
                numOnes++;
                if(numOnes==l){
                    count++;
                    break;
                }
            }else{
                numOnes=0;
            }
            temp>>=1;
        }
    }
    *result = (int *)malloc(count * sizeof(int));
    if (!result) {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return;
    }
    *resultLen = count;
    count = 0;
    for (int i = 0; i < upperBound; i++) {
        int numOnes = 0;
        int temp = i;
        for (int j=0;j<k;j++){
            if ((temp&1)==1){
                numOnes++;
                if(numOnes==l){
                    (*result)[count] = i;
                    count++;
                    break;
                }
            }else{
                numOnes=0;
            }
            temp>>=1;
        }
    }
}

int main() {
    int k, l, choose;
    printf("Enter the number if function to complete: ");
    scanf("%d", &choose);
    if(choose > 2 || choose < 1){
        printf("Wrong function number\n");
        return 0;
    }
    printf("Enter the value of k: ");
    scanf("%d", &k);
    printf("Enter the value of l: ");
    scanf("%d", &l);
    int *result;
    int resultLen;
    if (choose == 1) findNumbersWithOnes(k, l, &result, &resultLen);
    if (choose == 2) findNumbersWithConsecutiveOnes(k, l, &result, &resultLen);
    printf("Found %d numbers: [", resultLen);
    for (int i = 0; i < resultLen; i++) {
        printf("%d", result[i]);
        if (i < resultLen - 1) {
            printf(", ");
        }
    }
    printf("]\n");
    free(result);
    return 0;
}

