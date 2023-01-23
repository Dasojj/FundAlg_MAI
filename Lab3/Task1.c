#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int decimal_to_base_2_r(int decimal, int r) {
    if (r < 1 || r > 5) return -1;
    int base = 1 << r;
    int quotient = decimal;
    int remainder;
    int binary[32];
    int i = 0;
    while (quotient) {
        remainder = quotient & (base - 1);
        quotient = quotient >> r;
        binary[i] = remainder;
        i++;
    }
    int res = 0;
    for (int j = 0; j < i; j++) {
        res += binary[j] * pow(10, j);
    }
    return res;
}

int main(){
    int dec, r;
    printf("Enter the decimal: ");
    scanf("%d", &dec);
    printf("Enter r: ");
    scanf("%d", &r);
    printf("Number %d in system with base 2^%d(%d) is %d\n", dec, r, (int)pow(2, r), decimal_to_base_2_r(dec, r));
    return 0;
}
