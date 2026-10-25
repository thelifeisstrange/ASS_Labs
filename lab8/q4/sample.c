#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define CHAIRS 3
int waiting;
sem_t mutex, cust, barb;

void *barber(void *a)
{
    for (;;) {
        sem_wait(&cust);
        sem_wait(&mutex);
        waiting--;
        sem_post(&barb);
        sem_post(&mutex);
        printf("barber cuts (waiting=%d)\n", waiting);
        sleep(1);
    }
    return a;
}

void *customer(void *a)
{
    int id = *(int *)a;
    sem_wait(&mutex);
    if (waiting < CHAIRS) {
        waiting++;
        printf("cust %d sits (waiting=%d)\n", id, waiting);
        sem_post(&cust);
        sem_post(&mutex);
        sem_wait(&barb);
        printf("cust %d done\n", id);
    } else {
        printf("cust %d leaves\n", id);
        sem_post(&mutex);
    }
    return a;
}

int main(void)
{
    pthread_t b, t[5];
    int id[5];
    sem_init(&mutex, 0, 1);
    sem_init(&cust, 0, 0);
    sem_init(&barb, 0, 0);
    pthread_create(&b, NULL, barber, NULL);
    for (int i = 0; i < 5; i++) {
        id[i] = i + 1;
        pthread_create(&t[i], NULL, customer, &id[i]);
        usleep(300000);
    }
    for (int i = 0; i < 5; i++)
        pthread_join(t[i], NULL);
    return 0;
}
