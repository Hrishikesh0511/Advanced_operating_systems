#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int main(int argc,char *argv[]) {
    if(argc<=2)
    {
        printf("Expected more arguments\n");
        return 0;
    }
    if((fork()==0))
    {
        char **rem=argv+2;
        trace(atoi(argv[1]));
        exec(argv[2],rem);
        exit(0);
    }
    else{
        int wpid;
        wait(&wpid);
        exit(0);
    }
    return 0;
 }