#include <stdio.h>
#include <math.h>

void calculate_speedup_and_efficiency(int n_values[], int n_size, int p_values[], int p_size) {
    for (int i = 0; i < n_size; i++) {
        for (int j = 0; j < p_size; j++) {
            int n = n_values[i];
            int p = p_values[j];

            double T_serial = pow(n, 2);
            double T_parallel = (pow(n, 2) / p) + log2(p);

            double speedup = T_serial / T_parallel;
            double efficiency = speedup / p;

            printf("%5d %5d %12.2f %15.2f %10.2f %12.2f\n", n, p, T_serial, T_parallel, speedup, efficiency);
        }
    }
}

int main() {
    int n_values[] = {10, 20, 40, 80, 160, 320};
    int p_values[] = {1,2,4,8,16,32,64,128};

    int n_size = sizeof(n_values) / sizeof(n_values[0]);
    int p_size = sizeof(p_values) / sizeof(p_values[0]);

    calculate_speedup_and_efficiency(n_values, n_size, p_values, p_size);

    return 0;
}