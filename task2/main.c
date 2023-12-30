#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

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

void create_processes(int level, int children, int index) {
    if (level <= 0) {
        dump(index);
        return;
    }
    for (int i = 0; i < children; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            printf("fork %d failed\n", index);
            return;
        }
        if (pid == 0) {
            create_processes(level - 1, children, index * children + 1 + i);
            return;
        }
    }

    dump(index);
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
    
    create_processes(level, children, 0);
}