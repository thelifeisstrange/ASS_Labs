#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t s1, s2;

void *t1(void *a)
{
    sem_wait(&s1);
    sleep(1);
    sem_wait(&s2);
    return a;
}

void *t2(void *a)
{
    sem_wait(&s2);
    sleep(1);
    sem_wait(&s1);
    return a;
}

int main(void)
{
    sem_init(&s1, 0, 1);
    sem_init(&s2, 0, 1);
    pthread_t a, b;
    pthread_create(&a, NULL, t1, NULL);
    pthread_create(&b, NULL, t2, NULL);
    sleep(3);
    puts("deadlock: t1 holds s1 waits s2, t2 holds s2 waits s1");
    return 0;
}
