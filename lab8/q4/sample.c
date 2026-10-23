#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define CHAIRS     3
#define CUSTOMERS  8

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

static int waiting;
static int served;
static int leftover = CUSTOMERS;
static sema_t mutex, customers, barber;

static void customer_done(void)
{
    leftover--;
    if (leftover == 0)
        sema_post(&customers);
}

void *barber_fn(void *arg)
{
    (void)arg;
    for (;;) {
        printf("barber: sleeping\n");
        sema_wait(&customers);
        sema_wait(&mutex);
        if (waiting == 0) {
            sema_post(&mutex);
            break;
        }
        waiting--;
        sema_post(&barber);
        sema_post(&mutex);
        printf("barber: cutting hair (waiting chairs: %d)\n", waiting);
        usleep(120000);
        served++;
    }
    return NULL;
}

void *customer_fn(void *arg)
{
    int id = *(int *)arg;
    usleep(id * 40000);
    sema_wait(&mutex);
    if (waiting < CHAIRS) {
        waiting++;
        printf("customer %d: sits (waiting chairs: %d)\n", id, waiting);
        sema_post(&customers);
        sema_post(&mutex);
        sema_wait(&barber);
        printf("customer %d: getting a haircut\n", id);
        sema_wait(&mutex);
        customer_done();
        sema_post(&mutex);
    } else {
        printf("customer %d: shop full, leaves\n", id);
        customer_done();
        sema_post(&mutex);
    }
    return NULL;
}

int main(void)
{
    pthread_t tb, tc[CUSTOMERS];
    int id[CUSTOMERS];

    sema_init(&mutex, 1);
    sema_init(&customers, 0);
    sema_init(&barber, 0);

    printf("sleeping barber, %d waiting chairs, %d customers\n", CHAIRS, CUSTOMERS);
    pthread_create(&tb, NULL, barber_fn, NULL);
    for (int i = 0; i < CUSTOMERS; i++) {
        id[i] = i + 1;
        pthread_create(&tc[i], NULL, customer_fn, &id[i]);
    }
    for (int i = 0; i < CUSTOMERS; i++)
        pthread_join(tc[i], NULL);
    pthread_join(tb, NULL);
    printf("shop closed, served %d\n", served);
    return 0;
}
