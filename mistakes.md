# MIT 6.S081 Operating System Mistakes Log

## Lab Util

### primes

#### 弄混了 fork 后父子进程的 pid 特征

```
int pid = fork();
if (pid == 0); // child proc
if (pid > 0); // parent proc
if (pid < 0); // fork error
```

子进程不会返回自己的 PID

#### 误解了 fd 的用法，直接用新分配的 fd 覆盖了旧的变量，导致旧 fd 仍然在内核中打开，造成泄露。

```
if (fork() == 0) {
    ...
    left_pipe[0] = right_pipe[0]; // 子进程的 left pipe 继承父进程 right pipe 的读端
    pipe(right_pipe);        // 直接覆盖，旧的 right_pipe[1] 没有 close
}
```

覆盖 fd 不会自动进行资源回收

#### 父 sieve 未 wait 子进程

这是 grade 不通过的核心原因

#### 存在更优范式

v1-v2 版本在实现时遇到了许多问题：
- 子进程无法执行 sieve 初始化，必须先修改再让父进程单独 revert
- 循环会在单个进程内多次 fork，使用 has_child 作为标记

如果采用递归写法，整个逻辑会清晰很多

```
void sieve(int left_fd) {
    int prime;
    if (read(left_fd, &prime, sizeof(prime)) == 0) {
        close(left_fd);
        exit(0);
    }
    printf("prime %d\n", prime);

    int right_pipe[2];
    pipe(right_pipe);
    if (fork() == 0) {
        close(left_fd);
        close(right_pipe[1]);
        sieve(right_pipe[0]);   // 子进程从函数开头执行,干净
        exit(0);
    } else {
        close(right_pipe[0]);
        int n;
        while (read(left_fd, &n, sizeof(n)) > 0)
            if (n % prime != 0)
                write(right_pipe[1], &n, sizeof(n));
        close(left_fd);
        close(right_pipe[1]);
        wait(0);
        exit(0);
    }
}
```

也可以使用 goto 跳出循环，但是不如递归清晰
