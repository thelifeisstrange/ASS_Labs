#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int buf[5], in, out;
sem_t mutex, full, empty, ahead; /* ahead: max 10 unconsumed */

void *produce(void *a)
{
    for (int i = 0; i < 12; i++) {
        sem_wait(&ahead);
        sem_wait(&empty);
        sem_wait(&mutex);
        buf[in] = i;
        in = (in + 1) % 5;
        printf("produced %d\n", i);
        sem_post(&mutex);
        sem_post(&full);
        sleep(1);
    }
    return a;
}

void *consume(void *a)
{
    int x;
    for (int i = 0; i < 12; i++) {
        sem_wait(&full);
        sem_wait(&mutex);
        x = buf[out];
        out = (out + 1) % 5;
        printf("consumed %d\n", x);
        sem_post(&mutex);
        sem_post(&empty);
        sem_post(&ahead);
        sleep(2);
    }
    return a;
}

int main(void)
{
    pthread_t p, c;
    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, 5);
    sem_init(&ahead, 0, 10);
    pthread_create(&p, NULL, produce, NULL);
    pthread_create(&c, NULL, consume, NULL);
    pthread_join(p, NULL);
    pthread_join(c, NULL);
    return 0;
}
