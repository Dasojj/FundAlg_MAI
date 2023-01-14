#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

double geometric_mean(int num_args, ...) {
    double x, product = 1;
    va_list args;
    va_start(args, num_args);
    for (int i = 0; i < num_args; i++) {
        x = va_arg(args, double);
        if(x < 0){
            return -1;
            break;
        }
        product *= x;
    }
    va_end(args);
    return pow(product, 1.0 / num_args);
}

double Power(double base, double power){
    return exp(power * log(base));
}

int main(){
    int choice;
    printf("Choose function:\n1)geometric_mean(num_args, ...);\n2)power(base, exponent).\n");
    scanf("%d", &choice);
    if(choice == 1){
        double res = geometric_mean(3, 7.0, 15.0, 3.2);
        printf("Geometric mean of numbers 7, 15, 3.2 is: %lf \n", res);
        res = geometric_mean(3, 23.5, 11.0, 6.0);
        printf("Geometric mean of numbers 23.5, 11, 6 is: %lf \n", res);
        res = geometric_mean(3, 3.3, 3.4, 3.2);
        printf("Geometric mean of numbers 3.3, 3.4, 3.2 is: %lf \n", res);
    }
    else if(choice == 2){
        double base;
        int exponent;
        printf("Enter the base number: ");
        scanf("%lf", &base);
        printf("Enter the exponent: ");
        scanf("%d", &exponent);
        double result = Power(base, exponent);
        printf("%lf\n", result);
    }
    else{
        printf("You can only choose either 1 or 2.\n");
    }
    return 0;
}
