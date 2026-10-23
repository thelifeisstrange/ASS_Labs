#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 5
#define MEALS 2

enum { THINKING, HUNGRY, EATING };

/* Monitor: one mutex plus a condition variable per philosopher. */
static pthread_mutex_t mon = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t self[N];
static int state[N];

static void test(int i)
{
    int left = (i + N - 1) % N;
    int right = (i + 1) % N;
    if (state[i] == HUNGRY &&
        state[left] != EATING &&
        state[right] != EATING) {
        state[i] = EATING;
        pthread_cond_signal(&self[i]);
    }
}

static void pickup(int i)
{
    pthread_mutex_lock(&mon);
    state[i] = HUNGRY;
    printf("P%d hungry\n", i);
    test(i);
    while (state[i] != EATING)
        pthread_cond_wait(&self[i], &mon);
    printf("P%d eating (forks %d and %d)\n", i, i, (i + 1) % N);
    pthread_mutex_unlock(&mon);
}

static void putdown(int i)
{
    pthread_mutex_lock(&mon);
    state[i] = THINKING;
    printf("P%d thinking\n", i);
    test((i + N - 1) % N);
    test((i + 1) % N);
    pthread_mutex_unlock(&mon);
}

void *philosopher(void *arg)
{
    int i = *(int *)arg;
    for (int m = 0; m < MEALS; m++) {
        usleep(50000);
        pickup(i);
        usleep(80000);
        putdown(i);
    }
    return NULL;
}

int main(void)
{
    pthread_t tid[N];
    int id[N];

    for (int i = 0; i < N; i++) {
        pthread_cond_init(&self[i], NULL);
        state[i] = THINKING;
        id[i] = i;
    }

    printf("dining philosophers (monitor: mutex + condition variables)\n");
    for (int i = 0; i < N; i++)
        pthread_create(&tid[i], NULL, philosopher, &id[i]);
    for (int i = 0; i < N; i++)
        pthread_join(tid[i], NULL);
    return 0;
}
