#include <stdio.h>
#include <stdarg.h>

typedef struct {
    double x;
    double y;
} Point;

int is_convex(int n, ...) {
    va_list points;
    va_start(points, n);

    Point p1 = va_arg(points, Point);
    Point p2 = va_arg(points, Point);
    Point p3 = va_arg(points, Point);

    double cross_product = (p2.x - p1.x) * (p3.y - p2.y) - (p3.x - p2.x) * (p2.y - p1.y);
    int is_cw = cross_product < 0;

    for (int i = 3; i < n; i++) {
        Point p4 = va_arg(points, Point);
        double new_cross_product = (p3.x - p2.x) * (p4.y - p3.y) - (p4.x - p3.x) * (p3.y - p2.y);
        int new_is_cw = new_cross_product < 0;
        if ((is_cw && !new_is_cw)||(!is_cw && new_is_cw)) {
            va_end(points);
            return 0;
        }
        p2 = p3;
        p3 = p4;
    }

    va_end(points);
    return 1;
}

double evaluate_polynomial(double x, int n, ...) {
    va_list coefficients;
    va_start(coefficients, n);
    double result = va_arg(coefficients, double);
    for (int i = 0; i < n; i++) {
        result = result*x + va_arg(coefficients, double);
    }
    va_end(coefficients);
    return result;
}

int main(void) {
    Point a = {3, 4};
    Point b = {5, 11};
    Point c = {12, 8};
    Point d = {9, 5};
    Point e = {6, 3};
    if(is_convex(5, a, b, c, d, e)) printf("The polynom is convex.\n");
    else printf("The polynom is not convex.\n");

    double x = 2;
    double a3 = 2;
    double a2 = -3;
    double a1 = 5;
    double a0 = 1;
    double result = evaluate_polynomial(x, 3, a3, a2, a1, a0);
    printf("The value of the polynomial at x = %f is %f\n", x, result);
    return 0;
}
