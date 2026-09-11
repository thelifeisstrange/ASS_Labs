#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    char newpath[1024];
    char target[1024];
    struct stat st;
    ssize_t n;

    if (argc != 2) {
        printf("Usage: %s oldpath\n", argv[0]);
        return 1;
    }

    snprintf(newpath, sizeof(newpath), "%s.soft", argv[1]);

    if (symlink(argv[1], newpath) == -1) {
        perror("symlink");
        return 1;
    }

    printf("Old path: %s\n", argv[1]);
    printf("New path: %s\n", newpath);

    n = readlink(newpath, target, sizeof(target) - 1);
    if (n >= 0) {
        target[n] = '\0';
        printf("Soft link created. %s -> %s\n", newpath, target);
    }

    if (lstat(newpath, &st) == 0) {
        printf("lstat on link: inode = %llu, nlink = %llu, size = %lld (path length)\n",
               (unsigned long long)st.st_ino,
               (unsigned long long)st.st_nlink,
               (long long)st.st_size);
    }

    if (unlink(newpath) == -1) {
        perror("unlink");
        return 1;
    }

    printf("Unlinked %s\n", newpath);
    printf("Original file %s is unchanged.\n", argv[1]);
    return 0;
}
