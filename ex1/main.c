#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

u_int THREAD_COUNT = 1;
size_t ARRAY_SIZE = 10;

typedef struct {
  int *array;
  size_t *result;
  size_t start_index;
  size_t end_index;
} workerParams;

void *worker(void *arg) {
  workerParams *params = (workerParams *)arg;
  size_t sum = 0;

  for (int i = params->start_index; i < params->end_index; i++) {
    sum += params->array[i];
  }

  *params->result += sum;
  return NULL;
}

int *gen_huge_array(size_t size) {
  int *ptr = malloc(size * sizeof(int));

  for (size_t i = 0; i < size; i++) {
    ptr[i] = (rand() % 100);
  }

  srand(time(NULL));
  return ptr;
}

int main() {
  pthread_t threads[THREAD_COUNT];

  size_t sum_seq = 0;
  size_t sum_threads = 0;

  printf("Gerando array de %zu elementos...\n", ARRAY_SIZE);
  int *gen_array = gen_huge_array(ARRAY_SIZE);
  printf("Feito!\n");

  printf("Criando threads...\n");
  for (int i = 0; i < THREAD_COUNT; i++) {
    workerParams params;
    params.array = gen_array;
    params.result = &sum_threads;
    params.start_index = 0;
    params.end_index = 10;

    pthread_create(&threads[i], NULL, worker, &params);
  }
  printf("Feito!\n");

  for (int i = 0; i < THREAD_COUNT; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Soma total (threads): %zu", sum_threads);

  return 0;
}
