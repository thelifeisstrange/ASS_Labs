#include <stdio.h>

/* Q0: RR quantum 4, Q1: RR quantum 8, Q2: FCFS */
int q[3][10], n[3];
int burst[] = {16, 5, 10, 7}, rem[4], nproc = 4;

void enq(int l, int p) { q[l][n[l]++] = p; }

int deq(int l)
{
    int p = q[l][0];
    for (int i = 0; i < n[l] - 1; i++)
        q[l][i] = q[l][i + 1];
    n[l]--;
    return p;
}

int main(void)
{
    int t = 0, i, p, slice, qt[] = {4, 8, 99};
    for (i = 0; i < nproc; i++) {
        rem[i] = burst[i];
        enq(0, i);
    }
    printf("MLFQ Q0=RR4 Q1=RR8 Q2=FCFS\n");
    while (n[0] + n[1] + n[2]) {
        int l = n[0] ? 0 : n[1] ? 1 : 2;
        p = deq(l);
        slice = rem[p] < qt[l] ? rem[p] : qt[l];
        rem[p] -= slice;
        printf("t=%d P%d ran %d rem %d\n", t, p + 1, slice, rem[p]);
        t += slice;
        if (rem[p])
            enq(l < 2 ? l + 1 : 2, p);
        else
            printf("P%d done at %d\n", p + 1, t);
    }
    return 0;
}
