#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "libppc.h"

void matrixMultiply(double *A, double *B, double *C, int rowsA, int colsA, int colsB) {
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            M(i, j, colsB, C) = 0;
            for (int k = 0; k < colsA; k++) {
                M(i, j, colsB, C) += M(i, k, colsA, A) * M(k, j, colsB, B);
            }
        }
    }
}

int main() {
    long int rowsA, colsA, rowsB, colsB;

    printf("Digite o número de linhas e colunas para a matriz A: ");
    scanf("%ld %ld", &rowsA, &colsA);
    printf("Digite o número de linhas e colunas para a matriz B: ");
    scanf("%ld %ld", &rowsB, &colsB);

    if (colsA != rowsB) {
        printf("Erro: O número de colunas de A deve ser igual ao número de linhas de B.\n");
        return -1;
    }

    double *A = generate_random_double_matrix(rowsA, colsA);
    double *B = generate_random_double_matrix(rowsB, colsB);
    double *C = (double*)malloc(sizeof(double) * rowsA * colsB);

    printf("Matriz A:\n");
    print_double_matrix(A, rowsA, colsA);

    printf("\nMatriz B:\n");
    print_double_matrix(B, rowsB, colsB);

    double start_time = omp_get_wtime();
    matrixMultiply(A, B, C, rowsA, colsA, colsB);
    double end_time = omp_get_wtime();

    printf("\nResultado da multiplicação (Matriz C):\n");
    print_double_matrix(C, rowsA, colsB);

    free(A);
    free(B);
    free(C);

    printf("\nTempo de execução (Serial): %f segundos\n", end_time - start_time);

    return 0;
}

// Comando para compilar:
// Serial:
// gcc-13 -I. serial.c libpcc.c -o serial -fopenmp -lm
// ./serial

// Paralelo:
// gcc-13 -I. paralelo.c libpcc.c -o paralelo -fopenmp -lm
// ./paralelo