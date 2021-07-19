#include "sched.h"

#include <linux/slab.h>
#include <linux/limits.h>

static inline void list_del_leaf_wrr_rq(struct wrr_rq *wrr_rq);
#define for_each_sched_wrr_entity(wrr_se) for (; wrr_se; wrr_se = NULL)
#define MAX_PRIO 500

static inline int find_group(struct task_group *tg)
{
	// printk("ENTER: find_group\n");
	static char group_path[PATH_MAX];
	cgroup_path(tg->css.cgroup, group_path, PATH_MAX);
	if(strcmp(group_path,"/")==0) // FORE
        return 1;
	return 0;
}

static inline int on_wrr_rq(struct sched_wrr_entity *wrr_se)
{
	// printk("ENTER: on_wrr_rq\n");
	return !list_empty(&wrr_se->run_list);
}

static inline void list_del_leaf_wrr_rq(struct wrr_rq *wrr_rq){}

static inline void dequeue_pushable_task(struct rq *rq, struct task_struct *p){}

static inline void enqueue_pushable_task(struct rq *rq, struct task_struct *p){}

static inline struct task_struct *wrr_task_of(struct sched_wrr_entity *wrr_se)
{
	// printk("ENTER: wrr_task_of\n");
	return container_of(wrr_se, struct task_struct, wrr);
}

static inline u64 sched_wrr_runtime(struct wrr_rq *wrr_rq)//get the run time
{
	// printk("ENTER: sched_wrr_runtime\n");
	return wrr_rq->wrr_runtime;
}

static inline struct wrr_rq *wrr_rq_of_se(struct sched_wrr_entity *wrr_se)//get the ready queue
{
	// printk("ENTER: wrr_rq_of_se\n");
	struct task_struct *p = wrr_task_of(wrr_se);
	struct rq *rq = task_rq(p);

	return &rq->wrr;
}

static inline int wrr_se_prio(struct sched_wrr_entity *wrr_se)//get the prioty
{
	// printk("ENTER: wrr_se_prio\n");
	return wrr_task_of(wrr_se)->prio;
}

static inline int wrr_rq_throttled(struct wrr_rq *wrr_rq)// check if outoftime
{
	// printk("ENTER: wrr_rq_throttled\n");
	return wrr_rq->wrr_throttled ;
}

static int sched_wrr_runtime_exceeded(struct wrr_rq *wrr_rq)
{
	// printk("ENTER: sched_wrr_runtime_exceeded\n");
	u64 runtime = sched_wrr_runtime(wrr_rq);

	if (wrr_rq->wrr_throttled)
		return wrr_rq_throttled(wrr_rq);
		
	runtime = sched_wrr_runtime(wrr_rq);
	if (runtime == RUNTIME_INF)
		return 0;

	return 0;
}

int alloc_wrr_sched_group(struct task_group *tg, struct task_group *parent)
{
	// printk("ENTER: alloc_wrr_sched_group\n");
	return 1;
}

static inline struct rq *rq_of_wrr_rq(struct wrr_rq *wrr_rq)
{
	// printk("ENTER: rq_of_wrr_rq\n");
	return wrr_rq->rq;
}

void free_wrr_sched_group(struct task_group *tg) {}

void init_wrr_rq(struct wrr_rq *wrr_rq, struct rq *rq)
{
	// printk("ENTER: init_wrr_rq\n");
	struct list_head *queue;
	queue = &wrr_rq->wrr_queue;
	INIT_LIST_HEAD(queue);

	wrr_rq->wrr_time = 0;
	wrr_rq->wrr_throttled = 0;
	wrr_rq->wrr_runtime = 0;
	raw_spin_lock_init(&wrr_rq->wrr_runtime_lock);
}

static inline void list_add_leaf_wrr_rq(struct wrr_rq *wrr_rq)//add one wrr to 
{
}

static inline void sched_wrr_avg_update(struct rq *rq, u64 wrr_delta) {}

