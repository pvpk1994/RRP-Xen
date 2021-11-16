/********** RRP-Xen V3.1 (Multi Core Multi VCPU per Domain) ************************************/

/* RRP-Xen Version 3.1
 * Author:: Pavan Kumar Paluri
 * Copyright 2017-2022 - RTLAB UNIVERSITY OF HOUSTON */

/*************************************************************************************************
 **************** LAST UPDATED ON November 16, 2021 ************************************************************************

 			***** FEATURES *****
 * Support for cpupool and standalone ARINC 653 VM scheduler.
 * Extended support for ARINC 653 periodic model resource partitioning algorithms.
 * Bug fixes and performance improvements to fecilitate a clean xl-destroy and xl cpupool-destroy.
 * TODO: Build a better interface to add support for numerous CPUs operating under RRP-Xen V3.1.
 * Support for cyclical addition of VCPUs to CPUs from a given domain, now available for RRP-Xen V3.1.
 * Integration of vcpu_remove into free_vdata to improve code readability.
 * VCPUs of all the domains operating under RRP-Xen V3.1 will now enter into a valid blocked (--b--) state on Domain creation.
 * Inclusion of per-cpu Locks to avoid list corruptions across the scheduler.
 * This scheduler works fine overall.
 * Fixed numerous bugs that were causing rrp-single core and rrp-multi core schedulers to reboot on xl-destroy

 * Concern: The schedule entry VCPU assignment before ps_sched_set() is problematic for destroying Domains and its VCPUs
 * Solution: Let the CPU add VCPUs of a newly created domain cyclically to its runQs, however do not invoke update_schedule_vcpus() yet.

 * Concern: Th current setting might create a problem for ps_sched_set() to destroy the VCPUs because of the update_schedule_vcpus().
 * Solution: TODO yet to be figured.

 *************************************************************************************************/

/****************** LOCKING MECHANISM ************************

 * Scheduler Lock (RUNQ Lock)
   - is per-RUNQ and there is 1 runQ per CPU
   - serializes all runQ manipulation operations

 * Private Data Lock (Private Scheduler Lock)
   - Serializes accesses to the scheduler global state (for CPUs 0 and 2)

 * Lock Ordering Mechanism:
   -----------------------
   - If we need both locks, we must acquire the private scheduler lock ALWAYS FIRST
   - If we already own a RUNQ lock, we must never acquire the private scheduler lock

 ***************************************************************/



#include <xen/config.h>
#include <xen/lib.h>
#include <xen/sched.h>
#include <xen/sched-if.h>
#include <xen/timer.h>
#include <xen/softirq.h>
#include <xen/time.h>
#include <xen/trace.h>
#include <xen/errno.h>
#include <xen/list.h>
#include <xen/guest_access.h>
#include <public/sysctl.h>
#include <public/domctl.h>

#define DOM0_TS MILLISECS(10)
#define IDLETASK(cpu)   (idle_vcpu[cpu])
#define PSVCPU(vc)      ((struct ps_vcpu_t *)(vc)->sched_priv)
#define SCHED_PRIV(s)   ((struct ps_priv_t *)((s)->sched_data))
#define print_time() ( printk("CPU_ID: %d, ms::micro_s:: %3ld.%3ld, %-19s ",smp_processor_id(), NOW()/MILLISECS(1),NOW()%MILLISECS(1)/1000, __func__) )
#define SCHED_PCPU(_cpu) ((struct ps_pcpu_t *)per_cpu(schedule_data, _cpu).sched_priv)
#define RUNQ(_cpu)          (&(SCHED_PCPU(_cpu)->runq))
#define MAX_PCPUS 4

#define RRP_DEBUG

static int index=0;

/***********************************
* Scheduler Customization Structures
***********************************/

struct ps_vcpu_t
{
	struct vcpu *vc;
	struct list_head list_elem;
	struct list_head runq_elem; /* RUNQ Element for RRP-CPU RUNQs */
	int cpu_rrp;
};

struct sched_entry_t
{
	xen_domain_handle_t dom_handle;
	int vcpu_id;
	s_time_t wcet;
	struct vcpu* vc;
};

