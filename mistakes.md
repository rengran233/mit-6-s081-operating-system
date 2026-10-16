# MIT 6.S081 Operating System Mistakes Log

## Lab Syscall: System Calls

### System Call Tracing

要求：打印每个进程及其子进程执行的特定系统调用
输入：mask，标记哪些调用需要被 trace
返回值：返回
输出：print 相关调用信息
- print 的位置？
如果在syscall处打印，如何判断是否有trace生效
- 如何判定打印
- 打印信息的来源
- 

mask 初始化

位掩码筛选
