#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

static void copy_range(int src, int dest, off_t start, off_t nbytes)
{
    char buf[512];
    off_t left = nbytes;

    if (lseek(src, start, SEEK_SET) == -1) {
        perror("lseek");
        return;
    }
    while (left > 0) {
        ssize_t n = read(src, buf, left < (off_t)sizeof(buf) ? (size_t)left : sizeof(buf));
        if (n <= 0)
            break;
        write(dest, buf, (size_t)n);
        left -= n;
    }
}

int main(int argc, char *argv[])
{
    int src, f1, f2, f3;
    struct stat st;
    off_t size, third, rest;

    if (argc != 2) {
        printf("Usage: %s source_file\n", argv[0]);
        return 1;
    }

    src = open(argv[1], O_RDONLY);
    if (src < 0) {
        perror("open source");
        return 1;
    }
    if (fstat(src, &st) == -1) {
        perror("fstat");
        close(src);
        return 1;
    }

    size = st.st_size;
    third = size / 3;
    rest = size - 2 * third;

    f1 = open("initial.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    f2 = open("middle.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    f3 = open("last.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (f1 < 0 || f2 < 0 || f3 < 0) {
        perror("open dest");
        return 1;
    }

    copy_range(src, f1, 0, third);
    copy_range(src, f2, third, third);
    copy_range(src, f3, 2 * third, rest);

    close(src);
    close(f1);
    close(f2);
    close(f3);

    printf("File size = %lld bytes\n", (long long)size);
    printf("Copied initial  %lld bytes -> initial.txt\n", (long long)third);
    printf("Copied middle   %lld bytes -> middle.txt\n", (long long)third);
    printf("Copied last     %lld bytes -> last.txt\n", (long long)rest);
    return 0;
}