struct ps_priv_t
{
	spinlock_t lock;
	struct sched_entry_t schedule[PS_MAX_DOMAINS_PER_SCHEDULE];
  	/***
   	 * Number of valid schedulable entries in ps_schedule_table
   	 * Num of schedulable_entires not necessarily be equal to number of domains
   	 * since, each domain can appear multiple times within the schedule
  	 ***/
  	int num_schedule_entries;
  	s_time_t hyperperiod;
  	s_time_t next_hyperperiod;
   	struct list_head vcpu_list;
   	uint32_t cpu_id;
};
static struct ps_priv_t rrp_lt[MAX_PCPUS];

struct ps_pcpu_t
{
	/* Per-cpu params */
	struct sched_entry_t schedule[PS_MAX_DOMAINS_PER_SCHEDULE];
	int num_schedule_entries;
	s_time_t hyperperiod;
	s_time_t next_hyperperiod;
	struct list_head runq;
	unsigned int nr_runnable;
	uint32_t runq_sort_last;
	spinlock_t pcpu_lock; /* Spin lock for Concurrent RUNQ Locks */
};

static int aaf_pick_cpu(const struct scheduler *, struct vcpu *);
static void init_pdata(struct ps_priv_t*, struct ps_pcpu_t*, int);

/********* PCPU-VCPU RUNQ HELPER FUNCTIONS *************/

static inline int __vcpu_on_runq(struct ps_vcpu_t *svc)
{
	return !list_empty(&svc->list_elem);
}

static inline struct ps_vcpu_t *__runq_elem(struct list_head *elem)
{
	return list_entry(elem, struct ps_vcpu_t, list_elem);
}

static inline bool_t is_runq_idle(unsigned int cpu)
{
	/*
	 * We're peeking at cpu's runq, we must hold the proper lock.
	 */
	ASSERT(spin_is_locked(per_cpu(schedule_data, cpu).schedule_lock));
	return list_empty(RUNQ(cpu)) ||
	is_idle_vcpu(__runq_elem(RUNQ(cpu)->next)->vc);
}

static inline void inc_nr_runnable(unsigned int cpu)
{
	ASSERT(spin_is_locked(per_cpu(schedule_data, cpu).schedule_lock));
	SCHED_PCPU(cpu)->nr_runnable++;
}

static inline void dec_nr_runnable(unsigned int cpu)
{
	ASSERT(spin_is_locked(per_cpu(schedule_data, cpu).schedule_lock));
	ASSERT(SCHED_PCPU(cpu)->nr_runnable >= 1);
	SCHED_PCPU(cpu)->nr_runnable--;
}

static inline void __runq_insert(struct ps_vcpu_t *svc)
{
	unsigned int cpu = svc->vc->processor;
	const struct list_head * const runq = RUNQ(cpu);
	struct list_head *iter;
	printk("inside __runq_insert on CPU %d\n",svc->vc->processor);
	list_add_tail(&svc->list_elem, iter);
}

static inline void runq_insert(struct ps_vcpu_t *svc)
{
	__runq_insert(svc);
}

static inline void __runq_remove(struct ps_vcpu_t *svc)
{
	BUG_ON( !__vcpu_on_runq(svc) );
	list_del_init(&svc->list_elem);
}

static inline void runq_remove(struct ps_vcpu_t *svc)
{
	dec_nr_runnable(svc->vc->processor);
	__runq_remove(svc);
}

/* **********************************
  * Helper Functions
  * Compare domain handles of 2 xen_domain_handle_t's
  **********************************
  * >0 : h1 > h2
  * =0 : h1 == h2
  * <0 : h1 < h2
*/

static int dom_handle_cmp(const xen_domain_handle_t h1,
			  const xen_domain_handle_t h2)
{
	return memcmp(h1, h2, sizeof(xen_domain_handle_t));
}

static int dom_comp(s_time_t wcet1, s_time_t wcet2)
{
	return wcet1 > wcet2 ? 0 : 1;
}


/* Find a vcpu based on VCPU-ID and Domain-Handle for non-sched-set function */
static struct vcpu* find_vcpu(const struct scheduler *ops, xen_domain_handle_t h, int vcpu_id, int cpu_id)
{
	struct ps_vcpu_t *ps_vcpu;
	printk("Control in find_vcpu_sched_set\n");
	list_for_each_entry(ps_vcpu, &SCHED_PRIV(ops)->vcpu_list, list_elem)
	if(dom_handle_cmp(ps_vcpu->vc->domain->handle,h) == 0 &&
		(vcpu_id == ps_vcpu->vc->vcpu_id))
		{
			/* If the control enters the if conditional, the vc->processor always points to the cpu
			   in execution under aaf_pool
			 */
			printk("find_vcpu: DOMAIN HANDLE: %X\n",*ps_vcpu->vc->domain->handle);
			printk("find_vcpu: CPUID is: %d\n", ps_vcpu->vc->processor);
			return ps_vcpu->vc;
		}
	return NULL;
}

