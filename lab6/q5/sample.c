#include <stdio.h>
#include <pthread.h>

#define N 3

int A[N][N] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};
int B[N][N] = {
    {9, 8, 7},
    {6, 5, 4},
    {3, 2, 1}
};
int C[N][N];

void *mul_row(void *arg)
{
    int row = *(int *)arg;
    for (int j = 0; j < N; j++) {
        C[row][j] = 0;
        for (int k = 0; k < N; k++)
            C[row][j] += A[row][k] * B[k][j];
    }
    return NULL;
}

void print_matrix(const char *name, int m[N][N])
{
    printf("%s =\n", name);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%4d", m[i][j]);
        printf("\n");
    }
}

int main(void)
{
    pthread_t tid[N];
    int rows[N];

    print_matrix("A", A);
    print_matrix("B", B);

    for (int i = 0; i < N; i++) {
        rows[i] = i;
        pthread_create(&tid[i], NULL, mul_row, &rows[i]);
    }
    for (int i = 0; i < N; i++)
        pthread_join(tid[i], NULL);

    print_matrix("C = A * B", C);
    return 0;
}
