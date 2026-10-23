#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Q0_QUANTUM 4
#define Q1_QUANTUM 8
#define NQ 3

typedef struct proc {
    char name[8];
    int burst;
    int remaining;
    int arrival;
    int start;
    int finish;
    int qlevel;
    struct proc *next;
} Proc;

typedef struct {
    Proc *head;
    Proc *tail;
} Queue;

static Queue q[NQ];

static void enqueue(int level, Proc *p)
{
    p->qlevel = level;
    p->next = NULL;
    if (!q[level].head)
        q[level].head = q[level].tail = p;
    else {
        q[level].tail->next = p;
        q[level].tail = p;
    }
}

static Proc *dequeue(int level)
{
    Proc *p = q[level].head;
    if (!p)
        return NULL;
    q[level].head = p->next;
    if (!q[level].head)
        q[level].tail = NULL;
    p->next = NULL;
    return p;
}

static int empty(void)
{
    return !q[0].head && !q[1].head && !q[2].head;
}

static Proc *make_proc(const char *name, int burst, int arrival)
{
    Proc *p = malloc(sizeof(Proc));
    snprintf(p->name, sizeof(p->name), "%s", name);
    p->burst = p->remaining = burst;
    p->arrival = arrival;
    p->start = p->finish = -1;
    p->qlevel = 0;
    p->next = NULL;
    return p;
}

int main(void)
{
    /* Q0: RR q=4, Q1: RR q=8, Q2: FCFS. New jobs enter Q0. */
    Proc *jobs[4];
    jobs[0] = make_proc("P1", 16, 0);
    jobs[1] = make_proc("P2", 5, 0);
    jobs[2] = make_proc("P3", 10, 2);
    jobs[3] = make_proc("P4", 7, 4);

    int n = 4, admitted = 0, time = 0;

    printf("MLFQ: Q0 RR(q=%d), Q1 RR(q=%d), Q2 FCFS\n", Q0_QUANTUM, Q1_QUANTUM);
    printf("time  process  ran  remaining  next-queue\n");

    while (admitted < n || !empty()) {
        while (admitted < n && jobs[admitted]->arrival <= time) {
            enqueue(0, jobs[admitted]);
            admitted++;
        }
        if (empty()) {
            time++;
            continue;
        }

        int level = q[0].head ? 0 : (q[1].head ? 1 : 2);
        Proc *p = dequeue(level);
        int slice = (level == 0) ? Q0_QUANTUM : (level == 1) ? Q1_QUANTUM : p->remaining;
        if (slice > p->remaining)
            slice = p->remaining;

        if (p->start < 0)
            p->start = time;
        p->remaining -= slice;
        time += slice;

        while (admitted < n && jobs[admitted]->arrival <= time) {
            enqueue(0, jobs[admitted]);
            admitted++;
        }

        if (p->remaining == 0) {
            p->finish = time;
            printf("%4d  %-7s  %3d  %9d  done\n", time - slice, p->name, slice, 0);
        } else if (level < 2) {
            enqueue(level + 1, p);
            printf("%4d  %-7s  %3d  %9d  Q%d\n",
                   time - slice, p->name, slice, p->remaining, level + 1);
        } else {
            enqueue(2, p);
            printf("%4d  %-7s  %3d  %9d  Q2\n",
                   time - slice, p->name, slice, p->remaining);
        }
    }

    printf("\nname  burst  turnaround  waiting\n");
    for (int i = 0; i < n; i++) {
        int tat = jobs[i]->finish - jobs[i]->arrival;
        printf("%-4s  %5d  %10d  %7d\n",
               jobs[i]->name, jobs[i]->burst, tat, tat - jobs[i]->burst);
        free(jobs[i]);
    }
    return 0;
}
