#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *buf = malloc(8);
    if (!buf) {
        perror("malloc");
        return 1;
    }
    strcpy(buf, "hello");
    printf("after malloc(8):  \"%s\"  (ptr %p)\n", buf, (void *)buf);

    /* grow the block so a longer string fits */
    char *grown = realloc(buf, 32);
    if (!grown) {
        perror("realloc grow");
        free(buf);
        return 1;
    }
    buf = grown;
    strcat(buf, " manipal");
    printf("after realloc(32): \"%s\"  (ptr %p)\n", buf, (void *)buf);

    /* shrink the block; contents up to the new size are kept */
    char *shrunk = realloc(buf, 6);
    if (!shrunk) {
        perror("realloc shrink");
        free(buf);
        return 1;
    }
    buf = shrunk;
    buf[5] = '\0';
    printf("after realloc(6):  \"%s\"  (ptr %p)\n", buf, (void *)buf);

    /* release the block back to the heap */
    free(buf);
    buf = NULL;
    printf("after free:        memory released (ptr set to NULL)\n");
    return 0;
}