static void update_curr_wrr(struct rq *rq)
{
	// printk("ENTER: update_curr_wrr\n");
	struct task_struct *curr = rq->curr;
	struct sched_wrr_entity *wrr_se = &curr->wrr;
	struct wrr_rq *wrr_rq = wrr_rq_of_se(wrr_se);
	u64 delta_exec;

	if (curr->sched_class != &wrr_sched_class)
		return;

	delta_exec = rq->clock_task - curr->se.exec_start;
	if (unlikely((s64)delta_exec < 0))
		delta_exec = 0;

	schedstat_set(curr->se.statistics.exec_max,
		      max(curr->se.statistics.exec_max, delta_exec));

	curr->se.sum_exec_runtime += delta_exec;

	curr->se.exec_start = rq->clock_task;
	cpuacct_charge(curr, delta_exec);

	sched_wrr_avg_update(rq, delta_exec);
}

static inline void inc_wrr_tasks(struct sched_wrr_entity *wrr_se, struct wrr_rq *wrr_rq)
{
	// printk("ENTER: inc_wrr_tasks\n");
	wrr_rq->wrr_nr_running++;
}

static inline void dec_wrr_tasks(struct sched_wrr_entity *wrr_se, struct wrr_rq *wrr_rq)
{
	// printk("ENTER: dec_wrr_tasks\n");
	WARN_ON(!wrr_rq->wrr_nr_running);
	wrr_rq->wrr_nr_running--;
}

static void __enqueue_wrr_entity(struct sched_wrr_entity *wrr_se, bool head)
{
	// printk("ENTER: __enqueue_wrr_entity\n");
	struct wrr_rq *wrr_rq = wrr_rq_of_se(wrr_se);
	struct list_head *queue = &wrr_rq->wrr_queue;

	/*
	 * Don't enqueue the group if its throttled, or when empty.
	 * The latter is a consequence of the former when a child group
	 * get throttled and the current group doesn't have any other
	 * active members.
	 */

	if (!wrr_rq->wrr_nr_running)
		list_add_leaf_wrr_rq(wrr_rq);

	if (head)
		list_add(&wrr_se->run_list, queue);
	else
		list_add_tail(&wrr_se->run_list, queue);

	inc_wrr_tasks(wrr_se, wrr_rq);
}

static void __dequeue_wrr_entity(struct sched_wrr_entity *wrr_se)
{
	// printk("ENTER: __dequeue_wrr_entity\n");
	struct wrr_rq *wrr_rq = wrr_rq_of_se(wrr_se);

	list_del_init(&wrr_se->run_list);

	dec_wrr_tasks(wrr_se, wrr_rq);
	if (!wrr_rq->wrr_nr_running)
		list_del_leaf_wrr_rq(wrr_rq);
}

static void dequeue_wrr_stack(struct sched_wrr_entity *wrr_se)
{
	// printk("ENTER: dequeue_wrr_stack\n");
	struct sched_wrr_entity *back = NULL;

	for_each_sched_wrr_entity(wrr_se) {
		wrr_se->back = back;
		back = wrr_se;
	}

	for (wrr_se = back; wrr_se; wrr_se = wrr_se->back) {
		if (on_wrr_rq(wrr_se))
			{
				// printk("on_wrr_rq: true\n");
				__dequeue_wrr_entity(wrr_se);
			}
	}
	// printk("EXIT: dequeue_wrr_stack\n");
}

static void enqueue_wrr_entity(struct sched_wrr_entity *wrr_se, bool head)
{
	// printk("ENTER: enqueue_wrr_entity\n");
	dequeue_wrr_stack(wrr_se);
	for_each_sched_wrr_entity(wrr_se)
		__enqueue_wrr_entity(wrr_se, head);
	// printk("EXIT: enqueue_wrr_entity\n");
}

static void dequeue_wrr_entity(struct sched_wrr_entity *wrr_se)
{
	// printk("ENTER: dequeue_wrr_entity\n");
	dequeue_wrr_stack(wrr_se);

	for_each_sched_wrr_entity(wrr_se) {
		struct wrr_rq *wrr_rq = NULL;

		if (wrr_rq && wrr_rq->wrr_nr_running)
			__enqueue_wrr_entity(wrr_se, false);
	}
	// printk("EXIT: dequeue_wrr_entity\n");
}

static void enqueue_task_wrr(struct rq *rq, struct task_struct *p, int flags)
{
	// printk("ENTER: enqueue_task_wrr\n");
    struct sched_wrr_entity *wrr_se = &p->wrr;
    if (flags & ENQUEUE_WAKEUP)
		wrr_se->timeout = 0;

    enqueue_wrr_entity(wrr_se, flags & ENQUEUE_HEAD);

    inc_nr_running(rq);
	// printk("EXIT: enqueue_task_wrr\n");
}

