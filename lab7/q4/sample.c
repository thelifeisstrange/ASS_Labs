#include <stdio.h>

#define FRAMES 3
#define REFS   12

int main(void)
{
    int ref[] = { 1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5 };
    int frame[FRAMES];
    int refbit[FRAMES];
    int used = 0, clock = 0, faults = 0, hits = 0;

    for (int i = 0; i < FRAMES; i++) {
        frame[i] = -1;
        refbit[i] = 0;
    }

    printf("Second-chance (LRU approximation), %d frames\n", FRAMES);
    printf("ref  frames        result\n");

    for (int r = 0; r < REFS; r++) {
        int page = ref[r], hit = -1;
        for (int i = 0; i < used; i++)
            if (frame[i] == page)
                hit = i;

        if (hit >= 0) {
            refbit[hit] = 1;
            hits++;
        } else {
            faults++;
            if (used < FRAMES) {
                frame[used] = page;
                refbit[used] = 1;
                used++;
            } else {
                while (refbit[clock]) {
                    refbit[clock] = 0;
                    clock = (clock + 1) % FRAMES;
                }
                frame[clock] = page;
                refbit[clock] = 1;
                clock = (clock + 1) % FRAMES;
            }
        }

        printf("%3d  ", page);
        for (int i = 0; i < FRAMES; i++)
            if (i < used)
                printf("%d(%d) ", frame[i], refbit[i]);
            else
                printf("-    ");
        printf(" %s\n", hit >= 0 ? "hit" : "fault");
    }

    printf("page faults = %d\n", faults);
    printf("hits        = %d\n", hits);
    printf("hit ratio   = %.2f\n", (double)hits / REFS);
    return 0;
}
