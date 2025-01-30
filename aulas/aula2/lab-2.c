#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define SIZE 6
#define NUM_THREADS 2

typedef struct
{
  int *vetor1;
  int *vetor2;
  int inicio;
  int fim;
} ThreadData;

void *soma_vetores(void *arg)
{
  ThreadData *data = (ThreadData *)arg;

  for (int i = data->inicio; i < data->fim; i++)
  {
    data->vetor1[i] += data->vetor2[i];
  }

  pthread_exit(NULL);
}

void printa_vetor(int *vetor, int tamanho)
{
  for (int i = 0; i < tamanho; i++)
  {
    printf("%d ", vetor[i]);
  }
  printf("\n");
}

int main()
{
  pthread_t threads[NUM_THREADS];

  static int vetor1[SIZE] = {1, 2, 3, 4, 5, 6};
  static int vetor2[SIZE] = {2, 3, 4, 5, 6, 7};

  ThreadData threadData[NUM_THREADS];

  printf("Vetor 1 antes da soma\n");
  printa_vetor(vetor1, SIZE);
  printf("Vetor 2\n");
  printa_vetor(vetor2, SIZE);

  int elementos_por_thread = SIZE / NUM_THREADS;
  int resto = SIZE % NUM_THREADS;

  int inicio_atual = 0;
  for (int i = 0; i < NUM_THREADS; i++)
  {
    threadData[i].vetor1 = vetor1;
    threadData[i].vetor2 = vetor2;
    threadData[i].inicio = inicio_atual;
    threadData[i].fim = inicio_atual + elementos_por_thread + (i < resto ? 1 : 0);

    inicio_atual = threadData[i].fim;

    if (pthread_create(&threads[i], NULL, soma_vetores, (void *)&threadData[i]))
    {
      fprintf(stderr, "Erro ao criar thread %d\n", i);
      return 1;
    }
  }

  for (int i = 0; i < NUM_THREADS; i++)
  {
    pthread_join(threads[i], NULL);
  }

  printf("Vetor 1 após a soma\n");
  printa_vetor(vetor1, SIZE);

  return 0;
}