#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define SIZE 6
#define NUM_THREADS 3

typedef struct
{
  int *vetor;
  int inicio;
  int fim;
  int soma_parcial;
} ThreadData;

void *soma_parcial(void *arg)
{
  ThreadData *data = (ThreadData *)arg;
  data->soma_parcial = 0;

  for (int i = data->inicio; i < data->fim; i++)
  {
    data->soma_parcial += data->vetor[i];
  }

  pthread_exit((void *)&(data->soma_parcial));
}

int main()
{
  pthread_t threads[NUM_THREADS];
  int vetor[SIZE] = {1, 2, 3, 4, 5, 6};
  ThreadData threadData[NUM_THREADS];
  int soma_total = 0;
  int elementos_por_thread = SIZE / NUM_THREADS;
  int resto = SIZE % NUM_THREADS;

  int inicio_atual = 0;
  for (int i = 0; i < NUM_THREADS; i++)
  {
    threadData[i].vetor = vetor;
    threadData[i].inicio = inicio_atual;
    threadData[i].fim = inicio_atual + elementos_por_thread + (i < resto ? 1 : 0);
    inicio_atual = threadData[i].fim;

    if (pthread_create(&threads[i], NULL, soma_parcial, (void *)&threadData[i]))
    {
      fprintf(stderr, "Erro ao criar thread %d\n", i);
      return 1;
    }
  }

  for (int i = 0; i < NUM_THREADS; i++)
  {
    int *soma_parcial;
    pthread_join(threads[i], (void **)&soma_parcial);
    soma_total += *soma_parcial;
  }

  printf("A soma total dos elementos do vetor é: %d\n", soma_total);

  return 0;
}
