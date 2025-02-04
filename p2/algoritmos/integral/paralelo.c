#include <stdio.h>
#include <math.h>
#include <omp.h>
#include "libppc.h"

/**
 * Truncates a floating point number to its integer part
 * @param x Number to truncate
 * @return Truncated value as double
 */
double trunc_func(double x) {
    return (double)((int)x);
}

/**
 * Calculates a series sum based on powers and gamma function
 * @param x Input value
 * @return Sum of the series
 */
double f(double x) {
    int trunc_x = (int)trunc_func(x);
    double sum = 0.0;
    for (int y = 1; y <= trunc_x; y++) {
        sum += pow(x, y) / tgamma(y + 1);
    }
    return sum;
}

/**
 * Calculates a quadratic function
 * @param x Input value
 * @return Result of quadratic function
 */
double g(double x) {
    return (1.0 / 3.0) * x * x + 4 * x + 2;
}

/**
 * Calculates product of sine and cosine
 * @param x Input value in radians
 * @return sin(πx) * cos(πx)
 */
double h(double x) {
    return sin(M_PI * x) * cos(M_PI * x);
}

/**
 * Implements a parallel version of the trapezoidal rule for numerical integration
 * Uses OpenMP to parallelize the computation across trapezoids
 * @param a Lower bound of integration
 * @param b Upper bound of integration
 * @param n Number of trapezoids
 * @return Approximate value of the integral
 */
double trapezoidal_rule(double a, double b, int n) {
    double step = (b - a) / n;
    double integral = 0.0;
    
    #pragma omp parallel for reduction(+:integral)
    for (int i = 0; i < n; i++) {
        double x1 = a + i * step;
        double x2 = a + (i + 1) * step;
        integral += 0.5 * step * (f(x1) * g(h(x1)) + f(x2) * g(h(x2)));
    }
    
    return integral;
}

/**
 * Main function that demonstrates the parallel implementation of trapezoidal rule
 * Gets number of trapezoids from user, calculates integral and measures execution time
 * Uses OpenMP for parallel execution
 */
int main() {
    double a = 1.0, b = 20.0;
    int n;
    printf("Digite o numero de trapezios: ");
    scanf("%d", &n);
    if (n <= 0) {
        printf("O numero de trapezios deve ser maior que zero.\n");
        return 1;
    }
    
    double result;
    
    #pragma omp parallel
    {
        #pragma omp single
        printf("Executando com %d threads\n", omp_get_num_threads());
    }
    
    double start_time = omp_get_wtime();    
    result = trapezoidal_rule(a, b, n);
    double end_time = omp_get_wtime();
    
    printf("Valor aproximado da integral: %.6f\n", result);
    printf("Tempo de execucao: %f segundos\n", end_time - start_time);
    
    return 0;
}