static void update_schedule_vcpus(const struct scheduler *ops, int cpu_id)
{
	unsigned int i;
	int num_entries = SCHED_PRIV(ops)->num_schedule_entries;
	printk("UPDATE_SCHED_VCPUS: num_schedule_entries: %d\n", SCHED_PRIV(ops)->num_schedule_entries);
	for(i=0; i<num_entries; i++)
	{
		printk("UPDATE_SCHED_VCPU: Domain Handle: %X\n", *SCHED_PRIV(ops)->schedule[i].dom_handle);
		SCHED_PRIV(ops)->schedule[i].vc = find_vcpu(ops,SCHED_PRIV(ops)->schedule[i].dom_handle,
			SCHED_PRIV(ops)->schedule[i].vcpu_id, cpu_id);
	}
}

static struct vcpu* find_vcpu_pcpu(int cpu_id, xen_domain_handle_t h, int vcpu_id)
{
	struct ps_vcpu_t *ps_vcpu;
	printk("in find_vcpu_pcpu fn\n");
	list_for_each_entry(ps_vcpu, &SCHED_PCPU(cpu_id)->runq, runq_elem)
	if(dom_handle_cmp(ps_vcpu->vc->domain->handle, h) == 0 &&
		(vcpu_id == ps_vcpu->vc->vcpu_id))
	{
		printk("find_vcpu_pcpu in if, CPU_ID: %d\n", ps_vcpu->vc->processor);
		return ps_vcpu->vc;
	}
	return NULL;
}

static void update_vcpu_pcpu(int cpu_id)
{
	unsigned int i;
	int num_entries = SCHED_PCPU(cpu_id)->num_schedule_entries;
	printk("UPDATE_VCPU_PCPU: num_schedule_entries: %d\n", SCHED_PCPU(cpu_id)->num_schedule_entries);
	for(i=0; i<num_entries; i++)
	{
		printk("UPDATE_VCPU_PCPU: Domain Handle: %X\n", *SCHED_PCPU(cpu_id)->schedule[i].dom_handle);
		SCHED_PCPU(cpu_id)->schedule[i].vc = find_vcpu_pcpu(cpu_id, SCHED_PCPU(cpu_id)->schedule[i].dom_handle,
				SCHED_PCPU(cpu_id)->schedule[i].vcpu_id);
  	}
}

static void del_vcpu_pcpu(const struct scheduler *ops,int cpu_id)
{
	unsigned int i;
	unsigned long flags;
	int num_entries = SCHED_PCPU(cpu_id)->num_schedule_entries;
	printk("UPDATE_DEL_VCPU_PCPU: num_sched_entries: %d\n", num_entries);
	for(i =0; i<num_entries; i++)
	{
		printk("UPDATE_DEL_VCPU_PCPU: Domain Handle: %X\n", *SCHED_PCPU(cpu_id)->schedule[i].dom_handle);
				SCHED_PCPU(cpu_id)->schedule[i].vc = NULL;
	}
}

