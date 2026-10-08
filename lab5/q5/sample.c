#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        printf("Child %d: parent PID before orphan = %d\n", getpid(), getppid());
        sleep(5);
        printf("Child %d: parent PID after orphan  = %d (adopted by init/launchd)\n",
               getpid(), getppid());
        return 0;
    }

    printf("Parent %d exiting immediately, child %d will become an orphan.\n",
           getpid(), pid);
    exit(0);
}
