#include <stdio.h>
#include <sched.h>

char *SCHED_NAME[7] = {"SCHED_NORMAL", "SCHED_FIFO", "SCHED_RR", "SCHED_BATCH", "SCHED_ISO", "SCHED_IDLE", "SCHED_WRR"};

int main()
{
    int choice_sched, process_id, pprio, pre_sched, return_set;
    struct sched_param __param;

    printf("Please input the choice of scheduling algorithms (0-NORMAL,1-FIFO,2-RR,6-WRR):");
    scanf("%d", &choice_sched);
    printf("Current scheduling algorithm is %s\n", SCHED_NAME[choice_sched]);
    printf("Please input the id of the testprocess:");
    scanf("%d", &process_id);
    printf("Set Process's priority (1-99):");
    scanf("%d", &pprio);
    __param.sched_priority = pprio;
    printf("current process's priority is: %d\n", pprio);
    pre_sched = sched_getscheduler(process_id);
    printf("pre scheduler : %s\n", SCHED_NAME[pre_sched]);
    return_set = sched_setscheduler(process_id, choice_sched, &__param);

    if (return_set != 0)
    {
        printf("sched_setscheduler failed!");
        return -1;
    }
    else
        printf("Successfully switched!");

    printf("cur scheduler : %s\n", SCHED_NAME[choice_sched]);

    // get time slice
    struct timespec ts;
    sched_rr_get_interval(process_id, & ts);
    printf("Time Slice: %ld ms\n",ts.tv_nsec/1000000);
    return 0;
}