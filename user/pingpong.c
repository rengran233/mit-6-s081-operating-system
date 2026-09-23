#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int pid;
    int ping[2];
    int pong[2];
    pipe(ping);
    pipe(pong);
    
    pid = fork();
    if (pid == 0) {
        // child process
        int buf[1];
        close(ping[1]);
        close(pong[0]);

        // read from ping pipe
        if (read(ping[0], buf, sizeof(buf))) {
            printf("%d: received ping\n", getpid());
        }

        // write to pong pipe
        write(pong[1], "A", 1); // "A" has two bytes, but only one is sent

        close(ping[0]);
        close(pong[1]);
    } else if (pid > 0) {
        // parent process
        int buf[1];
        close(pong[1]);
        close(ping[0]);

        // write to ping pipe
        write(ping[1], "A", 1);

        // read from pong pipe
        if (read(pong[0], buf, sizeof(buf))) {
            printf("%d: received pong\n", getpid());
        }

        close(ping[1]);
        close(pong[0]);
        wait(0);
    } else {
        printf("fork error");
    }
    exit(0);
}