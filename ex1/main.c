#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

const unsigned int THREAD_COUNT = 4;
const unsigned size_t ARRAY_SIZE = 1000000000;
const size_t CHUNK_SIZE = ARRAY_SIZE / THREAD_COUNT;
const size_t CHUNK_MOD = ARRAY_SIZE % THREAD_COUNT;
const int MAX_RAND = 100;

typedef struct {
  int *array;
  size_t *result;
  int thread_id;
  size_t *chunk_size;
  size_t *chunk_mod;
} workerParams;

void *worker(void *arg) {
  workerParams *params = (workerParams *)arg;
  size_t sum = 0;
  size_t start_index = params->thread_id * (*params->chunk_size);
  size_t end_index = start_index + *params->chunk_size;

  if (params->thread_id == THREAD_COUNT - 1) {
    end_index = start_index + *params->chunk_size + *params->chunk_mod;
  }

  printf("Thread %d:\n", params->thread_id);
  printf("\tstart_index: %zu\n", start_index);
  printf("\tend_index: %zu\n", end_index);

  for (size_t i = start_index; i < end_index; i++) {
    sum += params->array[i];
  }

  *params->result += sum;
  return NULL;
}

int *gen_huge_array(size_t size) {
  srand(time(NULL));
  int *ptr = malloc(size * sizeof(int));

  for (size_t i = 0; i < size; i++) {
    ptr[i] = (rand() % MAX_RAND);
  }

  srand(time(NULL));
  return ptr;
}

int main() {
  // Precisamos de um array de parâmetros, para
  // evitar que valor não será substituído no for
  pthread_t threads[THREAD_COUNT];
  workerParams params[THREAD_COUNT];

  size_t chunk_size = ARRAY_SIZE / THREAD_COUNT;
  size_t chunk_mod = ARRAY_SIZE % THREAD_COUNT;

  size_t sum_seq = 0;
  size_t sum_threads = 0;

  // Geração de arrays
  printf("Gerando array de %zu elementos...\n", ARRAY_SIZE);
  int *gen_array = gen_huge_array(ARRAY_SIZE);
  printf("Feito!\n");

  // Criação de threads

  clock_t start_t = clock();
  printf("Criando threads...\n");
  printf("\n");
  for (int i = 0; i < THREAD_COUNT; i++) {
    params[i].array = gen_array;
    params[i].result = &sum_threads;
    params[i].thread_id = i;
    params[i].chunk_size = &chunk_size;
    params[i].chunk_mod = &chunk_mod;

    pthread_create(&threads[i], NULL, worker, &params[i]);
  }

  for (int i = 0; i < THREAD_COUNT; i++) {
    pthread_join(threads[i], NULL);
  }
  clock_t end_t = clock();
  float time_elapsed_t = (float)(end_t - start_t) / CLOCKS_PER_SEC;

  // Soma sequencial
  clock_t start_s = clock();
  for (int i = 0; i < ARRAY_SIZE; i++) {
    sum_seq += gen_array[i];
  }
  clock_t end_s = clock();
  float time_elapsed_s = (float)(end_s - start_s) / CLOCKS_PER_SEC;

  printf("\n");
  printf("Soma total (seq.): %zu [%.2f s]\n", sum_seq, time_elapsed_s);
  printf("Soma total (threads): %zu [%.2f s]\n", sum_threads, time_elapsed_t);

  return 0;
}
