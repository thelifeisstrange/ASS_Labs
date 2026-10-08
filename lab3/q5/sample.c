#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char *dirpath = (argc > 1) ? argv[1] : ".";
    char path[1024];
    char ans[8];
    DIR *dp;
    struct dirent *ent;
    struct stat st;
    uid_t me = getuid();

    dp = opendir(dirpath);
    if (dp == NULL) {
        perror("opendir");
        return 1;
    }

    printf("Your C source files in %s:\n", dirpath);
    while ((ent = readdir(dp)) != NULL) {
        size_t len = strlen(ent->d_name);
        if (len < 3 || strcmp(ent->d_name + len - 2, ".c") != 0)
            continue;

        snprintf(path, sizeof(path), "%s/%s", dirpath, ent->d_name);
        if (stat(path, &st) == -1)
            continue;
        if (st.st_uid != me)
            continue;

        printf("\n%s (inode %llu)\n", path, (unsigned long long)st.st_ino);
        printf("Grant read permission to others? (y/n): ");
        if (scanf("%7s", ans) != 1)
            break;
        if (ans[0] == 'y' || ans[0] == 'Y') {
            if (chmod(path, st.st_mode | S_IROTH) == -1)
                perror("chmod");
            else
                printf("Others now have read permission on %s\n", path);
        }
    }

    closedir(dp);
    return 0;
}
