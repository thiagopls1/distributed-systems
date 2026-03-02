#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

const unsigned int THREADS_COUNT = 2;

typedef struct {

} workerParams;

void *pairWorker(void *arg) {
  // workerParams *params = (workerParams *)arg;

  return NULL;
}

int main() {
  pthread_mutex_t mutex;

  pthread_cond_t man_arrived;
  pthread_cond_t woman_arrived;

  // INIT
  pthread_mutex_init(&mutex, NULL);

  pthread_cond_init(&man_arrived, NULL);
  pthread_cond_init(&woman_arrived, NULL);

  // Threads Creation
  pthread_t threads[THREADS_COUNT];

  for (int i = 0; i < THREADS_COUNT; i++)
    pthread_create(&threads[i], NULL, pairWorker, NULL);

  for (int i = 0; i < THREADS_COUNT; i++)
    pthread_join(threads[i], NULL);

  // DESTROY
  pthread_mutex_destroy(&mutex);

  pthread_cond_destroy(&man_arrived);
  pthread_cond_destroy(&woman_arrived);

  return 0;
}
