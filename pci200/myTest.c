/*************************************************/
#include "limb.h"
#include "myCan.h"
#include <stdio.h>
#include <unistd.h>

int32_t main(void) 
{
<<<<<<< HEAD
  NTCAN_HANDLE   handle;
  int32_t	  result;
  int32_t  	  ID[11] = {11,0x101,0x102,0x103,0x104,
                  0x105,0x201,0x202,0x203,0x204,0x205};
  command 	  cmd;
  int32_t         i; 
	 
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
  cmd.digit = pinky;
  cmd.speed = 400;
  cmd.mode = l_close; 
  result = writeLimb(handle,&cmd);
  printf("result:%d\n",result); 
  sleep(1); 
// Closing //   
  result = closeNTCAN(handle, ID);
  if(result != 0) {
    printf("closeNTCAN error \n"); 
    return 1; 
  }
  printf("Successful uses :]\n"); 
  return 0; 
=======
   NTCAN_HANDLE 	handle;
   int32_t			result;
   int32_t  		ID[6] = {2,0x101,0x102,0x103,0x104,0x105};
	command 			cmd; 
	 
// Open can 
   handle = initNTCAN(NTCAN_AUTOBAUD,  NTCAN_MODE_NO_INTERACTION,
		 ID, 0, NTCAN_MAX_RX_QUEUESIZE,10000,
		 NTCAN_MAX_TX_QUEUESIZE, 10000);
   if(handle == 0xFFFF) {
      printf("Error in open\n");
      return 1; }
// Running limb hand commands
	cmd.digit=thumb; 
	result = readLimb(handle, &cmd);
	if(result != 0) 
		{ return 1; }
	printf("Thumb: %d\n", cmd.mode); 
	cmd.digit=pointer; 
	result = readLimb(handle, &cmd);
	if(result != 0) 
		{ return 1; }
	printf("Pointer: %d\n", cmd.mode);
	cmd.digit=middle; 
	result = readLimb(handle, &cmd);
	if(result != 0) 
		{ return 1; }
	printf("Middle: %d\n", cmd.mode);
	cmd.digit=thumb; 
	result = readLimb(handle, &cmd);
	if(result != 0) 
		{ return 1; }
	printf("Index: %d\n", cmd.mode);
	cmd.digit=thumb; 
	result = readLimb(handle, &cmd);
	if(result != 0) 
		{ return 1; }
	printf("Pinky: %d\n", cmd.mode);
	  	
// Can close
   result = closeNTCAN(handle, ID); 
	if(result != 0) {
	   printf("closeNTCAN error \n");
  		return 1; }
   printf("Successful uses :]\n"); 
   return 0; 
>>>>>>> b2db34a880966da68ed898239b36b1945b475766
}


