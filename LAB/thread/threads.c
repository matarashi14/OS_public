#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5
#define ARRAY_SIZE 20

int global_array[ARRAY_SIZE] = {0};
pthread_mutex_t mutex;

void *thread_func(void *threadid)
{
    int tid = (intptr_t)threadid;

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        pthread_mutex_lock(&mutex);
        if (global_array[i] == 0)
        {
            global_array[i] = tid;
            pthread_mutex_unlock(&mutex);
            printf("global_array[%d] = %d\n", i, tid);
            sleep(rand() % 10);
        }
        else
        {
            pthread_mutex_unlock(&mutex);
        }
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[NUM_THREADS];
    long i;

    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < NUM_THREADS; ++i)
    {
        pthread_create(&threads[i], NULL, thread_func, (void *)(i + 1));
    }

    for (i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}