#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

int check_finite(double decimal, int base) {
    if(base == 10) return 1;
    int i = 0;
    double fraction = decimal - (int)decimal;
    while (fraction != 0) {
        fraction *= base;
        if (fraction >= 1) fraction -= 1;
        i++;
        if (i > log(INT_MAX)/log(base) + 1) return 0;
    }
    return 1;
}

double* has_finite_representation(int base, int count, int* finite_count, ...) {
    va_list args;
    va_start(args, finite_count);
    double* finite_numbers = (double*)malloc(count * sizeof(double));
    int index = 0;
    for(int i = 0; i < count; i++){
        double x = va_arg(args, double);
        if(check_finite(x, base)){
            finite_numbers[index] = x;
            index++;
        }
    }
    va_end(args);
    *finite_count = index;
    return finite_numbers;
}

int main() {
    int finite_count = 0;
    int base = 2;
    double* res = has_finite_representation(base, 2, &finite_count, 0.625, 0.1);
    for(int i = 0; i < finite_count; i++){
        printf("Number %lf is finite in base %d\n", res[i], base);
    }
    return 0;
}
