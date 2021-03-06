/*************************************************/
#include "limb.h"
#include "myCan.h"
#include <stdio.h>
#include <unistd.h>

int32_t main(void) 
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
		{ printf("Error in thumb init\n"); return 1; } 
	usleep(20000);
 
	index_H = initLimb(index,&index_cmd);
	if(index_H == 0xFFFF) 
		{ printf("Error in index init\n"); return 1; } 
	usleep(20000);
 
	middle_H = initLimb(middle,&middle_cmd);
	if(middle_H == 0xFFFF) 
		{ printf("Error in middle init\n"); return 1; } 
	usleep(20000);
 
	ring_H = initLimb(ring,&ring_cmd);
	if(ring_H == 0xFFFF) 
		{ printf("Error in ring init\n"); return 1; } 
	usleep(20000);
 
	pinkie_H = initLimb(pinkie,&pinkie_cmd);
	if(pinkie_H == 0xFFFF) 
		{ printf("Error in pinkie init\n"); return 1; } 
	usleep(20000); 
// Running limb hand commands
// Writing and Reading //
	printf("Closing\n");
	thumb_cmd.speed = 690;
	index_cmd.speed = 690;
	middle_cmd.speed = 690;
	ring_cmd.speed = 690;
	pinkie_cmd.speed = 690;

/*	thumb_cmd.mode = l_close;
	index_cmd.mode = l_close;
	middle_cmd.mode = l_close;
	ring_cmd.mode = l_close;
	pinkie_cmd.mode = l_close;

	printf("******THUMB******\n");
	result = writeLimb(thumb_H,&thumb_cmd);
	usleep(20000);
	result = readLimb(thumb_H,&thumb_cmd);
	sleep(1);

	printf("******INDEX******\n");
	result = writeLimb(index_H,&index_cmd);
	usleep(20000);
	result = readLimb(index_H,&index_cmd);
	sleep(1);

	printf("******MIDDLE******\n");
	result = writeLimb(middle_H,&middle_cmd);
	usleep(20000);
	result = readLimb(middle_H,&middle_cmd);
	sleep(1);

	printf("******RING******\n");
	result = writeLimb(ring_H,&ring_cmd);
	usleep(20000);
	result = readLimb(ring_H,&ring_cmd);
	sleep(1);

	printf("******PINKIE******\n"); 
	result = writeLimb(pinkie_H,&pinkie_cmd);
	usleep(20000);
	result = readLimb(pinkie_H,&pinkie_cmd);
	sleep(1);*/

	sleep(1); 
	thumb_cmd.mode = l_open;
	index_cmd.mode = l_open;
	middle_cmd.mode = l_open;
	ring_cmd.mode = l_open;
	pinkie_cmd.mode = l_open;
	
	printf("******PINKIE******\n");
	result = writeLimb(pinkie_H,&pinkie_cmd);
	usleep(20000);
	result = readLimb(pinkie_H,&pinkie_cmd);
	sleep(1);

	printf("******RING******\n");
	result = writeLimb(ring_H,&ring_cmd);
	usleep(20000);
	result = readLimb(ring_H,&ring_cmd);
	sleep(1);

	printf("******MIDDLE******\n");
	result = writeLimb(middle_H,&middle_cmd);
	usleep(20000);
	result = readLimb(middle_H,&middle_cmd);
	sleep(1);

	printf("******INDEX******\n");
	result = writeLimb(index_H,&index_cmd);
	usleep(20000);
	result = readLimb(index_H,&index_cmd);
	sleep(1);

	printf("******THUMB******\n");
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

	printf("******THUMB******\n");
	result = writeLimb(thumb_H,&thumb_cmd);
	usleep(20000);
	result = readLimb(thumb_H,&thumb_cmd);
	usleep(20000);

	printf("******INDEX******\n");
	result = writeLimb(index_H,&index_cmd);
	usleep(20000);
	result = readLimb(index_H,&index_cmd);
	usleep(20000);

	printf("******MIDDLE******\n");
	result = writeLimb(middle_H,&middle_cmd);
	usleep(20000);
	result = readLimb(middle_H,&middle_cmd);
	usleep(20000);

	printf("******RING******\n");
	result = writeLimb(ring_H,&ring_cmd);
	usleep(20000);
	result = readLimb(ring_H,&ring_cmd);
	usleep(20000);

	printf("******PINKIE******\n"); 
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
	printf("Successful uses :]\n"); 
  	return 0; 
}


