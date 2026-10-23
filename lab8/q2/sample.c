#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NREAD  4
#define NWRITE 2

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

static int shared = 0;
static int readcount;
static sema_t mutex, wrt;

void *reader(void *arg)
{
    int id = *(int *)arg;
    for (int i = 0; i < 3; i++) {
        sema_wait(&mutex);
        readcount++;
        if (readcount == 1)
            sema_wait(&wrt);
        sema_post(&mutex);

        printf("reader %d  reads %d  (readers=%d)\n", id, shared, readcount);
        usleep(50000);

        sema_wait(&mutex);
        readcount--;
        if (readcount == 0)
            sema_post(&wrt);
        sema_post(&mutex);
        usleep(30000);
    }
    return NULL;
}

void *writer(void *arg)
{
    int id = *(int *)arg;
    for (int i = 0; i < 3; i++) {
        sema_wait(&wrt);
        shared += 10;
        printf("writer %d  writes %d\n", id, shared);
        usleep(70000);
        sema_post(&wrt);
        usleep(40000);
    }
    return NULL;
}

int main(void)
{
    pthread_t r[NREAD], w[NWRITE];
    int rid[NREAD], wid[NWRITE];

    sema_init(&mutex, 1);
    sema_init(&wrt, 1);

    printf("first readers-writers: readers share; writers exclusive\n");
    for (int i = 0; i < NREAD; i++) {
        rid[i] = i + 1;
        pthread_create(&r[i], NULL, reader, &rid[i]);
    }
    for (int i = 0; i < NWRITE; i++) {
        wid[i] = i + 1;
        pthread_create(&w[i], NULL, writer, &wid[i]);
    }
    for (int i = 0; i < NREAD; i++)
        pthread_join(r[i], NULL);
    for (int i = 0; i < NWRITE; i++)
        pthread_join(w[i], NULL);
    printf("final shared value = %d\n", shared);
    return 0;
}
