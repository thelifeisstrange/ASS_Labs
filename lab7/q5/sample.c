#include <stdio.h>
#include <stdlib.h>

#define HEAP 1024
#define MIN  16

typedef struct block {
    int offset;
    int size;
    int free;
    struct block *next;
} Block;

static Block *head;

static Block *make(int offset, int size, int free)
{
    Block *b = malloc(sizeof(Block));
    b->offset = offset;
    b->size = size;
    b->free = free;
    b->next = NULL;
    return b;
}

static void show(const char *msg)
{
    printf("%s\n", msg);
    for (Block *b = head; b; b = b->next)
        printf("  [%4d + %4d] %s\n", b->offset, b->size, b->free ? "free" : "used");
}

static int next_pow2(int n)
{
    int s = MIN;
    while (s < n)
        s *= 2;
    return s;
}

static Block *split_to(Block *b, int need)
{
    while (b->size / 2 >= need && b->size / 2 >= MIN) {
        int half = b->size / 2;
        Block *buddy = make(b->offset + half, half, 1);
        buddy->next = b->next;
        b->next = buddy;
        b->size = half;
        printf("  split %d+%d -> %d and %d\n", b->offset, half * 2, half, half);
    }
    return b;
}

void *buddy_alloc(int size)
{
    int need = next_pow2(size);
    for (Block *b = head; b; b = b->next) {
        if (b->free && b->size >= need) {
            split_to(b, need);
            b->free = 0;
            return b;
        }
    }
    return NULL;
}

static int buddy_of(int offset, int size)
{
    return offset ^ size;
}

void buddy_free(Block *target)
{
    target->free = 1;
    int merged = 1;
    while (merged) {
        merged = 0;
        for (Block *b = head; b && b->next; b = b->next) {
            if (!b->free || !b->next->free || b->size != b->next->size)
                continue;
            if (buddy_of(b->offset, b->size) != b->next->offset)
                continue;
            printf("  merge %d and %d -> %d\n", b->size, b->next->size, b->size * 2);
            Block *n = b->next;
            b->size *= 2;
            b->next = n->next;
            free(n);
            merged = 1;
            break;
        }
    }
}

int main(void)
{
    head = make(0, HEAP, 1);
    show("initial heap");

    Block *a = buddy_alloc(70);
    printf("alloc 70 (rounded to %d) at %d\n", a->size, a->offset);
    show("after A");

    Block *b = buddy_alloc(35);
    printf("alloc 35 (rounded to %d) at %d\n", b->size, b->offset);
    show("after B");

    Block *c = buddy_alloc(120);
    printf("alloc 120 (rounded to %d) at %d\n", c->size, c->offset);
    show("after C");

    printf("free A\n");
    buddy_free(a);
    show("after free A");

    printf("free B\n");
    buddy_free(b);
    show("after free B");

    printf("free C\n");
    buddy_free(c);
    show("after free C");

    while (head) {
        Block *n = head->next;
        free(head);
        head = n;
    }
    return 0;
}
