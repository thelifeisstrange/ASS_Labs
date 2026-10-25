#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t pool; /* 3 identical resources */

void *work(void *a)
{
    int id = *(int *)a;
    sem_wait(&pool);
    printf("T%d got resource\n", id);
    sleep(1);
    sem_post(&pool);
    printf("T%d released\n", id);
    return a;
}

int main(void)
{
    pthread_t t[5];
    int id[5];
    sem_init(&pool, 0, 3);
    for (int i = 0; i < 5; i++) {
        id[i] = i + 1;
        pthread_create(&t[i], NULL, work, &id[i]);
    }
    for (int i = 0; i < 5; i++)
        pthread_join(t[i], NULL);
    return 0;
}
