#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } 
    else if (pid == 0) {
        /* Child Process */
        printf("--- Child Process Output ---\n");
        printf("  Current Process ID (PID)  : %d\n", getpid());
        printf("  Parent Process ID (PPID) : %d\n", getppid());
        printf("  Fork return value (pid)  : %d\n\n", pid);
        exit(0);
    } 
    else {
        /* Parent Process */
        wait(NULL); // Keep output clean and ordered
        printf("--- Parent Process Output ---\n");
        printf("  Current Process ID (PID)  : %d\n", getpid());
        printf("  Parent Process ID (PPID) : %d\n", getppid());
        printf("  Created Child PID (pid)  : %d\n", pid);
    }

    return 0;
}
