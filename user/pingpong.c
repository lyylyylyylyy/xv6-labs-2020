#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define R 0
#define W 1

int time[10];

int main(int argc, char *argv[])
{
    /** Use pipe to create a pipe.*/
    int parent_fd[2], child_fd[2];
    pipe(child_fd);
    pipe(parent_fd);
    /**Use fork to create a child.*/
    if (fork() == 0) {
        /* child process */
        close(parent_fd[W]);
        close(child_fd[R]);
        char buffer = ' ';
        read(parent_fd[R], &buffer, 1);
        printf("%d: received ping\n", getpid());
        write(child_fd[W], &buffer, 1);
        close(parent_fd[R]);
        close(child_fd[W]);
    } else {
        /* parent process */
        close(parent_fd[R]);
        close(child_fd[W]);
        char buffer = ' ';
        write(parent_fd[W], &buffer, 1);
        read(child_fd[R], &buffer, 1);
        printf("%d: received pong\n", getpid());
        close(parent_fd[W]);
        close(child_fd[R]);
        wait(time);
    }
    exit(0);
}