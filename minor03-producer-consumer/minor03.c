#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NITEMS 10

char shared_buffer[NITEMS];
int shared_count = 0;

pthread_mutex_t mutex;
pthread_cond_t cond_full;
pthread_cond_t cond_empty;

unsigned int prod_index = 0;
unsigned int cons_index = 0;

void *producer(void *arg);
void *consumer(void *arg);

int main(void)
{
    pthread_t prod_tid, cons_tid1, cons_tid2;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_full, NULL);
    pthread_cond_init(&cond_empty, NULL);

    pthread_create(&prod_tid, NULL, producer, NULL);
    pthread_create(&cons_tid1, NULL, consumer, NULL);
    pthread_create(&cons_tid2, NULL, consumer, NULL);

    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid1, NULL);
    pthread_join(cons_tid2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_full);
    pthread_cond_destroy(&cond_empty);

    return 0;
}

void *producer(void *arg)
{
    char key;

    printf("Enter text for producer to read and consumer to print, use Ctrl-C to exit.\n\n");

    while (1)
    {
        if (scanf("%c", &key) != 1)
            continue;

        pthread_mutex_lock(&mutex);

        while (shared_count == NITEMS)
            pthread_cond_wait(&cond_full, &mutex);

        shared_buffer[prod_index] = key;
        shared_count++;

        if (prod_index == NITEMS - 1)
            prod_index = 0;
        else
            prod_index++;

        pthread_cond_signal(&cond_empty);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void *consumer(void *arg)
{
    char key;
    long unsigned int id = (long unsigned int)pthread_self();

    while (1)
    {
        pthread_mutex_lock(&mutex);

        while (shared_count == 0)
            pthread_cond_wait(&cond_empty, &mutex);

        key = shared_buffer[cons_index];
        shared_count--;

        if (cons_index == NITEMS - 1)
            cons_index = 0;
        else
            cons_index++;

        pthread_cond_signal(&cond_full);
        pthread_mutex_unlock(&mutex);

        printf("consumer %lu: %c\n", id, key);
    }

    return NULL;
}
