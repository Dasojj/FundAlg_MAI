#include <stdio.h>
#include <stdarg.h>
#include <math.h>

double convert_to_base(double decimal, int to_base) {
    double old = decimal;
    int i;
    double result = 0;
    for (i = 1; i <= 10; i++) {
        double x = decimal * to_base;
        int integer_part = (int) x;
        result = result + integer_part * pow(0.1, i);
        decimal = x - integer_part;
    }
    return result;
}

double convert_to_decimal(int fraction, int base) {
    while(fraction % 10 == 0) fraction /= 10;
    double result = 0;
    int i = -1;
    int remainder = 0;
    while (fraction > 0) {
        remainder = fraction % 10;
        fraction = fraction / 10;
        result += remainder * pow(base, i);
        i--;
    }
    return result;
}

void print_has_finite_representation(int base, int count, ...) {
    va_list args;
    va_start(args, count);
    double decimal;
    for (int i = 0; i < count; i++) {
        decimal = va_arg(args, double);
        double decimalInbase = convert_to_base(decimal, base);
        double decimalAfterbase = convert_to_decimal((int)round(decimalInbase * 1e6), base);
        if (decimal != decimalAfterbase){
            printf("Fraction %lf does not have a finite representation in base %d\n", decimal, base);
        } else {
            printf("Fraction %lf has a finite representation in base %d\n", decimal, base);
        }
    }
    va_end(args);
}

int main() {
    print_has_finite_representation(2, 2, 0.625, 0.1);
    return 0;
}
