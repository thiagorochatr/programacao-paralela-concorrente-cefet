#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>
#include "libppc.h"

void odd_even_sort(int *arr, int n) {
    bool sorted = false;
    while (!sorted) {
        sorted = true;
        
        // Fase Ímpar
        for (int i = 1; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                sorted = false;
            }
        }
        
        // Fase Par
        for (int i = 0; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                sorted = false;
            }
        }
    }
}

int main() {
    int n;
    printf("Digite a quantidade de números: ");
    scanf("%d", &n);
    
    if (n <= 0) {
        printf("Quantidade de números deve ser maior que zero.\n");
        return 1;
    }
    
    int *arr = generate_random_int_vector(n, 0, 1000000);
    
    printf("Antes da ordenação:\n");
    print_int_vector(arr, 10, 10);
    
    double start_time = omp_get_wtime();
    odd_even_sort(arr, n);
    double end_time = omp_get_wtime();
    
    printf("\nApós a ordenação:\n");
    print_int_vector(arr, 10, 10);
    
    printf("\nTempo de execução (Serial): %f segundos\n", end_time - start_time);
    
    save_int_vector(arr, n, "sorted_serial.dat");
    
    free(arr);
    return 0;
}