static int ps_sched_set(const struct scheduler *ops, struct xen_sysctl_aaf_schedule *schedule)
{
	printk("Entering schedule_set AAF\n");
	int cpu = smp_processor_id();
	printk("cpu ID in ps_sched_set is: %d\n", cpu);
	struct ps_priv_t *sched_priv = SCHED_PRIV(ops);
	struct ps_pcpu_t *sched_pcpu = SCHED_PCPU(schedule->cpu_id);
	unsigned int i;
	unsigned long flags;
	s_time_t wcet_total =0;
	int rc = -EINVAL;

  	if(schedule->hyperperiod <=0)
	{
		printk("Allocation Failure\n");
		goto dump;
	}

	if( schedule->num_schedule_entries <1 )
	{
		printk("Allocation Failure\n");
		goto dump;
	}

	for(i=0; i < schedule->num_schedule_entries; i++)
	{
		if(schedule->schedule[i].wcet <= 0)
		{
			printk("WCET is less than 0!!\n");
			goto dump;
		}
	}

	sched_priv->num_schedule_entries = schedule->num_schedule_entries;
	sched_pcpu->num_schedule_entries = schedule->num_schedule_entries;
	sched_priv->hyperperiod = schedule->hyperperiod;
 	sched_pcpu->hyperperiod = schedule->hyperperiod;
	sched_priv->cpu_id = schedule->cpu_id;

	for(i=0 ;i< schedule->num_schedule_entries; i++)
	{
		memcpy(sched_priv->schedule[i].dom_handle, schedule->schedule[i].dom_handle, sizeof(schedule->schedule[i].dom_handle));
		sched_priv->schedule[i].vcpu_id = schedule->schedule[i].vcpu_id;
		sched_priv->schedule[i].wcet = schedule->schedule[i].wcet;

		memcpy(sched_pcpu->schedule[i].dom_handle, schedule->schedule[i].dom_handle, sizeof(schedule->schedule[i].dom_handle));
		sched_pcpu->schedule[i].vcpu_id = schedule->schedule[i].vcpu_id;
		sched_pcpu->schedule[i].wcet = schedule->schedule[i].wcet;
	}

	printk("Memcpy and import of other params of sched_entries done\n");

	/* RRP-CPUs ZONE */
	if(schedule->cpu_id == 1 || schedule->cpu_id == 3 || schedule->cpu_id == 5)
	{
		update_vcpu_pcpu(schedule->cpu_id);

	}

	else
		update_schedule_vcpus(ops, smp_processor_id());

	sched_pcpu->next_hyperperiod = NOW();
	sched_priv->next_hyperperiod = NOW();
	rc =0;

	if(schedule->cpu_id == 1 || schedule->cpu_id == 3 || schedule->cpu_id == 5)
	{
		return rc;
	}

dump:
	printk("Control going to Dump Failure...\n");
	spin_unlock_irqrestore(&sched_priv->lock,flags);
	return rc;
}

static int ps_sched_get(const struct scheduler *ops, struct xen_sysctl_aaf_schedule *schedule)
{
	struct ps_priv_t *sched_priv = SCHED_PRIV(ops);
	unsigned int i;
	unsigned long flags;
	spin_lock_irqsave(&sched_priv->lock, flags);
	schedule->num_schedule_entries = sched_priv->num_schedule_entries;
	schedule->hyperperiod = sched_priv->hyperperiod;

	for(i=0; i< sched_priv->num_schedule_entries; i++)
	{
		memcpy(schedule->schedule[i].dom_handle,sched_priv->schedule[i].dom_handle,
			sizeof(sched_priv->schedule[i].dom_handle));
		schedule->schedule[i].vcpu_id = sched_priv->schedule[i].vcpu_id;
		schedule->schedule[i].wcet = sched_priv->schedule[i].wcet;
	}

	spin_unlock_irqrestore(&sched_priv->lock, flags);
	return 0;
}

struct sched_entry_t *dom_wcet_comp(const struct scheduler *ops)
{
	struct ps_priv_t *sched_priv = SCHED_PRIV(ops);
	int i,j;
	for(i=0; i< sched_priv->num_schedule_entries; i++)
	{
		for(j=i+1; j< sched_priv->num_schedule_entries; j++)
		{
			int k;
			k = dom_comp(sched_priv->schedule[i].wcet, sched_priv->schedule[j].wcet);
			if (k != 0)
			{
				/* swap the contents to sort it in ascending order of wcet(s) */
				int temp;
				temp = sched_priv->schedule[i].wcet;
				sched_priv->schedule[i].wcet = sched_priv->schedule[j].wcet;
				sched_priv->schedule[j].wcet = temp;
			}
		}
	}

	return sched_priv->schedule;
}

/********************************************
 * SCHEDULER CALLBACK FUNCTIONS
 ********************************************/

static int aafsched_init(struct scheduler *ops)
{
	struct ps_priv_t *sched_priv;
	printk("Entering sched_init\n");
	sched_priv = xzalloc(struct ps_priv_t);

	if(sched_priv == NULL)
		return -ENOMEM;

	ops->sched_data = sched_priv;
	sched_priv->next_hyperperiod = 0;
	spin_lock_init(&sched_priv->lock);
	INIT_LIST_HEAD(&sched_priv->vcpu_list);
	printk("Leaving sched_init\n");
	return 0;
}

static void aafsched_deinit(struct scheduler *ops)
{
	printk("Entering sched_deinit\n");
	xfree(SCHED_PRIV(ops));
	ops->sched_data = NULL;
	printk("Leaving sched_deinit\n");
}

