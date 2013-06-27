/*************************************************/
#include "limb.h"
#include "myCan.h"
#include <stdio.h>
#include <unistd.h>

int32_t main(void) 
{
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
}


