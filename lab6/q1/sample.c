#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 100

int fib[MAX];
int n;

void *generate(void *arg)
{
    (void)arg;
    if (n >= 1)
        fib[0] = 0;
    if (n >= 2)
        fib[1] = 1;
    for (int i = 2; i < n; i++)
        fib[i] = fib[i - 1] + fib[i - 2];
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <count>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    if (n < 1 || n > MAX) {
        fprintf(stderr, "count must be between 1 and %d\n", MAX);
        return 1;
    }

    pthread_create(&tid, NULL, generate, NULL);
    pthread_join(tid, NULL);

    printf("Fibonacci sequence (%d terms):\n", n);
    for (int i = 0; i < n; i++)
        printf("%d ", fib[i]);
    printf("\n");
    return 0;
}
