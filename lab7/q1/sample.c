#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct mab {
    int offset, size, allocated;
    struct mab *next, *prev;
} Mab, *MabPtr;

MabPtr rover;

void show(MabPtr m)
{
    for (; m; m = m->next)
        printf("[%d+%d %s] ", m->offset, m->size, m->allocated ? "used" : "free");
    printf("\n");
}

MabPtr memSplit(MabPtr m, int size)
{
    if (m->size == size)
        return m;
    MabPtr r = malloc(sizeof(Mab));
    r->offset = m->offset + size;
    r->size = m->size - size;
    r->allocated = 0;
    r->next = m->next;
    r->prev = m;
    if (m->next)
        m->next->prev = r;
    m->next = r;
    m->size = size;
    return m;
}

MabPtr memMerge(MabPtr m)
{
    while (m->next && !m->next->allocated) {
        MabPtr n = m->next;
        m->size += n->size;
        m->next = n->next;
        if (n->next)
            n->next->prev = m;
        free(n);
    }
    if (m->prev && !m->prev->allocated)
        return memMerge(m->prev);
    return m;
}

MabPtr memFree(MabPtr m)
{
    m->allocated = 0;
    return memMerge(m);
}

/* policy: 0 first, 1 next, 2 best, 3 worst */
MabPtr memChk(MabPtr m, int size, int pol)
{
    MabPtr p = (pol == 1 && rover) ? rover : m, best = NULL, start = p;
    do {
        if (!p->allocated && p->size >= size) {
            if (pol == 0 || pol == 1)
                return p;
            if (pol == 2 && (!best || p->size < best->size))
                best = p;
            if (pol == 3 && (!best || p->size > best->size))
                best = p;
        }
        p = p->next ? p->next : m;
        if (pol != 1)
            if (!p || p == start)
                break;
    } while (p != start);
    return best;
}

MabPtr memAlloc(MabPtr head, int size, int pol)
{
    MabPtr h = memChk(head, size, pol);
    if (!h)
        return NULL;
    h = memSplit(h, size);
    h->allocated = 1;
    rover = h->next ? h->next : head;
    return h;
}

int main(int argc, char **argv)
{
    int pol = 0, req[] = {100, 50, 200, 80, 30};
    MabPtr b[5], heap = malloc(sizeof(Mab));
    char *n[] = {"first", "next", "best", "worst"};

    if (argc > 1) {
        if (!strcmp(argv[1], "next"))
            pol = 1;
        else if (!strcmp(argv[1], "best"))
            pol = 2;
        else if (!strcmp(argv[1], "worst"))
            pol = 3;
    }
    heap->offset = heap->allocated = 0;
    heap->size = 500;
    heap->next = heap->prev = NULL;
    rover = heap;
    printf("%s-fit\n", n[pol]);
    show(heap);
    for (int i = 0; i < 5; i++) {
        b[i] = memAlloc(heap, req[i], pol);
        printf("alloc %d @ %d\n", req[i], b[i]->offset);
        show(heap);
    }
    memFree(b[1]);
    memFree(b[3]);
    printf("after free 50 and 80:\n");
    show(heap);
    printf("alloc 70 @ %d\n", memAlloc(heap, 70, pol)->offset);
    show(heap);
    return 0;
}
