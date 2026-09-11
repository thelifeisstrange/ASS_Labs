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
        printf("[Loader Child] Loading ./program1 binary executable...\n");
        
        // Replaces child's code with program1 executable
        execl("./program1", "program1", NULL);

        // execl only returns if an error occurs
        perror("Exec failed");
        exit(1);
    } 
    else {
        /* Parent Process */
        printf("[Loader Parent] Waiting for loaded executable to complete...\n");
        wait(NULL);
        printf("[Loader Parent] Execution finished.\n");
    }

    return 0;
}
