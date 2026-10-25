#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int data, rcount;
sem_t mutex, wrt;

void *reader(void *a)
{
    int id = *(int *)a;
    sem_wait(&mutex);
    if (++rcount == 1)
        sem_wait(&wrt);
    sem_post(&mutex);
    printf("R%d reads %d\n", id, data);
    sleep(1);
    sem_wait(&mutex);
    if (--rcount == 0)
        sem_post(&wrt);
    sem_post(&mutex);
    return a;
}

void *writer(void *a)
{
    int id = *(int *)a;
    sem_wait(&wrt);
    data += 10;
    printf("W%d writes %d\n", id, data);
    sleep(1);
    sem_post(&wrt);
    return a;
}

int main(void)
{
    pthread_t r[2], w[1];
    int rid[] = {1, 2}, wid = 1;
    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);
    pthread_create(&r[0], NULL, reader, &rid[0]);
    pthread_create(&r[1], NULL, reader, &rid[1]);
    pthread_create(&w[0], NULL, writer, &wid);
    pthread_join(r[0], NULL);
    pthread_join(r[1], NULL);
    pthread_join(w[0], NULL);
    return 0;
}
