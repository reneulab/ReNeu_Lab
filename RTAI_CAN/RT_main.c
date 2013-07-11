/*************************************************/
#include "limb.h"
#include "myCan.h"
#include <stdio.h>
#include <unistd.h>

/************************************************/
// RTAI dependencies 
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/errnoh.>
#include "rtai.h"
#include "rtai_sched.h"
#include <rtai_sem.h>

MODULE_LICENSE("GPL");
/***********************************************/

static RT_TASK myTask;

void myTask(long arg) 
{
  NTCAN_HANDLE  thumb_H;
  NTCAN_HANDLE	index_H;
  NTCAN_HANDLE	middle_H;
  NTCAN_HANDLE	ring_H;
  NTCAN_HANDLE	pinkie_H;
   
  int32_t	result;
  command 	thumb_cmd, index_cmd, middle_cmd, ring_cmd, pinkie_cmd;
  int32_t   i;
	 
// Open can
	thumb_H = initLimb(thumb,&thumb_cmd);
	if(thumb_H == 0xFFFF) 
		{ rt_printk("Error in thumb init\n"); return 1; } 
	usleep(20000);
 
	index_H = initLimb(index,&index_cmd);
	if(index_H == 0xFFFF) 
		{ rt_printk("Error in index init\n"); return 1; } 
	usleep(20000);
 
	middle_H = initLimb(middle,&middle_cmd);
	if(middle_H == 0xFFFF) 
		{ rt_printk("Error in middle init\n"); return 1; } 
	usleep(20000);
 
	ring_H = initLimb(ring,&ring_cmd);
	if(ring_H == 0xFFFF) 
		{ rt_printk("Error in ring init\n"); return 1; } 
	usleep(20000);
 
	pinkie_H = initLimb(pinkie,&pinkie_cmd);
	if(pinkie_H == 0xFFFF) 
		{ rt_printk("Error in pinkie init\n"); return 1; } 
	usleep(20000); 
// Running limb hand commands
// Writing and Reading //
	rt_printk("Closing\n");
	thumb_cmd.speed = 690;
	index_cmd.speed = 690;
	middle_cmd.speed = 690;
	ring_cmd.speed = 690;
	pinkie_cmd.speed = 690;

	thumb_cmd.mode = l_close;
	index_cmd.mode = l_close;
	middle_cmd.mode = l_close;
	ring_cmd.mode = l_close;
	pinkie_cmd.mode = l_close;

	rt_printk("******THUMB******\n");
	result = writeLimb(thumb_H,&thumb_cmd);
	usleep(20000);
	result = readLimb(thumb_H,&thumb_cmd);
	sleep(1);

	rt_printk("******INDEX******\n");
	result = writeLimb(index_H,&index_cmd);
	usleep(20000);
	result = readLimb(index_H,&index_cmd);
	sleep(1);

	rt_printk("******MIDDLE******\n");
	result = writeLimb(middle_H,&middle_cmd);
	usleep(20000);
	result = readLimb(middle_H,&middle_cmd);
	sleep(1);

	rt_printk("******RING******\n");
	result = writeLimb(ring_H,&ring_cmd);
	usleep(20000);
	result = readLimb(ring_H,&ring_cmd);
	sleep(1);

	rt_printk("******PINKIE******\n"); 
	result = writeLimb(pinkie_H,&pinkie_cmd);
	usleep(20000);
	result = readLimb(pinkie_H,&pinkie_cmd);
	sleep(1);

	sleep(1); 
	thumb_cmd.mode = l_open;
	index_cmd.mode = l_open;
	middle_cmd.mode = l_open;
	ring_cmd.mode = l_open;
	pinkie_cmd.mode = l_open;
	
	rt_printk("******PINKIE******\n");
	result = writeLimb(pinkie_H,&pinkie_cmd);
	usleep(20000);
	result = readLimb(pinkie_H,&pinkie_cmd);
	sleep(1);

	rt_printk("******RING******\n");
	result = writeLimb(ring_H,&ring_cmd);
	usleep(20000);
	result = readLimb(ring_H,&ring_cmd);
	sleep(1);

	rt_printk("******MIDDLE******\n");
	result = writeLimb(middle_H,&middle_cmd);
	usleep(20000);
	result = readLimb(middle_H,&middle_cmd);
	sleep(1);

	rt_printk("******INDEX******\n");
	result = writeLimb(index_H,&index_cmd);
	usleep(20000);
	result = readLimb(index_H,&index_cmd);
	sleep(1);

	rt_printk("******THUMB******\n");
	result = writeLimb(thumb_H,&thumb_cmd);
	usleep(20000);
	result = readLimb(thumb_H,&thumb_cmd);
	sleep(1);

	sleep(1); 
	thumb_cmd.mode = l_stop;
	index_cmd.mode = l_stop;
	middle_cmd.mode = l_stop;
	ring_cmd.mode = l_stop;
	pinkie_cmd.mode = l_stop;

	rt_printk("******THUMB******\n");
	result = writeLimb(thumb_H,&thumb_cmd);
	usleep(20000);
	result = readLimb(thumb_H,&thumb_cmd);
	usleep(20000);

	rt_printk("******INDEX******\n");
	result = writeLimb(index_H,&index_cmd);
	usleep(20000);
	result = readLimb(index_H,&index_cmd);
	usleep(20000);

	rt_printk("******MIDDLE******\n");
	result = writeLimb(middle_H,&middle_cmd);
	usleep(20000);
	result = readLimb(middle_H,&middle_cmd);
	usleep(20000);

	rt_printk("******RING******\n");
	result = writeLimb(ring_H,&ring_cmd);
	usleep(20000);
	result = readLimb(ring_H,&ring_cmd);
	usleep(20000);

	rt_printk("******PINKIE******\n"); 
	result = writeLimb(pinkie_H,&pinkie_cmd);
	usleep(20000);
	result = readLimb(pinkie_H,&pinkie_cmd);
	usleep(20000);
// Closing //  
	closeLimb(thumb_H,thumb); 
	closeLimb(index_H,index); 
	closeLimb(middle_H,middle); 
	closeLimb(ring_H,ring); 
	closeLimb(pinkie_H,pinkie); 
	rt_printk("Successful uses :]\n"); 
  	return 0; 
}

int32_t init_module(void) 
{
	int retval;
	rt_set_oneshot_mode(); 
	start_rt_timer(1);
	retval = rt_task_init(&myTask, print_function, 0, 1024, RT_SCHED_LOWEST_PRIORITY, 0, 0);
	if(retval != 0) {
		if(-EINVAL == retval) {
			printk("task: task structure is invalid\n");
		} else {
			printk("task: error starting task\n");
		}
		return retval;
	}
	retval = rt_task_resume(&myTask);
	/* error checking */
	return 0;
}

void cleanup_module(void) 
{
	stop_rt_timer();
	rt_busy_sleep(10000000);
	rt_task_delete(&myTask);
	return;
}

