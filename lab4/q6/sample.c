#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

static void print_stat(const char *path, struct stat *st)
{
    printf("\n=== %s ===\n", path);
    printf("st_dev     (device ID)            : %llu\n", (unsigned long long)st->st_dev);
    printf("st_ino     (inode number)         : %llu\n", (unsigned long long)st->st_ino);
    printf("st_mode    (protection)           : %o (octal)\n", st->st_mode);
    printf("st_nlink   (number of hard links) : %llu\n", (unsigned long long)st->st_nlink);
    printf("st_uid     (user ID of owner)     : %u\n", st->st_uid);
    printf("st_gid     (group ID of owner)    : %u\n", st->st_gid);
    printf("st_rdev    (device ID, special)   : %llu\n", (unsigned long long)st->st_rdev);
    printf("st_size    (total size, bytes)    : %lld\n", (long long)st->st_size);
    printf("st_blksize (block size for I/O)   : %ld\n", (long)st->st_blksize);
    printf("st_blocks  (number of 512B blocks): %lld\n", (long long)st->st_blocks);
    printf("st_atime   (last access)          : %ld  %s", (long)st->st_atime, ctime(&st->st_atime));
    printf("st_mtime   (last modification)    : %ld  %s", (long)st->st_mtime, ctime(&st->st_mtime));
    printf("st_ctime   (last status change)   : %ld  %s", (long)st->st_ctime, ctime(&st->st_ctime));
}

int main(int argc, char *argv[])
{
    char *dirpath;
    char path[1024];
    DIR *dp;
    struct dirent *ent;
    struct stat st;

    if (argc != 2) {
        printf("Usage: %s directory\n", argv[0]);
        return 1;
    }
    dirpath = argv[1];

    dp = opendir(dirpath);
    if (dp == NULL) {
        perror("opendir");
        return 1;
    }

    while ((ent = readdir(dp)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;
        snprintf(path, sizeof(path), "%s/%s", dirpath, ent->d_name);
        if (stat(path, &st) == -1) {
            perror(path);
            continue;
        }
        print_stat(path, &st);
    }

    closedir(dp);
    return 0;
}
