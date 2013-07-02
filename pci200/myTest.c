/*************************************************/
#include "limb.h"
#include "myCan.h"
#include <stdio.h>
#include <unistd.h>

int32_t main(void) 
{
  NTCAN_HANDLE   handle;
  int32_t	  	result;
  int32_t  	  	ID[11] = {11,thumb,index,middle,ring,
                  pinky,0x201,0x202,0x203,0x204,0x205};
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
// Reading  //
  for(i=thumb; i<=pinky; i++)
  {
    cmd.digit = i;
    cmd.mode = 0xFF;
    result = readLimb(handle, &cmd);
    if(result != 0) 
    { 
      result = closeNTCAN(handle, ID); 
      if(result != 0) {
        printf("closeNTCAN error \n");
        return 1; 
      }
      return 1;
    }
    printf("Digit:%x Mode:%d\n", cmd.digit,cmd.mode);
    usleep(20000);
  }
// Writing //
	for(i=1; i<=5; i++)
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


