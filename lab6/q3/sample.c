#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 1000

int primes[MAX];
int count;
int start, end;

int is_prime(int x)
{
    if (x < 2)
        return 0;
    for (int i = 2; i * i <= x; i++)
        if (x % i == 0)
            return 0;
    return 1;
}

void *generate(void *arg)
{
    (void)arg;
    count = 0;
    for (int i = start; i <= end && count < MAX; i++)
        if (is_prime(i))
            primes[count++] = i;
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <start> <end>\n", argv[0]);
        return 1;
    }

    start = atoi(argv[1]);
    end = atoi(argv[2]);
    if (start > end) {
        fprintf(stderr, "start must be <= end\n");
        return 1;
    }

    pthread_create(&tid, NULL, generate, NULL);
    pthread_join(tid, NULL);

    printf("Primes from %d to %d:\n", start, end);
    for (int i = 0; i < count; i++)
        printf("%d ", primes[i]);
    printf("\n");
    return 0;
}
