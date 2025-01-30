#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 26

void *imprime_letra(void *arg)
{
  char letra = *((char *)arg);
  printf("%c ", letra);
  pthread_exit(NULL);
}

int main()
{
  pthread_t threads[NUM_THREADS];
  char alfabeto[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++)
  {
    alfabeto[i] = 'A' + i;
  }

  for (int i = 0; i < NUM_THREADS; i++)
  {
    if (pthread_create(&threads[i], NULL, imprime_letra, (void *)&alfabeto[i]))
    {
      fprintf(stderr, "Erro ao criar thread %d\n", i);
      return 1;
    }
  }

  for (int i = 0; i < NUM_THREADS; i++)
  {
    pthread_join(threads[i], NULL);
  }

  printf("\nTodas as threads terminaram.\n");

  return 0;
}
