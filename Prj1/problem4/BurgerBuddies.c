#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

// global variables
int cook_num, cashier_num, cust_num, rack_num;
// mutex_sem
sem_t mutex_print;
sem_t mutex_rack;
sem_t mutex_entry;
sem_t mutex_leave;

// syn_sem
sem_t empty_rack;
sem_t burger_num;
sem_t order_num;
sem_t provide_num;

// terminate_signal
int left_num = 0;
int terminate_signal = 0;

// threads
pthread_t* cooks;
pthread_t* customers;
pthread_t* cashiers;

int arg_parser(int argc, char* argv[])
{
    if(argc!=5)
    {
        printf("ARG_NUM_ERROR!\n");
        return 1;
    }
    cook_num     =  atoi(argv[1]);
    cashier_num  =  atoi(argv[2]);
    cust_num   =  atoi(argv[3]);
    rack_num     =  atoi(argv[4]);
    if(!(cook_num > 0 && cashier_num > 0 && cust_num > 0 && rack_num > 0))
    {
        printf("NEGATIVE_ARG_ERROR!\n");
        return 1;
    }
    return 0;
}

int semaphore_init()
{
    int ans = 0;
    // mutex_sem
    ans |= sem_init(&mutex_print, 0, 1);
    ans |= sem_init(&mutex_rack, 0, 1);
    ans |= sem_init(&mutex_entry, 0, 1);
    ans |= sem_init(&mutex_leave, 0, 1);

    // syn_sem
    ans |= sem_init(&empty_rack, 0, rack_num);
    ans |= sem_init(&burger_num, 0, 0);
    ans |= sem_init(&order_num, 0, 0);
    ans |= sem_init(&provide_num, 0, 0);
    
    if(ans != 0)
        printf("SEM_INIT_ERROR!\n");
    return ans;
}

void* cook_func(void* ID)
{
    unsigned long id = (unsigned long) ID;
    while(1)
    {
        sem_wait(&empty_rack);//check
        sem_wait(&mutex_rack);//lock

        sem_wait(&mutex_print);
        printf("Cook [%lu] make a burger.\n",id);
        sem_post(&mutex_print);

        sem_post(&mutex_rack);//unlock
        sem_post(&burger_num);
    }
}
void* cashier_func(void* ID)
{
    unsigned long id = (unsigned long) ID;
    while(1)
    {
        sem_wait(&order_num);//activate

        sem_wait(&mutex_print);
        printf("Casher [%lu] accepts an order.\n",id);//accept
        sem_post(&mutex_print);

        sem_wait(&burger_num);

        sem_wait(&mutex_rack);//lock

        sem_wait(&mutex_print);
        printf("Casher [%lu] take a burger to customor.\n",id);
        sem_post(&mutex_print);

        sem_post(&provide_num);
        sem_post(&empty_rack);
        sem_post(&mutex_rack);//unlock
    }
}
void* custom_func(void* ID)
{
    unsigned long id = (unsigned long) ID;

    sem_wait(&mutex_entry);//lock

    sem_wait(&mutex_print);
    printf("Customer [%lu] come.\n",id);
    sem_post(&mutex_print);

    sem_post(&order_num);//order
    sem_post(&mutex_entry);//unlock

    sem_wait(&provide_num);//take

    sem_wait(&mutex_leave);//lock
    left_num++;//leave
    if(left_num == cust_num)
        terminate_signal = 1;
    sem_post(&mutex_leave);//unlock
}


int thread_malloc()
{
    cooks = (pthread_t*) malloc(cook_num * sizeof(pthread_t));
    customers = (pthread_t*) malloc(cust_num * sizeof(pthread_t));
    cashiers = (pthread_t*) malloc(cashier_num * sizeof(pthread_t));
    return cooks != NULL && customers != NULL && cashiers != NULL;
}
int thread_create()
{
    int i;
    for(i=0;i<cook_num;++i)
        if(pthread_create(cooks+i, NULL, cook_func, (void*)(i+1))!=0)
            return 0;
    for(i=0;i<cashier_num;++i)
        if(pthread_create(cashiers+i, NULL, cashier_func, (void*)(i+1))!=0)
            return 0;
    for(i=0;i<cust_num;++i)
        if(pthread_create(customers+i, NULL, custom_func, (void*)(i+1))!=0)
            return 0;
    return 1;
}
void thread_kill()
{
    int i;
    for(i=0;i<cook_num;++i)
        pthread_kill(cooks[i], 0);
    for(i=0;i<cust_num;++i)
        pthread_kill(customers[i], 0);
    for(i=0;i<cashier_num;++i)
        pthread_kill(cashiers[i], 0);
}
int thread_init()
{
    if(!thread_malloc())
    {
        printf("THREAD_MALLOC_ERROR!\n");
        return 1;
    }
    if(!thread_create())
    {
        printf("THREAD_CERATE_ERROR!\n");
        return 1;
    }
    return 0;
}
void thread_free()
{
    free(cooks);
    free(customers);
    free(cashiers);
}

void print_info()
{
    printf("Cooks [%d], Cashiers [%d], Customers [%d]\n",cook_num,cashier_num,cust_num);
    printf("Begin run\n");
}
int main(int argc, char* argv[])
{
    if(arg_parser(argc, argv))
        return 1;
    print_info();
    if(semaphore_init() != 0)
        return 1;
    if(thread_init() != 0)
        return 1;
        
    while(!terminate_signal);
    thread_kill();
    thread_free();
}