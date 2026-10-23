#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define SLOTS 3
#define USERS 6

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

static int sema_value(sema_t *s)
{
    pthread_mutex_lock(&s->lock);
    int v = s->count;
    pthread_mutex_unlock(&s->lock);
    return v;
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

static sema_t seats;

void *user(void *arg)
{
    int id = *(int *)arg;
    printf("user %d waits  (available=%d)\n", id, sema_value(&seats));
    sema_wait(&seats);
    printf("user %d sits   (available=%d)\n", id, sema_value(&seats));
    usleep(150000);
    sema_post(&seats);
    printf("user %d leaves (available=%d)\n", id, sema_value(&seats));
    return NULL;
}

int main(void)
{
    pthread_t tid[USERS];
    int id[USERS];

    /* counting semaphore: 3 identical seats; up to 3 users at once */
    sema_init(&seats, SLOTS);
    printf("counting semaphore initial value = %d\n", SLOTS);

    for (int i = 0; i < USERS; i++) {
        id[i] = i + 1;
        pthread_create(&tid[i], NULL, user, &id[i]);
        usleep(20000);
    }
    for (int i = 0; i < USERS; i++)
        pthread_join(tid[i], NULL);
    printf("final available = %d\n", sema_value(&seats));
    return 0;
}
