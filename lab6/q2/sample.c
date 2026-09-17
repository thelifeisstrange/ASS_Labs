#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

int n;

void *sum_upto(void *arg)
{
    (void)arg;
    long sum = 0;
    for (int i = 0; i <= n; i++)
        sum += i;
    return (void *)(intptr_t)sum;
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    void *result;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    if (n < 0) {
        fprintf(stderr, "n must be non-negative\n");
        return 1;
    }

    pthread_create(&tid, NULL, sum_upto, NULL);
    pthread_join(tid, &result);

    printf("Sum of non-negative integers 0..%d = %ld\n", n, (long)(intptr_t)result);
    return 0;
}
