#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define ARRAY_SIZE 100

int main()
{
    int array[ARRAY_SIZE]; // Array compartilhado entre o pai e o filho
    int sum = 0;           // Variável para somar os elementos do array
    pid_t pid;

    // Inicializa o array com zeros
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        array[i] = 0;
    }

    // Cria o processo filho
    pid = fork();

    if (pid < 0)
    {
        // Se houver um erro no fork
        perror("Erro no fork");
        exit(1);
    }

    if (pid == 0)
    {
        // Processo filho
        printf("[FILHO] Alterando o vetor...\n");

        // O filho modifica o array, definindo todos os elementos como 1
        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            array[i] = 1;
        }

        // Soma no processo filho (antes do Copy on Write ser acionado no pai)
        sum = 0;
        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            sum += array[i];
        }
        printf("[FILHO] Soma dos elementos do vetor: %d\n", sum);

        exit(0); // O processo filho termina aqui
    }
    else
    {
        // Processo pai
        // O pai calcula a soma dos elementos do array antes de qualquer alteração (deve ser 0)
        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            sum += array[i];
        }
        printf("[PAI] Soma dos elementos do vetor: %d\n", sum);

        wait(NULL); // Espera o processo filho terminar
    }

    return 0;
}
