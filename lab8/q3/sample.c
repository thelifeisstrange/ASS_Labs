#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int count;
} sema_t;

static void sema_init(sema_t *s, int v)
{
    pthread_mutex_init(&s->lock, NULL);
    pthread_cond_init(&s->cond, NULL);
    s->count = v;
}

static void sema_wait(sema_t *s)
{
    pthread_mutex_lock(&s->lock);
    while (s->count == 0)
        pthread_cond_wait(&s->cond, &s->lock);
    s->count--;
    pthread_mutex_unlock(&s->lock);
}

static void sema_post(sema_t *s)
{
    pthread_mutex_lock(&s->lock);
    s->count++;
    pthread_cond_signal(&s->cond);
    pthread_mutex_unlock(&s->lock);
}

static sema_t s1, s2;

void *thread_a(void *arg)
{
    (void)arg;
    printf("A: wait s1\n");
    sema_wait(&s1);
    printf("A: got s1, now wait s2\n");
    usleep(100000);
    sema_wait(&s2);
    printf("A: got both (should not print)\n");
    sema_post(&s2);
    sema_post(&s1);
    return NULL;
}

void *thread_b(void *arg)
{
    (void)arg;
    printf("B: wait s2\n");
    sema_wait(&s2);
    printf("B: got s2, now wait s1\n");
    usleep(100000);
    sema_wait(&s1);
    printf("B: got both (should not print)\n");
    sema_post(&s1);
    sema_post(&s2);
    return NULL;
}

static void on_alarm(int sig)
{
    (void)sig;
    printf("deadlock: A holds s1 and waits for s2; B holds s2 and waits for s1\n");
    fflush(stdout);
    _exit(0);
}

int main(void)
{
    pthread_t ta, tb;

    setvbuf(stdout, NULL, _IOLBF, 0);
    sema_init(&s1, 1);
    sema_init(&s2, 1);

    signal(SIGALRM, on_alarm);
    alarm(2);

    printf("two threads lock two semaphores in opposite order\n");
    pthread_create(&ta, NULL, thread_a, NULL);
    pthread_create(&tb, NULL, thread_b, NULL);
    pthread_join(ta, NULL);
    pthread_join(tb, NULL);
    return 0;
}
