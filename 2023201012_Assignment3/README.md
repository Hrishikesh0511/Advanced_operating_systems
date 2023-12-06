# REPORT
## Requirement-1: System Call
* For implementing the trace system call, I changed the following files:
1. ### syscall.c:
* This file generally contains all the function pointers to all the system call functions so, addition of any system call need to have a pointer.
> [SYS_trace] &nbsp; &nbsp; &nbsp; &nbsp; sys_trace

