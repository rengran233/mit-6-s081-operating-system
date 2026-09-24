#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int pid;
    int right_pipe[2];

    pipe(right_pipe);
    pid = fork();

    if (pid < 0) {
        printf("fork error");
        exit(1);
    } else if (pid > 0) {
        // parent process
        close(right_pipe[0]);
        for (int i = 2; i <= 280; i++) {
            write(right_pipe[1], &i, 4);
        }
        close(right_pipe[1]);
        wait(0);
    } else {
        // child process
        int prime_to_sieve = 0;
        int left_pipe[2];
        int num = 0;
        int has_child = 0;

        left_pipe[0] = right_pipe[0];
        close(right_pipe[1]); // set left pipe to original right pipe, but the write end is useless
        pipe(right_pipe);
        read(left_pipe[0], &prime_to_sieve, 4);
        printf("prime %d\n", prime_to_sieve);

        while (read(left_pipe[0], &num, 4) > 0) {
            if (num % prime_to_sieve != 0) {
                write(right_pipe[1], &num, 4);
                if (!has_child) {
                    // parent proc set
                    has_child = 1;
                    printf("prime %d\n", num); 

                    if (fork() == 0) {
                        // child proc set
                        has_child = 0;
                        prime_to_sieve = num;

                        close(left_pipe[0]); // no longer need grandparent's read end
                        left_pipe[0] = right_pipe[0];
                        close(right_pipe[1]);
                        pipe(right_pipe);
                    }
                }
            }
        }
        close(right_pipe[0]);
        close(right_pipe[1]);
        wait(0);
    }

    exit(0);
}
