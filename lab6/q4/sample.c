#include <stdio.h>
#include <pthread.h>

int a[] = {12, 7, 9, 4, 18, 5, 3, 8};
int n = 8;
long even_sum, odd_sum;

void *sum_even(void *arg)
{
    (void)arg;
    even_sum = 0;
    for (int i = 0; i < n; i++)
        if (a[i] % 2 == 0)
            even_sum += a[i];
    return NULL;
}

void *sum_odd(void *arg)
{
    (void)arg;
    odd_sum = 0;
    for (int i = 0; i < n; i++)
        if (a[i] % 2 != 0)
            odd_sum += a[i];
    return NULL;
}

int main(void)
{
    pthread_t te, to;

    printf("Array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");

    pthread_create(&te, NULL, sum_even, NULL);
    pthread_create(&to, NULL, sum_odd, NULL);
    pthread_join(te, NULL);
    pthread_join(to, NULL);

    printf("Even sum = %ld\n", even_sum);
    printf("Odd sum  = %ld\n", odd_sum);
    return 0;
}
