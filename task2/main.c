#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/wait.h>

void dump(int index) {
    struct timeval tp;
    struct tm *t;
    int status;
    while (1) {
        if (index == 0) {
            usleep(2000);
            continue;
        }
        gettimeofday(&tp, 0);
        t = localtime(&tp.tv_sec);
        printf("process number: %d, pid: %d, ppid: %d, time: %02d:%02d:%02d:%03ld\n", index, getpid(),
            getppid(), t->tm_hour, t->tm_min, t->tm_sec, tp.tv_usec/1000);
        usleep(index * 200 * 1000);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s n m\n", argv[0]);
        return 1;
    }

    int level = atoi(argv[1]), children = atoi(argv[2]);
    if (level <= 0 || children <= 0) {
        printf("Arguments should be positive\n");
        return 1;
    }
    
    int index = 0;
    int i;
    int j = 0;
    start:
    level -= 1;
    if (level < 0) {
        goto end;
    }
    for (i = 0; i < children; i++) {
        j = index;
        index = index * children + 1 + i;
        pid_t pid = fork();
        if (pid < 0) {
            printf("fork %d failed\n", index);
            return 1;
        }
        if (pid == 0) {
            goto start;
        } else {
            index = j;
        }
    }
    end:
    dump(index);
}