static void requeue_wrr_entity(struct wrr_rq *wrr_rq, struct sched_wrr_entity *wrr_se, int head)
{
	// printk("ENTER: requeue_wrr_entity\n");
	if (on_wrr_rq(wrr_se)) 
	{
        struct list_head *queue = &wrr_rq->wrr_queue;

		if (head)
			list_move(&wrr_se->run_list, queue);
		else
			list_move_tail(&wrr_se->run_list, queue);
	}
}

static void requeue_task_wrr(struct rq *rq, struct task_struct *p, int head)
{
	// printk("ENTER: requeue_task_wrr\n");
	struct sched_wrr_entity *wrr_se = &p->wrr;
	struct wrr_rq *wrr_rq;
	

	for_each_sched_wrr_entity(wrr_se) {
		wrr_rq = wrr_rq_of_se(wrr_se);
		requeue_wrr_entity(wrr_rq, wrr_se, head);
	}
	// printk("EXIT: requeue_task_wrr\n");
}

static void yield_task_wrr(struct rq *rq)
{
	// printk("ENTER: yield_task_wrr\n");
	requeue_task_wrr(rq, rq->curr, 0);
	// printk("EXIT: yield_task_wrr\n");
}

static void dequeue_task_wrr(struct rq *rq, struct task_struct *p, int flags)
{
	// printk("ENTER: dequeue_task_wrr\n");
	struct sched_wrr_entity *wrr_se = &p->wrr;

	update_curr_wrr(rq);
	dequeue_wrr_entity(wrr_se);

	dequeue_pushable_task(rq, p);

	dec_nr_running(rq);
	// printk("EXIT: dequeue_task_wrr\n");
}

static void check_preempt_curr_wrr(struct rq *rq, struct task_struct *p, int flags)
{
}

static struct sched_wrr_entity *pick_next_wrr_entity(struct rq *rq,
						   struct wrr_rq *wrr_rq)
{
	// printk("ENTER: pick_next_wrr_entity\n");
	struct sched_wrr_entity *next = NULL;
	struct list_head *queue = &wrr_rq->wrr_queue;

	next = list_entry(queue->next, struct sched_wrr_entity, run_list);

	return next;
}

static struct task_struct *_pick_next_task_wrr(struct rq *rq)
{
	struct sched_wrr_entity *wrr_se;
	struct task_struct *p;
	struct wrr_rq *wrr_rq;

	wrr_rq = &rq->wrr;

	if (!wrr_rq->wrr_nr_running)
		return NULL;

	if (wrr_rq_throttled(wrr_rq))
		return NULL;

	do {
		wrr_se = pick_next_wrr_entity(rq, wrr_rq);
		BUG_ON(!wrr_se);
        wrr_rq = NULL;
	} while (wrr_rq);

	p = wrr_task_of(wrr_se);
	p->se.exec_start = rq->clock_task;

	return p;
}

static struct task_struct *pick_next_task_wrr(struct rq *rq)
{
	struct task_struct *p = _pick_next_task_wrr(rq);

	/* The running task is never eligible for pushing */
	if (p)
		dequeue_pushable_task(rq, p);

	return p;
}

static void put_prev_task_wrr(struct rq *rq, struct task_struct *p)
{
	// printk("ENTER: put_prev_task_wrr\n");
	update_curr_wrr(rq);
	// printk("EXIT: put_prev_task_wrr\n");
}

static void set_curr_task_wrr(struct rq *rq)
{
	// printk("ENTER: set_curr_task_wrr\n");
	struct task_struct *p = rq->curr;

	p->se.exec_start = rq->clock_task;

	/* The running task is never eligible for pushing */
	dequeue_pushable_task(rq, p);
	// printk("EXIT: set_curr_task_wrr\n");
}

static void watchdog(struct rq *rq, struct task_struct *p)
{
	// printk("ENTER: watchdog\n");
	unsigned long soft, hard;

	/* max may change after cur was read, this will be fixed next tick */
	soft = task_rlimit(p, RLIMIT_RTTIME);
	hard = task_rlimit_max(p, RLIMIT_RTTIME);

	if (soft != RLIM_INFINITY) {
		unsigned long next;

		p->wrr.timeout++;
		next = DIV_ROUND_UP(min(soft, hard), USEC_PER_SEC/HZ);
		if (p->wrr.timeout > next)
			p->cputime_expires.sched_exp = p->se.sum_exec_runtime;
	}
	// printk("EXIT: watchdog\n");
}

