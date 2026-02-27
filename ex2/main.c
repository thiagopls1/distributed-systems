#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define BUFFER_SIZE 5
#define MAX_RAND 100

typedef struct {
    int queue[BUFFER_SIZE];
    int in;
    int out;
    int count;
} queueBuffer;

typedef struct {
    queueBuffer* q_buffer;
    pthread_mutex_t* mutex;
    pthread_cond_t* not_full;
    pthread_cond_t* not_empty;
} threadParams;


void *producerWorker(void *arg) {
    threadParams *params = (threadParams *)arg;
    queueBuffer *q_buffer = params->q_buffer;
    while (true) {
        int item = rand() % MAX_RAND;

        pthread_mutex_lock(params->mutex);

        while (q_buffer->count == BUFFER_SIZE)
            pthread_cond_wait(params->not_full, params->mutex);
        
        q_buffer->queue[q_buffer->in] = item;
        printf("Produced: %d at %d\n", item, q_buffer->in);
        q_buffer->in = (q_buffer->in + 1) % BUFFER_SIZE;
        q_buffer->count++;

        pthread_cond_signal(params->not_empty);
        pthread_mutex_unlock(params->mutex);

        sleep(1);
    }
    return NULL;
}

void *consumerWorker(void *arg) {
    threadParams *params = (threadParams *)arg;
    queueBuffer *q_buffer = params->q_buffer;

    while (true) {
        pthread_mutex_lock(params->mutex);

        while (q_buffer->count == 0)
            pthread_cond_wait(params->not_empty, params->mutex);

        int item = q_buffer->queue[q_buffer->out];
        printf("Consumed: %d at %d\n", item, q_buffer->out);
        q_buffer->out = (q_buffer->out + 1) % BUFFER_SIZE;
        q_buffer->count--;

        pthread_cond_signal(params->not_full);
        pthread_mutex_unlock(params->mutex);

        sleep(1);
    }
    return NULL;
}

int main() {
    queueBuffer q_buffer;
    q_buffer.count = 0;
    q_buffer.in = 0;
    q_buffer.out = 0;

    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

    pthread_t consumer, producer;

    threadParams workerParams = {
        .q_buffer = &q_buffer,
        .mutex = &mutex,
        .not_full = &not_full,
        .not_empty = &not_empty
    };

    pthread_create(&consumer, NULL, consumerWorker, &workerParams);
    pthread_create(&producer, NULL, producerWorker, &workerParams);

    pthread_join(consumer, NULL);
    pthread_join(producer, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    return 0;
}