static void* aafsched_alloc_vdata(const struct scheduler *ops, struct vcpu *vc, void *dd)
{
	struct ps_priv_t *sched_priv = SCHED_PRIV(ops);
	struct ps_vcpu_t *avc;
	unsigned int entry, entry_rrp;
	unsigned long flags, flags_pcpu;
	spinlock_t *lock;
	avc = xmalloc(struct ps_vcpu_t);

	if(avc == NULL)
		return NULL;

	INIT_LIST_HEAD(&avc->runq_elem);
	avc->cpu_rrp = -1;
	spin_lock_irqsave(&sched_priv->lock, flags);
	printk("Inside alloc_vdata Function...\n");

	if(vc->domain->domain_id == 0)
	{
		avc->cpu_rrp = -1;
		printk("alloc_vdata being invoked for Dom0's VCPU #: %d\n",vc->vcpu_id);
		entry = sched_priv->num_schedule_entries;

		if(entry < PS_MAX_DOMAINS_PER_SCHEDULE)
		{
			sched_priv->schedule[entry].dom_handle[0] = '\0';
			sched_priv->schedule[entry].vcpu_id = vc->vcpu_id;
			sched_priv->schedule[entry].wcet = DOM0_TS;
			sched_priv->schedule[entry].vc = vc;
			sched_priv->hyperperiod += DOM0_TS;
			++(sched_priv->num_schedule_entries);
		}
	}

	avc->vc = vc;

	if(!is_idle_vcpu(vc) && (vc->processor != 1 && vc->processor != 3 && vc->processor != 5))
	{
		avc->cpu_rrp = -2;
		list_add(&avc->list_elem, &SCHED_PRIV(ops)->vcpu_list);
	}

	update_schedule_vcpus(ops, smp_processor_id());
	printk("About to leave alloc_vdata function...\n");
	spin_unlock_irqrestore(&sched_priv->lock, flags);
	return avc;
}

/********* ASSISTANCE FN FOR FREE_VDATA ************
 * @param: ops: Pointer to the system wide priv
 * @param: priv: void pointer ready to be typecast
 * *************************************************/

static void dom_zero_free_vdata(const struct scheduler *ops, void *priv)
{
	struct ps_vcpu_t *pvcpu = priv;
	printk("dom_zero_free_vdata invoked\n");

	if(pvcpu == NULL)
		return;

	if(!is_idle_vcpu(pvcpu->vc))
		list_del(&pvcpu->list_elem);

	xfree(pvcpu);
	update_schedule_vcpus(ops, 1);
	printk("leaving dom_zero_free_vdata\n");
}

static void aaf_free_vdata(const struct scheduler *ops, void *priv)
{
	struct ps_vcpu_t *pvcpu = priv;
	int cpu_id;
	printk("aaf_free_vdata invoked\n");

	if(pvcpu == NULL)
		return;

	if(is_idle_vcpu(pvcpu->vc))
	{
		dom_zero_free_vdata(ops, priv);
		return;
	}

	else
	{
		/* If current vcpu is a RRP-cpu, then do a list_del on per-cpu RunQ */
		if(pvcpu->cpu_rrp > 0)
		{
			cpu_id = pvcpu->cpu_rrp;
			list_del(&pvcpu->runq_elem);
			xfree(pvcpu);
			update_vcpu_pcpu(cpu_id);
			return;
		}

		/* Else, we know this vcpu is a Non-RRP-cpu, then simply do a list_del on global RunQ */
		else if(pvcpu->cpu_rrp == -2)
		{
			dom_zero_free_vdata(ops, priv);
		}
	}
}

static void aaf_vcpu_sleep(const struct scheduler *ops, struct vcpu *vc)
{

	/* If by any chance, the VCPU being put to sleep is the same as one that is currently running,
	 * raise a Soft interrupt request to let scheduler know to switch domains */
	if(per_cpu(schedule_data, vc->processor).curr == vc)
		cpu_raise_softirq(vc->processor, SCHEDULE_SOFTIRQ);
}

static void aaf_vcpu_wake(const struct scheduler *ops, struct vcpu *vc)
{
	cpu_raise_softirq(vc->processor, SCHEDULE_SOFTIRQ);
}

