#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    char newpath[1024];
    struct stat st;

    if (argc != 2) {
        printf("Usage: %s oldpath\n", argv[0]);
        return 1;
    }

    snprintf(newpath, sizeof(newpath), "%s.hard", argv[1]);

    if (link(argv[1], newpath) == -1) {
        perror("link");
        return 1;
    }

    printf("Old path: %s\n", argv[1]);
    printf("New path: %s\n", newpath);

    if (stat(newpath, &st) == 0) {
        printf("Hard link created. inode = %llu, nlink = %llu\n",
               (unsigned long long)st.st_ino,
               (unsigned long long)st.st_nlink);
    }

    if (unlink(newpath) == -1) {
        perror("unlink");
        return 1;
    }

    printf("Unlinked %s\n", newpath);

    if (stat(argv[1], &st) == 0) {
        printf("Original file remains. inode = %llu, nlink = %llu\n",
               (unsigned long long)st.st_ino,
               (unsigned long long)st.st_nlink);
    }

    return 0;
}
