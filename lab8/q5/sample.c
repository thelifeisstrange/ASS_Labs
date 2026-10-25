#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 5
int state[N];
pthread_mutex_t mon = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ok[N];

void test(int i)
{
    if (state[i] == 1 && state[(i + 4) % N] != 2 && state[(i + 1) % N] != 2) {
        state[i] = 2;
        pthread_cond_signal(&ok[i]);
    }
}

void pickup(int i)
{
    pthread_mutex_lock(&mon);
    state[i] = 1;
    test(i);
    while (state[i] != 2)
        pthread_cond_wait(&ok[i], &mon);
    printf("P%d eats\n", i);
    pthread_mutex_unlock(&mon);
}

void putdown(int i)
{
    pthread_mutex_lock(&mon);
    state[i] = 0;
    test((i + 4) % N);
    test((i + 1) % N);
    pthread_mutex_unlock(&mon);
}

void *phil(void *a)
{
    int i = *(int *)a;
    pickup(i);
    sleep(1);
    putdown(i);
    return a;
}

int main(void)
{
    pthread_t t[N];
    int id[N];
    for (int i = 0; i < N; i++) {
        pthread_cond_init(&ok[i], NULL);
        id[i] = i;
        pthread_create(&t[i], NULL, phil, &id[i]);
    }
    for (int i = 0; i < N; i++)
        pthread_join(t[i], NULL);
    return 0;
}