static void  *aaf_vcpu_insert(const struct scheduler *ops, struct vcpu *vc)
{
	struct ps_vcpu_t *pvcpu = vc->sched_priv;
	struct ps_priv_t *sched_priv = SCHED_PRIV(ops);
	spinlock_t *lock;
	unsigned int entry;
	vc->processor = aaf_pick_cpu(ops, vc);
	lock = vcpu_schedule_lock_irq(vc);
	entry = SCHED_PCPU(vc->processor)->num_schedule_entries;
	struct ps_pcpu_t *ps_cpu = SCHED_PCPU(vc->processor);

	if(vc->domain->domain_id !=0 && vc->processor == 1 || vc->processor == 3 || vc->processor == 5 && !is_idle_vcpu(vc) )
	{
		if(entry < 512)
		{
			memcpy(ps_cpu->schedule[entry].dom_handle, pvcpu->vc->domain->handle, sizeof(pvcpu->vc->domain->handle));
			ps_cpu->schedule[entry].vcpu_id = pvcpu->vc->vcpu_id;
			ps_cpu->schedule[entry].wcet = DOM0_TS;
			ps_cpu->schedule[entry].vc = vc;
			ps_cpu->hyperperiod +=  DOM0_TS;
			++(ps_cpu->num_schedule_entries);
		}
	}

	printk("Function: %s  Processor: %d  #schedule-entries: %d\n",__func__, vc->processor, SCHED_PCPU(vc->processor)->num_schedule_entries);
	pvcpu->cpu_rrp = vc->processor;
	list_add(&pvcpu->runq_elem, &SCHED_PCPU(vc->processor)->runq);
	printk("Function: %s VCPU: %d from Domain: %X added to CPU: %d\n", __func__, vc->vcpu_id, vc->domain->handle,vc->processor);
	vcpu_schedule_unlock_irq(lock, vc);
}

static struct task_slice ps_rrp_do_schedule(const struct scheduler *ops, s_time_t now, bool_t tasklet_work_scheduled)
{
	struct task_slice ret;
	struct vcpu *new_task = NULL;
	static unsigned int sched_index = 0;
	static s_time_t next_switch_time;
	struct ps_priv_t *sched_private = SCHED_PRIV(ops);
	unsigned int cpu = smp_processor_id();
	unsigned long flags;

	/* TRACE */
	if( unlikely(tb_init_done))
	{
		struct
		{
			unsigned cpu:16, tasklet:8, idle:8;
		}d;
	d.cpu = smp_processor_id();
	d.tasklet = tasklet_work_scheduled;
	d.idle = is_idle_vcpu(current);
	}

	/* RRP-MULTICORE IMPLEMENTATION ZONE
	 * --------------------------------- */
	if(cpu == 1 || cpu == 3 || cpu== 5)
	{
		struct ps_pcpu_t *sched_pcpu = SCHED_PCPU(cpu);
		if(sched_pcpu->num_schedule_entries < 1)
			sched_pcpu->next_hyperperiod = now + MILLISECS(30);

		else if(now >= sched_pcpu->next_hyperperiod)
		{
			sched_index = 0;
			sched_pcpu->next_hyperperiod = now + sched_pcpu->hyperperiod;
			next_switch_time = now + sched_pcpu->schedule[0].wcet;
		}

		else
		{
			while((now >= next_switch_time) && (sched_index < sched_pcpu->num_schedule_entries))
			{
				sched_index++;
				next_switch_time += sched_pcpu->schedule[sched_index].wcet;
			}
		}

		if(sched_index >= sched_pcpu->num_schedule_entries)
			next_switch_time = sched_pcpu->next_hyperperiod;

		new_task = (sched_index < sched_pcpu->num_schedule_entries) ? sched_pcpu->schedule[sched_index].vc : IDLETASK(cpu);

		if(!((new_task!= NULL) && (vcpu_runnable(new_task)) && PSVCPU(new_task)))
			new_task = IDLETASK(cpu);

		BUG_ON(new_task == NULL);
		BUG_ON(now >= sched_pcpu->next_hyperperiod);

		if(tasklet_work_scheduled)
			new_task = IDLETASK(cpu);

		if(!is_idle_vcpu(new_task) && (new_task->processor != cpu))
			new_task = IDLETASK(cpu);

		ret.time = next_switch_time - now;
		ret.task = new_task;
		ret.migrated = 0;

		return ret;
	}

