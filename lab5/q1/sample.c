#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int status;
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } 
    else if (pid == 0) {
        /* Child Process */
        printf("[Child] Process started (PID: %d). Doing work...\n", getpid());
        sleep(2); // Simulate work
        printf("[Child] Work complete. Exiting.\n");
        exit(0);
    } 
    else {
        /* Parent Process */
        printf("[Parent] Waiting for child (PID: %d) to complete...\n", pid);
        wait(&status); // Blocks parent until child exits
        printf("[Parent] Child completed. Resume parent execution.\n");
    }

    return 0;
}

