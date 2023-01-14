#include <stdio.h>
#include <math.h>

double bisection(double (*f)(double), double a, double b, double eps) {
    double c = (a + b) / 2;
    while (fabs(b - a) > eps) {
        if (f(c) == 0)
            return c;
        else if (f(a) * f(c) < 0)
            b = c;
        else
            a = c;
        c = (a + b) / 2;
    }
    return c;
}


double equation(double x) {
    return x*x*x - x*x + 2;
}

int main() {
    double a = -10.0, b = 10.0, eps = 0.0001;
    double root = bisection(equation, a, b, eps);
    printf("Root of the equation is: %lf\n", root);
    return 0;
}
