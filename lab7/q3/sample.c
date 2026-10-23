#include <stdio.h>

#define NSEG 5

typedef struct {
    int base;
    int limit;
} Seg;

static Seg table[NSEG];

void make_table(void)
{
    /* Bases and limits from the physical-memory figure. */
    table[0].base = 14000;
    table[0].limit = 12000; /* 14000-26000 */
    table[1].base = 63000;
    table[1].limit = 4000;  /* 63000-67000 */
    table[2].base = 43000;
    table[2].limit = 4000;  /* 43000-47000 */
    table[3].base = 32000;
    table[3].limit = 11000; /* 32000-43000 */
    table[4].base = 47000;
    table[4].limit = 10000; /* 47000-57000 */
}

int to_physical(int seg, int offset, int *phys)
{
    if (seg < 0 || seg >= NSEG)
        return 0;
    if (offset < 0 || offset >= table[seg].limit)
        return 0;
    *phys = table[seg].base + offset;
    return 1;
}

int main(void)
{
    int phys;
    int query_seg[] = { 2, 3, 0 };
    int query_off[] = { 53, 852, 1222 };

    make_table();

    printf("Segment table\n");
    printf("seg  base    limit\n");
    for (int i = 0; i < NSEG; i++)
        printf("%3d  %5d  %5d\n", i, table[i].base, table[i].limit);

    printf("\nLogical -> physical\n");
    for (int i = 0; i < 3; i++) {
        int s = query_seg[i], o = query_off[i];
        if (to_physical(s, o, &phys))
            printf("(%d, %d) -> %d\n", s, o, phys);
        else
            printf("(%d, %d) -> segmentation fault\n", s, o);
    }
    return 0;
}
