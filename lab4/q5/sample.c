#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

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

    printf("Inode numbers in %s:\n", dirpath);
    while ((ent = readdir(dp)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;
        snprintf(path, sizeof(path), "%s/%s", dirpath, ent->d_name);
        if (stat(path, &st) == -1) {
            perror(path);
            continue;
        }
        printf("%llu\t%s\n", (unsigned long long)st.st_ino, ent->d_name);
    }

    closedir(dp);
    return 0;
}
