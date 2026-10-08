#include <stdio.h>
#include <pthread.h>

#define R 3
#define C 4

int M[R][C] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12}
};
int row_sum[R];
int col_sum[C];

void *rows(void *arg)
{
    (void)arg;
    for (int i = 0; i < R; i++) {
        row_sum[i] = 0;
        for (int j = 0; j < C; j++)
            row_sum[i] += M[i][j];
    }
    return NULL;
}

void *cols(void *arg)
{
    (void)arg;
    for (int j = 0; j < C; j++) {
        col_sum[j] = 0;
        for (int i = 0; i < R; i++)
            col_sum[j] += M[i][j];
    }
    return NULL;
}

int main(void)
{
    pthread_t tr, tc;

    printf("Matrix:\n");
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++)
            printf("%4d", M[i][j]);
        printf("\n");
    }

    pthread_create(&tr, NULL, rows, NULL);
    pthread_create(&tc, NULL, cols, NULL);
    pthread_join(tr, NULL);
    pthread_join(tc, NULL);

    printf("Row sums: ");
    for (int i = 0; i < R; i++)
        printf("%d ", row_sum[i]);
    printf("\nColumn sums: ");
    for (int j = 0; j < C; j++)
        printf("%d ", col_sum[j]);
    printf("\n");
    return 0;
}
