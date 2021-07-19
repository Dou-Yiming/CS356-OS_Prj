#include<stdio.h>
#include <stdlib.h>

struct prinfo {
    pid_t parent_pid; /* process id of parent */
    pid_t pid; /* process id */
    pid_t first_child_pid; /* pid of youngest child */
    pid_t next_sibling_pid; /* pid of older sibling */
    long state; /* current state of process */
    long uid; /* user id of process owner */
    char comm[64]; /* name of program executed */
};

void printk_prinfo(struct prinfo* pf)
{
    printf("pid = %lu, parent_pid = %lu, comm = %s\n", (unsigned long)pf->pid, (unsigned long)pf->parent_pid, pf->comm);
}


int main()
{
    printf("Testing problem1...\n\n");
    struct prinfo* buf=(struct prinfo*)malloc(1024*sizeof(struct prinfo));
    int nr=0;
    int flag=syscall(356,buf,&nr);
    printf("syscall return %d\n",flag);
    printf("nr = %d\n",nr);
    int i=0;
    for(i=0;i<nr;++i)
    {
        printf("===============i = %d===============\n",i);
        printk_prinfo(buf+i);
        printf("\n");
    }
    free(buf);

    printf("Test End!:\n\n");
    return 0;
}