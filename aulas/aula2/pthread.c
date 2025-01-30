#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4

// Struct para memória compartilhada entre as threads
typedef struct
{
  int shared_value; // Valor compartilhado que todas as threads podem acessar/modificar
} shared_mem_t;

// Struct para os dados privados de cada thread
typedef struct
{
  int rank;                 // Identificador único da thread
  shared_mem_t *shared_mem; // Ponteiro para a memória compartilhada
} private_data_t;

// Função que cada thread irá executar
void *thread_function(void *arg)
{
  // Faz o cast do argumento para a struct private_data_t
  private_data_t *private_data = (private_data_t *)arg;

  // Acessa o rank e a memória compartilhada
  int rank = private_data->rank;
  shared_mem_t *shared_mem = private_data->shared_mem;

  // Incrementa o valor compartilhado com base no rank da thread
  printf("Thread %d está incrementando o valor compartilhado.\n", rank);
  shared_mem->shared_value += rank;

  // Exibe o valor atualizado
  printf("Thread %d: valor compartilhado atualizado para %d\n", rank, shared_mem->shared_value);

  // Termina a thread
  pthread_exit(NULL);
}

int main()
{
  // Aloca a memória compartilhada
  shared_mem_t *shared_mem = (shared_mem_t *)malloc(sizeof(shared_mem_t));
  shared_mem->shared_value = 0; // Inicializa o valor compartilhado

  // Array para armazenar as threads e seus dados privados
  pthread_t threads[NUM_THREADS];
  private_data_t thread_data[NUM_THREADS];

  // Cria as threads
  for (int i = 0; i < NUM_THREADS; i++)
  {
    // Preenche os dados privados da thread
    thread_data[i].rank = i;                // O rank é o índice da thread
    thread_data[i].shared_mem = shared_mem; // Aponta para a memória compartilhada

    // Cria a thread
    if (pthread_create(&threads[i], NULL, thread_function, (void *)&thread_data[i]))
    {
      fprintf(stderr, "Erro ao criar a thread %d\n", i);
      exit(1);
    }
  }

  // Espera que todas as threads terminem
  for (int i = 0; i < NUM_THREADS; i++)
  {
    pthread_join(threads[i], NULL);
  }

  // Exibe o valor final da memória compartilhada
  printf("Valor final compartilhado: %d\n", shared_mem->shared_value);

  // Libera a memória alocada
  free(shared_mem);

  return 0;
}
