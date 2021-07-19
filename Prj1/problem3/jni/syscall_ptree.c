#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    printf("Testing problem 3...\n\n");
    int pid=fork();
    if(pid<0)
    {
        printf("FORK ERROR!\n");
        return 1;
    }
    else if(pid==0)//child
    {
        printf("This is child process, pid = %lu\n",(unsigned long)getpid());
        execl("/data/code/problem2/helloARM","helloARM",NULL);
    }
    else//parent
        printf("This is parent process, pid = %lu\n",(unsigned long)getpid());
    return 0;
}