	/* RRP-SINGLE CORE IMPLEMENTATION ZONE
	 * ----------------------------------- */
	else
	{
		spin_lock_irqsave(&sched_private->lock, flags);

		if(sched_private->num_schedule_entries < 1)
			sched_private->next_hyperperiod = now + MILLISECS(30);

		else if (now >= sched_private->next_hyperperiod)
		{
			sched_index = 0;
			sched_private->next_hyperperiod = now + sched_private->hyperperiod;
			next_switch_time = now+sched_private->schedule[0].wcet;
		}

		else
		{
			while(now >= next_switch_time && sched_index < sched_private->num_schedule_entries)
			{
				sched_index++;
				next_switch_time += sched_private->schedule[sched_index].wcet;
			}
		}

		/* Boundary conditions checking phase */
		if(sched_index >= sched_private->num_schedule_entries)
		{
			next_switch_time = sched_private->next_hyperperiod;
		}

		/* If current task is done with it's execution, point the schedule to next vcpu structure in schedule *ops,
		 * OR if we have exhausted all schedule entries within that hyperperiod, run idle timeslices until beginning
		 * of next hyperperiod
		 */
		new_task = (sched_index < sched_private->num_schedule_entries) ?sched_private->schedule[sched_index].vc
				: IDLETASK(cpu);

		if(!((new_task!= NULL) && (vcpu_runnable(new_task))&& PSVCPU(new_task)))
			new_task = IDLETASK(cpu);

		BUG_ON(new_task == NULL);
		BUG_ON(now >= sched_private->next_hyperperiod);
		spin_unlock_irqrestore(&sched_private->lock, flags);

		if(tasklet_work_scheduled)
			new_task = IDLETASK(cpu);

		if(!is_idle_vcpu(new_task) && (new_task->processor != cpu))
			new_task = IDLETASK(cpu);

		if(new_task->domain->domain_id != 0 && new_task->domain->domain_id !=32767)
		{
			ret.time = next_switch_time - now;
			ret.task = new_task;
			ret.migrated = 0;
			return ret;
		}

		else
		{
			ret.time = next_switch_time - now;
			ret.task = new_task;
			ret.migrated = 0;
			return ret;
		}

	}
}

static int test_adjust_global(const struct scheduler *ops, struct xen_sysctl_scheduler_op *sc)
{
	struct xen_sysctl_aaf_schedule local_sched;
	int rc=-EINVAL;

	switch(sc->cmd)
	{
		case XEN_SYSCTL_SCHEDOP_putinfo:
			if(copy_from_guest(&local_sched, sc->u.sched_aaf.schedule, 1))
			{
				rc=-EINVAL;
				break;
			}
			rc = ps_sched_set(ops, &local_sched);
			printk("Sched_set invoked in test_adjust_gloabl\n");
			printk("Index of the LT: %d\n",index);
			index++;
			break;
	}
	return rc;
}

static void aaf_switch_sched(struct scheduler *ops,unsigned int cpu, void *pdata, void* vdata)
{
	struct schedule_data *sd = &per_cpu(schedule_data, cpu);
	struct ps_priv_t *prv = SCHED_PRIV(ops);
	struct ps_vcpu_t *pvc = vdata;
	printk("Switching schedule using aaf_switch_sched on pcpu: %d\n", cpu);
	idle_vcpu[cpu]->sched_priv = vdata;
	spin_lock(&prv->lock);
	init_pdata(prv, pdata, cpu);
	spin_unlock(&prv->lock);
	per_cpu(scheduler, cpu) = ops;
	per_cpu(schedule_data, cpu).sched_priv = pdata; /* pdata customization for our scheduler */
	printk("Leaving aaf_switch_sched\n");
	smp_mb();
	sd->schedule_lock = &sd->_lock;
}

static int aaf_pick_cpu(const struct scheduler *ops, struct vcpu *vc)
{
	cpumask_t *online;
	cpumask_t cpus;
	unsigned int cpu;
	online = cpupool_domain_cpumask(vc->domain);
	cpumask_and(&cpus, online, vc->cpu_hard_affinity);
	cpu = cpumask_test_cpu(vc->processor, &cpus) ? vc->processor : cpumask_cycle(vc->processor, &cpus);

	ASSERT( !cpumask_empty(&cpus) && cpumask_test_cpu(cpu, &cpus) );

	printk("%s invoked for Domain's: %X VCPU: %d on CPU: %d \n",__func__, vc->domain->handle, vc->vcpu_id, cpu);
	return cpu;
}

