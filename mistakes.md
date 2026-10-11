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

### find

#### 设计多余的浅模块

原方案使用 `get_subdirs()` 函数获取目录的子目录列表然后遍历递归调用 find。但是在获取子目录的过程中已经进入了递归流程，该函数的设计增加了完全多余的复杂度，并且需要额外维护传递许多不必要的值，带来了极大的编写困难。

#### 优化：复用 dirent.name

实现思路是将每个文件的 base name 与目标比对，并打印符合要求的文件的路径。

原版本使用 switch 对文件类型分支处理并统一使用 get_base() 获取 base name。但是 dirent 本身就已经包含 base name。所以优化后直接使用 T_DIR 分支中的 read 循环中得到的 dirent.name 属性作为 base name，省去 get_base() 函数的代码和开销。

这样做隐式转变了 path 参数的含义：原来的 switch 方案接受任何类型的文件路径作为参数，并在 switch 内判断和打印路径，递归时也对每个文件进行递归。但 dirent 的信息只在 read 循环中作为目录的子项被读取，T_DIR 之外的分支得不到这个信息，必须使用 get_base()。

优化版本的 find 只接受指向目录的 path 作为参数，并在 read 循环中打印每个匹配的目录项。（**收窄了接口的契约**）

设计的差异在于：初版是直接复制了 ls 程序的结构，ls 处理的对象就是 path，并且包括起点；而 find 还要用到 base name，不包括起点。
