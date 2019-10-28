
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

pthread_mutex_t mutex = -1;

void* thread_fun(void *arg)
{
    pthread_t pid = pthread_self();

    if(-1 == mutex)
    {
        pthread_mutex_init(&mutex, NULL);
    }

    pthread_mutex_lock(&mutex);

    printf("thread: %d, do something......\r\n", pid);

    pthread_mutex_unlock(&mutex);
}

