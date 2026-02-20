#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *worker(void *arg) {
  size_t value = (size_t)arg;
  printf("Created a new thread (%d)\n", value);
  sleep(value);
  printf("Done (%d).\n", value);
  return NULL;
}

int *gen_huge_array(size_t size) {
  int *ptr = malloc(size * sizeof(int));

  for (size_t i = 0; i < size; i++) {
    ptr[i] = (rand() % 10);
  }

  srand(time(NULL));
  return ptr;
}

int main() {
  // pthread_t thread1;
  // pthread_t thread2;

  size_t size = 1000000000;
  int *gen_array = gen_huge_array(size);

  printf("Array elements:\n");
  for (size_t i = 0; i < size; i++) {
    printf("%d", gen_array[i]);
  }

  // pthread_create(&thread2, NULL, worker, &data2);
  // pthread_create(&thread1, NULL, worker, &data1);

  // pthread_join(thread1, NULL);
  // pthread_join(thread2, NULL);

  return 0;
}
