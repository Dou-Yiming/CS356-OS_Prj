#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/unistd.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/gfp.h>
#include <linux/string.h>

MODULE_LICENSE("Dual BSD/GPL");
#define __NR_hellocall 356

struct prinfo 
{
    pid_t parent_pid; /* process id of parent */
    pid_t pid; /* process id */
    pid_t first_child_pid; /* pid of youngest child */
    pid_t next_sibling_pid; /* pid of older sibling */
    long state; /* current state of process */
    long uid; /* user id of process owner */
    char comm[64]; /* name of program executed */
    int depth; /* depth of the task */
};

// get process info
void get_prinfo(struct task_struct* task, struct prinfo* prinfo,int *nr)
{
    prinfo->parent_pid=task->parent?task->parent->pid:(pid_t)0;
    prinfo->pid=task->pid;
    if(list_empty(&(task->children)))//no child
        prinfo->first_child_pid=(pid_t)0;
    else
        prinfo->first_child_pid=list_entry(&(task->children), struct task_struct, children)->pid;
    if(list_empty(&(task->sibling)))//no sibling
        prinfo->next_sibling_pid=(pid_t)0;
    else
        prinfo->next_sibling_pid=list_entry(&(task->sibling).next, struct task_struct, sibling)->pid;
    prinfo->state=task->state;
    prinfo->uid=task->cred->uid;
    strcpy(prinfo->comm,task->comm);
    *nr+=1;
}

//dfs process tree
void dfs_ptree(struct task_struct* root,struct prinfo* buf, int *nr, int depth)
{
    struct prinfo*cur_prinfo=buf+*nr;
    cur_prinfo->depth=depth;
    get_prinfo(root,cur_prinfo,nr);//get info of root
    struct list_head*pos=NULL;
    list_for_each(pos,&(root->children))//for each child
    {
        dfs_ptree(list_entry(pos,struct task_struct,sibling),buf,nr,depth+1);
    }
}

//system call
int ptree(struct prinfo *buf, int *nr)
{
    printk("call syscall: 356...\n");
    //malloc
    struct prinfo* ker_buf=(struct prinfo*)kcalloc(1024,sizeof(struct prinfo),GFP_KERNEL);
    int * ker_nr=(int*)kzalloc(sizeof(int),GFP_KERNEL);
    //check
    if(!ker_buf||!ker_nr)
    {
        printk("KERNEL MALLOC FAILED!\n");
        return 1;
    }
    //dfs
    read_lock(&tasklist_lock);//prevent writing while reading
    dfs_ptree(&init_task,ker_buf,ker_nr,0);
    read_unlock(&tasklist_lock);
    //move to usr space
    if(copy_to_user(buf,ker_buf,(*ker_nr)*sizeof(struct prinfo))||copy_to_user(nr,ker_nr,sizeof(int)))
    {
        printk("COPY FAILED!\n");
        kfree(ker_buf);
        kfree(ker_nr);
        return 1;
    }
    else
    {
        kfree(ker_buf);
        kfree(ker_nr);
        return 0;
    }
}



static int (*oldcall)(void);
static int sys_prteecall(struct prinfo* buf,int* nr)
{
    printk("this is my system ptree call !\n");
    return ptree(buf,nr);
}
static int addsyscall_init(void)
{
    long *syscall = (long*)0xc000d8c4;
    oldcall=(int(*)(void))(syscall[__NR_hellocall]);
    syscall[__NR_hellocall]=(unsigned long)sys_prteecall;
    printk(KERN_INFO "module load!\n");
    return 0;
}
static void addsyscall_exit(void)
{
    long *syscall = (long*)0xc000d8c4;
    syscall[__NR_hellocall]=(unsigned long)oldcall;
    printk(KERN_INFO "module exit!\n");
}

module_init(addsyscall_init);
module_exit(addsyscall_exit);