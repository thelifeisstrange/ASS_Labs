#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct mab {
    int offset;
    int size;
    int allocated;
    struct mab *next;
    struct mab *prev;
} Mab, *MabPtr;

enum { FIRST, NEXT, BEST, WORST };

static MabPtr rover;

MabPtr memInit(int size)
{
    MabPtr m = malloc(sizeof(Mab));
    m->offset = 0;
    m->size = size;
    m->allocated = 0;
    m->next = m->prev = NULL;
    rover = m;
    return m;
}

static void print_map(MabPtr m)
{
    printf("  memory: ");
    for (; m; m = m->next)
        printf("[%d+%d %s] ", m->offset, m->size, m->allocated ? "used" : "free");
    printf("\n");
}

MabPtr memSplit(MabPtr m, int size)
{
    if (!m || m->allocated || m->size < size)
        return NULL;
    if (m->size == size)
        return m;

    MabPtr rest = malloc(sizeof(Mab));
    rest->offset = m->offset + size;
    rest->size = m->size - size;
    rest->allocated = 0;
    rest->next = m->next;
    rest->prev = m;
    if (m->next)
        m->next->prev = rest;
    m->next = rest;
    m->size = size;
    return m;
}

MabPtr memMerge(MabPtr m)
{
    if (!m || m->allocated)
        return m;
    while (m->next && !m->next->allocated) {
        MabPtr n = m->next;
        m->size += n->size;
        m->next = n->next;
        if (n->next)
            n->next->prev = m;
        if (rover == n)
            rover = m;
        free(n);
    }
    while (m->prev && !m->prev->allocated) {
        MabPtr p = m->prev;
        p->size += m->size;
        p->next = m->next;
        if (m->next)
            m->next->prev = p;
        if (rover == m)
            rover = p;
        free(m);
        m = p;
    }
    return m;
}

MabPtr memFree(MabPtr m)
{
    if (!m)
        return NULL;
    m->allocated = 0;
    return memMerge(m);
}

static MabPtr find_first(MabPtr start, int size)
{
    for (MabPtr p = start; p; p = p->next)
        if (!p->allocated && p->size >= size)
            return p;
    return NULL;
}

static MabPtr find_next(MabPtr head, int size)
{
    MabPtr p = rover ? rover : head;
    MabPtr start = p;
    do {
        if (!p->allocated && p->size >= size)
            return p;
        p = p->next ? p->next : head;
    } while (p != start);
    return NULL;
}

static MabPtr find_best(MabPtr start, int size)
{
    MabPtr best = NULL;
    for (MabPtr p = start; p; p = p->next)
        if (!p->allocated && p->size >= size)
            if (!best || p->size < best->size)
                best = p;
    return best;
}

static MabPtr find_worst(MabPtr start, int size)
{
    MabPtr worst = NULL;
    for (MabPtr p = start; p; p = p->next)
        if (!p->allocated && p->size >= size)
            if (!worst || p->size > worst->size)
                worst = p;
    return worst;
}

MabPtr memChk(MabPtr m, int size, int policy)
{
    switch (policy) {
    case NEXT:  return find_next(m, size);
    case BEST:  return find_best(m, size);
    case WORST: return find_worst(m, size);
    default:    return find_first(m, size);
    }
}

MabPtr memAlloc(MabPtr head, int size, int policy)
{
    MabPtr hole = memChk(head, size, policy);
    if (!hole)
        return NULL;
    hole = memSplit(hole, size);
    hole->allocated = 1;
    rover = hole->next ? hole->next : head;
    return hole;
}

static int policy_of(const char *s)
{
    if (!strcmp(s, "next"))
        return NEXT;
    if (!strcmp(s, "best"))
        return BEST;
    if (!strcmp(s, "worst"))
        return WORST;
    return FIRST;
}

int main(int argc, char *argv[])
{
    int policy = policy_of(argc > 1 ? argv[1] : "first");
    const char *name[] = { "first-fit", "next-fit", "best-fit", "worst-fit" };
    int req[] = { 100, 50, 200, 80, 30 };
    MabPtr blocks[5] = { 0 };
    MabPtr heap = memInit(500);

    printf("Policy: %s  (heap size 500)\n", name[policy]);
    print_map(heap);

    for (int i = 0; i < 5; i++) {
        blocks[i] = memAlloc(heap, req[i], policy);
        printf("alloc %d -> offset %d\n", req[i],
               blocks[i] ? blocks[i]->offset : -1);
        print_map(heap);
    }

    printf("free block of size %d at %d\n", req[1], blocks[1]->offset);
    memFree(blocks[1]);
    print_map(heap);

    printf("free block of size %d at %d\n", req[3], blocks[3]->offset);
    memFree(blocks[3]);
    print_map(heap);

    MabPtr extra = memAlloc(heap, 70, policy);
    printf("alloc 70 -> offset %d\n", extra ? extra->offset : -1);
    print_map(heap);

    while (heap) {
        MabPtr n = heap->next;
        free(heap);
        heap = n;
    }
    return 0;
}
