#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } 
    else if (pid == 0) {
        /* Child Process */
        printf("[Child PID: %d] Exiting immediately to become a zombie...\n", getpid());
        exit(0);
    } 
    else {
        /* Parent Process */
        printf("[Parent PID: %d] Child created (PID: %d).\n", getpid(), pid);
        printf("[Parent PID: %d] Sleeping for 15s WITHOUT calling wait()...\n", getpid());
        
        sleep(15); // Child remains a zombie during this period
        
        printf("[Parent PID: %d] Exiting. init process will adopt and reap child.\n", getpid());
    }

    return 0;
}