static void task_tick_wrr(struct rq *rq, struct task_struct *p, int queued)
{
	// printk("ENTER: task_tick_wrr\n");
	struct sched_wrr_entity *wrr_se = &p->wrr;

	update_curr_wrr(rq);

	watchdog(rq, p);

	/*
	 * RR tasks need a special form of timeslice management.
	 * FIFO tasks have no timeslices.
	 */
	if (p->policy != SCHED_WRR)
		return;

	if (p->wrr.time_slice == 0)
	{
		if(find_group(p->sched_task_group) == 1)
			p->wrr.time_slice = WRR_TIMESLICE_FORE;// + ((MAX_PRIO - p->prio) * HZ /1000);
		else 
			p->wrr.time_slice = WRR_TIMESLICE_BACK;// + ((MAX_PRIO - p->prio) * HZ /10000);
	}

	if ((--p->wrr.time_slice) > 0)
	{
		printk("=====================Current time_slice: %u=====================\n",p->wrr.time_slice);
		printk("=====================Current prio: %d=====================\n",p->prio);
		printk("=====================Current MAX_PRIO: %d=====================\n",MAX_PRIO);
		++p->prio;
		if (p->prio > MAX_PRIO)
			p->prio = MAX_PRIO;
		return;
	}

    if(find_group(p->sched_task_group)==1) 
	{
		p->wrr.time_slice = WRR_TIMESLICE_FORE;// + ((MAX_PRIO - p->prio) * HZ /1000);
		printk("Time_slice updated to: %u\n",WRR_TIMESLICE_FORE);
	}
	else 
    {
		p->wrr.time_slice = WRR_TIMESLICE_BACK;// + ((MAX_PRIO - p->prio) * HZ /10000);
		printk("Time_slice updated to: %u\n",WRR_TIMESLICE_BACK);
	}    

	/*
	 * Requeue to the end of queue if we (and all of our ancestors) are the
	 * only element on the queue
	 */
	for_each_sched_wrr_entity(wrr_se) {
		if (wrr_se->run_list.prev != wrr_se->run_list.next) {
			requeue_task_wrr(rq, p, 0);
			set_tsk_need_resched(p);
			return;
		}
	}
}

void switched_from_wrr(){}

static unsigned int get_rr_interval_wrr(struct rq *rq, struct task_struct *task) // get time slice of wrr
{
	// printk("ENTER: get_rr_interval_wrr\n");
	if (task->policy == SCHED_WRR)
	{
		if(find_group(task->sched_task_group)==1)
			return WRR_TIMESLICE_FORE; // prio_wrr
		else
			return WRR_TIMESLICE_BACK; // prio_wrr
	}
	else
		return 0;
}

void prio_changed_wrr(){}

void switched_to_wrr(){}

const struct sched_class wrr_sched_class = {
	.next			= &fair_sched_class, /*Required*/
	.enqueue_task		= enqueue_task_wrr,/*Required*/ //OK
	.dequeue_task		= dequeue_task_wrr,/*Required*/ //OK
	.yield_task		= yield_task_wrr,/*Required*/ //OK

	.check_preempt_curr	= check_preempt_curr_wrr,/*Required*/ //OK

	.pick_next_task		= pick_next_task_wrr,/*Required*/ //OK
	.put_prev_task		= put_prev_task_wrr,/*Required*/ //OK

    
#ifdef CONFIG_SMP
    .task_fork          =task_fork_wrr,
	.select_task_rq		= select_task_rq_wrr,

	.set_cpus_allowed       = set_cpus_allowed_wrr,
	.rq_online              = rq_online_wrr,
	.rq_offline             = rq_offline_wrr
	.pre_schedule		= pre_schedule_wrr,
	.post_schedule		= post_schedule_wrr,
	.task_woken		= task_woken_wrr,
#endif
    .switched_from		= switched_from_wrr,
	.set_curr_task          = set_curr_task_wrr,/*Required*/ //OK
	.task_tick		= task_tick_wrr,/*Required*/

	.get_rr_interval	= get_rr_interval_wrr,

	.prio_changed		= prio_changed_wrr,
	.switched_to		= switched_to_wrr,
};