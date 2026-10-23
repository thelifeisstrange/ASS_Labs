#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_AHEAD 10
#define ITEMS     25

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

static int buffer[MAX_AHEAD];
static int in, out, produced, consumed;
static sema_t empty, full, mutex;

void *producer(void *arg)
{
    (void)arg;
    for (int i = 1; i <= ITEMS; i++) {
        sema_wait(&empty);
        sema_wait(&mutex);
        buffer[in] = i;
        printf("produced %2d  (in buffer: %d)\n", i,
               (produced + 1) - consumed);
        in = (in + 1) % MAX_AHEAD;
        produced++;
        sema_post(&mutex);
        sema_post(&full);
        usleep(40000);
    }
    return NULL;
}

void *consumer(void *arg)
{
    (void)arg;
    for (int i = 1; i <= ITEMS; i++) {
        sema_wait(&full);
        sema_wait(&mutex);
        int item = buffer[out];
        out = (out + 1) % MAX_AHEAD;
        consumed++;
        printf("consumed %2d  (in buffer: %d)\n", item, produced - consumed);
        sema_post(&mutex);
        sema_post(&empty);
        usleep(80000);
    }
    return NULL;
}

int main(void)
{
    pthread_t tp, tc;

    /* empty=10: producer may get at most 10 items ahead of consumer */
    sema_init(&empty, MAX_AHEAD);
    sema_init(&full, 0);
    sema_init(&mutex, 1);

    printf("bounded buffer size %d, produce %d items\n", MAX_AHEAD, ITEMS);
    pthread_create(&tp, NULL, producer, NULL);
    pthread_create(&tc, NULL, consumer, NULL);
    pthread_join(tp, NULL);
    pthread_join(tc, NULL);
    return 0;
}
