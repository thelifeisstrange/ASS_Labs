#include <stdio.h>

int main(void)
{
    int ref[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    int f[3] = {-1, -1, -1}, rbit[3] = {0}, clock = 0, used = 0, fault = 0, hit = 0;

    for (int k = 0; k < 12; k++) {
        int p = ref[k], found = -1;
        for (int i = 0; i < used; i++)
            if (f[i] == p)
                found = i;
        if (found >= 0) {
            rbit[found] = 1;
            hit++;
        } else {
            fault++;
            if (used < 3) {
                f[used] = p;
                rbit[used++] = 1;
            } else {
                while (rbit[clock]) {
                    rbit[clock] = 0;
                    clock = (clock + 1) % 3;
                }
                f[clock] = p;
                rbit[clock] = 1;
                clock = (clock + 1) % 3;
            }
        }
        printf("%d : ", p);
        for (int i = 0; i < 3; i++)
            printf(i < used ? "%d(%d) " : "- ", f[i], rbit[i]);
        printf("%s\n", found >= 0 ? "hit" : "fault");
    }
    printf("faults=%d hits=%d ratio=%.2f\n", fault, hit, hit / 12.0);
    return 0;
}
