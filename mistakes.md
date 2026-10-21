# MIT 6.S081 Operating System Mistakes Log

## Lab Syscall: System Calls

### System Call Tracing

在 struct proc 中添加一个 mask 属性，通过 trace 设置，每次执行 syscall 时都会使用 mask 位掩码来决定是否打印调用信息

### Attack

attacktest 的主进程分别在两个子进程中 exec 了 secret 和 attack。如何用 attack 获取另一个子进程中没有清理的内存？直接新分配吗

- xv6 使用 FIFO 结构管理内存页的分配和释放，直接申请就能得到秘密页

进程的 fork exec 等操作也会造成内存分配，直接复制 secret 的代码不能精准找到秘密页，需要遍历哪个页包含了信息
