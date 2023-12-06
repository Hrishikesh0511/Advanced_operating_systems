#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int main(int argc,char *argv[]) {
    if(argc<=2)
    {
        printf("Expected more arguments\n");
        exit(1);
        return 0;
    }
    if((fork()==0))
    {
        int new_sp=atoi(argv[1]);
        int pid=atoi(argv[2]);
        if(new_sp<0 || new_sp>100)
        {
            printf("Error:static priority should be in range (0-100)\n");
            exit(1);
        }
        int old_sp=set_priority(new_sp, pid);
        if(old_sp<0)
        {
            printf("Error:Process with pid (%d) doesn't exist\n", pid);
            exit(1);
        }
        printf("Process with pid (%d) has been changed priority from %d to %d\n", pid, old_sp, new_sp);
        exit(0);
    }
    else{
        int wpid;
        wait(&wpid);
        exit(0);
    }
    return 0;
 }