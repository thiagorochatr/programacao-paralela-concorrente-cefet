#include <stdio.h>
#include <math.h>
#include <omp.h>
#include "libppc.h"

double trunc_func(double x) {
    return (double)((int)x);
}

double f(double x) {
    int trunc_x = (int)trunc_func(x);
    double sum = 0.0;
    for (int y = 1; y <= trunc_x; y++) {
        sum += pow(x, y) / tgamma(y + 1);
    }
    return sum;
}

double g(double x) {
    return (1.0 / 3.0) * x * x + 4 * x + 2;
}

double h(double x) {
    return sin(M_PI * x) * cos(M_PI * x);
}

double trapezoidal_rule(double a, double b, int n) {
    double step = (b - a) / n;
    double integral = 0.0;
    
    for (int i = 0; i < n; i++) {
        double x1 = a + i * step;
        double x2 = a + (i + 1) * step;
        integral += 0.5 * step * (f(x1) * g(h(x1)) + f(x2) * g(h(x2)));
    }
    
    return integral;
}

int main() {
    double a = 1.0, b = 20.0;
    int n;
    printf("Digite o numero de trapezios: ");
    scanf("%d", &n);
    if (n <= 0) {
        printf("O numero de trapezios deve ser maior que zero.\n");
        return 1;
    }
    
    double start_time = omp_get_wtime();
    double result = trapezoidal_rule(a, b, n);
    double end_time = omp_get_wtime();
    
    printf("Valor aproximado da integral: %.6f\n", result);
    printf("Tempo de execucao: %f segundos\n", end_time - start_time);
    
    return 0;
}