#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *p = malloc(8);
    strcpy(p, "hello");
    printf("malloc(8): %s\n", p);

    p = realloc(p, 32);          /* resize (grow) */
    strcat(p, " world");
    printf("realloc(32): %s\n", p);

    p = realloc(p, 6);           /* resize (shrink) */
    p[5] = '\0';
    printf("realloc(6): %s\n", p);

    free(p);                     /* release */
    printf("freed\n");
    return 0;
}
