#include <stdio.h>

int base[5] = {14000, 63000, 43000, 32000, 47000};
int limit[5] = {12000, 4000, 4000, 11000, 10000};

int phys(int s, int off)
{
    if (s < 0 || s > 4 || off < 0 || off >= limit[s])
        return -1;
    return base[s] + off;
}

int main(void)
{
    int s[] = {2, 3, 0}, o[] = {53, 852, 1222};
    printf("seg base  limit\n");
    for (int i = 0; i < 5; i++)
        printf("%d   %d  %d\n", i, base[i], limit[i]);
    for (int i = 0; i < 3; i++)
        printf("(%d, %d) -> %d\n", s[i], o[i], phys(s[i], o[i]));
    return 0;
}
