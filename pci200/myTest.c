/*************************************************/
#include "limb.h"
#include "myCan.h"
#include <stdio.h>
#include <unistd.h>

int32_t main(void) 
{
  NTCAN_HANDLE   handle;
  int32_t	  	result;
  int32_t  	  	ID[11] = {11,thumb_W,index_W,middle_W,ring_W,
                  pinky_W,thumb_R,index_R,middle_R,ring_R,pinky_R};
  command 	  	cmd;
  int32_t     	i;
  CMSG			msg; 
	 
// Open can 
  handle = initNTCAN(NTCAN_BAUD_1000, 0,
		 ID,0, NTCAN_MAX_RX_QUEUESIZE,1000,
		 NTCAN_MAX_TX_QUEUESIZE, 1000);
  if(handle == 0xFFFF) {
    printf("Error in open\n");
    return 1; 
  }
// Running limb hand commands
// Writing and Reading //
/*	for(i=1; i<=5; i++)
  	{
  		cmd.digit = ID[i];
  		cmd.speed = 500;
  		cmd.mode = l_close; 
  		result = writeLimb(handle,&cmd);
  		printf("result:%d\n",result);
		usleep(20000); 
 		result = readLimb(handle, &cmd);
    	if(result != 0) 
    	{ 
      	result = closeNTCAN(handle, ID); 
      	if(result != 0) 
			{
        		printf("closeNTCAN error \n");
        		return 1; 
      	}
      	return 1;
    	}
    	printf("Digit:%x Mode:%d\n", cmd.digit,cmd.mode);
    	usleep(20000); 
  		sleep(1);	
	}
  	sleep(2);
	for(i=1; i<=5; i++)
  	{
  		cmd.digit = ID[i];
  		cmd.speed = 500;
  		cmd.mode = l_stop; 
  		result = writeLimb(handle,&cmd);
  		printf("result:%d\n",result); 
    	usleep(20000);
  		result = readLimb(handle, &cmd);
    	if(result != 0) 
    	{ 
      	result = closeNTCAN(handle, ID); 
      	if(result != 0) 
			{
       		printf("closeNTCAN error \n");
        		return 1; 
      	}
      	return 1;
    	}
    	printf("Digit:%x Mode:%d\n", cmd.digit,cmd.mode);
		usleep(20000);
  		sleep(1); 	
  	}
  	sleep(2);
	for(i=5; i>=1; i--)
  	{
  		cmd.digit = ID[i];
  		cmd.speed = 650;
  		cmd.mode = l_open; 
  		result = writeLimb(handle,&cmd);
  		printf("result:%d\n",result);
		usleep(20000); 
		result = readLimb(handle, &cmd);
    	if(result != 0) 
    	{ 
      	result = closeNTCAN(handle, ID); 
      	if(result != 0) 
			{
       		printf("closeNTCAN error \n");
        		return 1; 
      	}
      	return 1;
    	}
    	printf("Digit:%x Mode:%d\n", cmd.digit,cmd.mode);
		usleep(20000);
  		sleep(1);
  	} */
	for(i=1;i<=5;i++) 
	{	
		cmd.digit = ID[i];
		cmd.speed = 690;
		cmd.mode = l_close;
		result = writeLimb(handle,&cmd);
		usleep(20000);
		cmd.digit = ID[i+5];
		result = readLimb(handle,&cmd);
		usleep(20000);
		sleep(2);
		cmd.digit = ID[i];
		cmd.mode = l_open;
		result = writeLimb(handle,&cmd);
		usleep(20000);
		cmd.digit = ID[i+5];
		result = readLimb(handle,&cmd);
		usleep(20000);
		sleep(2);
	}
	 
// Closing //   
	result = closeNTCAN(handle, ID);
  	if(result != 0) 
	{
   	printf("closeNTCAN error \n"); 
    	return 1; 
  	}
  	printf("Successful uses :]\n"); 
  	return 0; 
}


