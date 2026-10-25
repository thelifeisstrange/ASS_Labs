#include <stdio.h>

int sz[20] = {1024}, fr[20] = {1}, n = 1;

void show(void)
{
    int off = 0;
    for (int i = 0; i < n; i++) {
        printf("[%d+%d %s] ", off, sz[i], fr[i] ? "free" : "used");
        off += sz[i];
    }
    printf("\n");
}

int alloc(int need)
{
    int s = 16;
    while (s < need)
        s *= 2;
    for (int i = 0; i < n; i++) {
        if (!fr[i] || sz[i] < s)
            continue;
        while (sz[i] > s) {
            for (int j = n; j > i + 1; j--) {
                sz[j] = sz[j - 1];
                fr[j] = fr[j - 1];
            }
            n++;
            sz[i] /= 2;
            sz[i + 1] = sz[i];
            fr[i + 1] = 1;
        }
        fr[i] = 0;
        return i;
    }
    return -1;
}

void rel(int idx)
{
    fr[idx] = 1;
    int m = 1;
    while (m) {
        m = 0;
        int off = 0;
        for (int i = 0; i < n - 1; i++) {
            if (fr[i] && fr[i + 1] && sz[i] == sz[i + 1] && (off & sz[i]) == 0) {
                sz[i] *= 2;
                for (int j = i + 1; j < n - 1; j++) {
                    sz[j] = sz[j + 1];
                    fr[j] = fr[j + 1];
                }
                n--;
                m = 1;
                break;
            }
            off += sz[i];
        }
    }
}

int main(void)
{
    printf("init: ");
    show();
    int a = alloc(70), b = alloc(35), c = alloc(120);
    printf("alloc 70,35,120: ");
    show();
    rel(c);
    rel(b);
    rel(a);
    printf("free all: ");
    show();
    return 0;
}
