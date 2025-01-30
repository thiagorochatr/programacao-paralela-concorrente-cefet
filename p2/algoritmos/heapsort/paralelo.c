#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "libppc.h"

void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        heapify(arr, n, largest);
    }
}

void my_heapsort(int arr[], int n) {
    #pragma omp parallel for
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        heapify(arr, i, 0);
    }
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int n;
    printf("Digite a quantidade de numeros: ");
    scanf("%d", &n);
    
    if (n <= 0) {
        printf("Quantidade de numeros deve ser maior que zero.\n");
        return 1;
    }

    int *arr = generate_random_int_vector(n, 0, 1000000);

    printf("Array original:\n");
    printArray(arr, n);

    double start_time = omp_get_wtime();
    my_heapsort(arr, n);
    double end_time = omp_get_wtime();

    printf("Array ordenado:\n");
    printArray(arr, n);

    printf("\nTempo de execucao: %f segundos\n", end_time - start_time);

    return 0;
}