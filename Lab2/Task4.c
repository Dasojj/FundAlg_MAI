#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

double Power(double base, int power){
    if(power == 0) return 1;
    else if(power < 0) return Power(1/base, -power);
    return power == 1 ? base : base * Power(base, power - 1);
}

int main(){
    double num;
    int p;
    scanf("%lf", &num);
    scanf("%d", &p);
    printf("%lf", Power(num, p));
    printf("\n");
    return 0;
}
