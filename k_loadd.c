/*
 * k_loadd.c - calculate load average from a workqueue and do something.
 *  
 * I build this on 3.10.0-1160.el7.x86_64
 */

#include <linux/module.h> /* Needed by all modules */
#include <linux/printk.h> /* Needed for pr_info() */
#include <linux/sched.h>  /* needed for FIXED_1 */


#define LOAD_INT(x) ((x) >> FSHIFT)
#define LOAD_FRAC(x) LOAD_INT(((x) & (FIXED_1-1)) * 100)


extern void msleep(unsigned int msecs); // not needed. was testing msleep() earlier.

void loadavg_workqueue_fn(struct work_struct *work);
static struct workqueue_struct *loadavg_workqueue;
DECLARE_WORK(work, loadavg_workqueue_fn);

struct timer_list loadavg_timer;


int WORK_ITS; // iterations that loadavg_workqueue_fn() has run.

long read_loadavg[3]; // store 1,5,15 minute loads at [0] [1] [2],

int msecs_between = 1000; // number of milliseconds between iterations





/* 
 * This routine was stolen from get_avenrun() in the kernel. 
 * get_avenrun() is not exported and therefore shouldn't be accessed from
 * a module. Putting it in this module makes it easier to calculate the 
 * load averages. Pasting get_avenrun() below:  

   2730 void get_avenrun(unsigned long *loads, unsigned long offset, int shift)
   2731 {  
   2732         loads[0] = (avenrun[0] + offset) << shift;
   2733         loads[1] = (avenrun[1] + offset) << shift;
   2734         loads[2] = (avenrun[2] + offset) << shift;
   2735 }  
 */

void calc_loads(unsigned long *loads, unsigned long offset, int shift) {
    loads[0] = (avenrun[0] + offset) << shift; // 1 minute
    loads[1] = (avenrun[1] + offset) << shift; // 5 minute
    loads[2] = (avenrun[2] + offset) << shift; // 15 minute
}


void loadavg_workqueue_fn(struct work_struct *work) {
    unsigned long averages_returned[3];

    char *print_avgs[] = {"1 Avg", "5 Avg", "15 Avg"}; 

    pr_info("Attempting to read the load averages, iteration: %i\n", WORK_ITS);
    calc_loads(averages_returned, FIXED_1/200, 0);
    pr_info("Printing loads.\n");
    int i;
    for (i = 0; 3 > i; i++) {
        pr_info("%s: %lu.%02lu\n", print_avgs[i], LOAD_INT(averages_returned[i]), LOAD_FRAC(averages_returned[i]));
    }
    WORK_ITS++;
}


void loadavg_timer_fn(unsigned long data) {
    // have timer fire at current jiffies + milliseconds delay (in jiffies).
    mod_timer(&loadavg_timer, jiffies + msecs_to_jiffies(msecs_between));
    // add the load avg calculation work to the workqueue
    queue_work(loadavg_workqueue, &work);
}



int init_module(void) {
    pr_info("Loading the kernel-space load average reader.\n");
    
    WORK_ITS = 0; // just record the number of iterations the loadavg_workqueue_fn() runs.

    pr_info("Creating workqueues.\n");
    // create the loadavg_wq thread
    loadavg_workqueue = create_workqueue("loadavg_wq");

    pr_info("Creating timer.\n");
    // setup the timer 
    setup_timer(&loadavg_timer, loadavg_timer_fn, 0);

    // kick off the first iteration of the timer.
    // This will load have the timer scheduled for the next iteration
    // and also proceed with doing the first load calculation.
    loadavg_timer_fn(0);

    // int timer_is_valid;
    // timer_is_valid = 

    // non 0 return means there was a failure. Return 0 for success.
    return 0;
}



void cleanup_module(void) {
    // delete the timer. 
    del_timer(&loadavg_timer);
    // delete the workqueue
    destroy_workqueue(loadavg_workqueue);
    pr_info("k_loadd.ko unloading..\n");
}

MODULE_LICENSE("GPL");