static void* ps_alloc_pdata(const struct scheduler *ops, int cpu)
{
	struct ps_pcpu_t *psc;
	struct ps_priv_t *ps_private = SCHED_PRIV(ops);
	printk("Entering ps_alloc_pdata\n");
	psc = xzalloc(struct ps_pcpu_t);

	if(psc == NULL)
		return ERR_PTR(-ENOMEM);

	if(per_cpu(schedule_data, cpu).sched_priv == NULL)
		per_cpu(schedule_data, cpu).sched_priv = psc;

	INIT_LIST_HEAD(&psc->runq);
	psc->next_hyperperiod = 0;
	spin_lock_init(&psc->pcpu_lock);
	printk("Leaving ps_alloc_pdata\n");
	return psc;
}

#ifdef RRP_DEBUG

static void init_pdata(struct ps_priv_t *priv, struct ps_pcpu_t *psc, int cpu)
{
	ASSERT(spin_is_locked(&priv->lock));
	INIT_LIST_HEAD(&psc->runq);
	spin_lock_init(&psc->pcpu_lock);
}

static void ps_init_pdata(const struct scheduler *ops, void *pdata, int cpu)
{
	unsigned long flags;
	struct ps_priv_t *prv = SCHED_PRIV(ops);
	struct schedule_data *pc = &per_cpu(schedule_data, cpu);

	/* Locking - Just as per credit scheduler */
	ASSERT(pc->schedule_lock == &pc->_lock && !spin_is_locked(&pc->_lock));

	printk("Entering init_pdata\n");
	spin_lock_irqsave(&prv->lock, flags);
	init_pdata(prv, pdata, cpu);
	spin_unlock_irqrestore(&prv->lock, flags);
	printk("Leaving init_pdata\n");
}

#endif

static void ps_free_pdata(const struct scheduler *ops, void *pcpu, int cpu)
{
	printk("Entering ps_free_pdata\n");
	struct ps_pcpu_t *spc = pcpu;

	if(!spc)
	{
		printk("RRP-Xen: NULL PCPU provided: %d\n", cpu);
		return;
	}

	xfree(spc);
	printk("Leaving ps_free_pdata\n");
}

static void _vcpu_remove(struct ps_priv_t *prv, struct vcpu *v)
{
	unsigned int bs;
	unsigned int cpu = v->processor;
 	struct ps_vcpu_t *ps_vc;
}

static void rrp_vcpu_remove(const struct scheduler *ops, struct vcpu *v)
{
	printk("Entering rrp_vcpu_remove \n");
	struct ps_priv_t *ps_priv = SCHED_PRIV(ops);
	struct ps_vcpu_t *ps_vcpu = PSVCPU(v);
	int cpu_id = v->processor;
	spinlock_t *lock;

	if(cpu_id == 1 || cpu_id == 3 || cpu_id == 5)
	{
		printk("list_del_init to take place on CPU#: %d\n",cpu_id);
		list_del(&ps_vcpu->runq_elem);
	}

	printk("Leving rrp_vcpu remove \n");
}


/************* RRP-XEN V3.1 SCHEDULER HOOKS ********************/
const struct scheduler sched_aaf_def =
{
	.name = "AAF Single Scheduler",
	.opt_name = "aaf",
	.sched_id = XEN_SCHEDULER_AAF,
	.sched_data = NULL,

	.init           = aafsched_init,
	.deinit         = aafsched_deinit,

	.alloc_pdata    = ps_alloc_pdata,
	.free_pdata     = ps_free_pdata,
	.init_pdata     = NULL,
	.deinit_pdata   = NULL,

	.alloc_domdata  = NULL,
	.free_domdata   = NULL,

	.switch_sched   = aaf_switch_sched,

	.insert_vcpu    = aaf_vcpu_insert,
	.remove_vcpu    = NULL,
	.alloc_vdata    = aafsched_alloc_vdata,
	.free_vdata     = aaf_free_vdata,

	.adjust         = NULL,
	.adjust_global  = test_adjust_global,
	.pick_cpu       = aaf_pick_cpu,

	.do_schedule    = ps_rrp_do_schedule,

	.sleep          = aaf_vcpu_sleep,
	.wake           = aaf_vcpu_wake,

	.context_saved  = NULL,
	.yield          = NULL,
	.migrate        = NULL,
};

REGISTER_SCHEDULER(sched_aaf_def);


