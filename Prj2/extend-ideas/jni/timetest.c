#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sched.h>
#include <sys/time.h>
#include <unistd.h>

int in_sched_method;
char *type[9] = {"SCHED_NORMAL", "SCHED_FIFO", "SCHED_RR", "", "", "", "SCHED_WRR"};

int main()
{
    int cur_pid, i, id, child_pid, sum, j, ret, father_pid;
    int begintime, responsetime, endtime;
    int exe_time;
    struct sched_param param;
    printf("Choice of Scheduling algorithms (0-NORMAL,1-FIFO,2-RR,6-WRR) : ");
    scanf("%d", &in_sched_method);
    printf("Input scheduling algorithm is %s\n", type[in_sched_method]);
    father_pid = getpid();
    param.sched_priority = 60;
    ret = sched_setscheduler(father_pid, in_sched_method, &param);
    struct timeval tv;
    struct timezone tz;
    for (i = 1; i <= 10; i++)
    {
        gettimeofday(&tv, &tz);
        begintime = tv.tv_usec;
        printf("P%d begintime: %d\n", i, begintime);
        cur_pid = fork();
        if (cur_pid == 0)
        {
            gettimeofday(&tv, &tz);
            responsetime = tv.tv_usec;
            printf("P%d responsetime: %d\n", i, responsetime);
            id = 1;
            sum = 0;
            for (j = 1; j <= 1e8; j++)
                sum++;
            gettimeofday(&tv, &tz);
            endtime = tv.tv_usec;
            printf("P%d endtime: %d\n", i, endtime);
            break;
        }
    }
    if (id == 2)
    {
        for (i = 1; i <= 10; i++)
            wait(NULL);
    }
    return 0;
}
