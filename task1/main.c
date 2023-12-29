#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

void dump() {
    struct timeval tp;
    gettimeofday(&tp, 0);
    struct tm *t = localtime(&tp.tv_sec);
    printf("pid: %d, ppid: %d, time: %02d:%02d:%02d:%03ld\n", getpid(),
        getppid(), t->tm_hour, t->tm_min, t->tm_sec, tp.tv_usec/1000);
}

int main() {
    pid_t pid1, pid2;
    pid1 = fork();
    if (pid1 < 0) {
        printf("fork1 failed\n");
        return 1;
    }
    if (pid1 > 0) {
        // printf("%d", pid1);
        pid2 = fork();
        if (pid2 < 0) {
            printf("fork2 failed\n");
            return 1;
        }
    }
    dump();
    if (pid2 > 0 && pid1 > 0) {
        system("ps -x");
    }
    return 0;
}
