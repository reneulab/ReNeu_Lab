/*************************************************/
#include "limb.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

int32_t main(void) 
{
   NTCAN_HANDLE 	handle;
   int32_t			result;
   int32_t  		ID[1] = {0x601}; 
// Open can 
   handle = initNTCAN(NTCAN_AUTOBAUD,  NTCAN_MODE_NO_INTERACTION,
		 ID,1, 0, NTCAN_MAX_RX_QUEUESIZE,10000,
		 NTCAN_MAX_TX_QUEUESIZE, 10000);
   if(handle == 0xFFFF) {
      printf("Error in open\n");
      return 1; }
// Running all limb hand commands 	
   grip(handle, ID[0]);
   sleep(1); 
   jawChuck(handle, THUMB_OPEN, ID[0]);
   sleep(1); 
   jawChuck(handle, THUMB_CLOSE, ID[0]);
   sleep(1); 
   jawChuck(handle, STANDARD_OPEN, ID[0]);
   sleep(1); 
   jawChuck(handle, STANDARD_CLOSE, ID[0]);
   sleep(1); 
   natural(handle, ID[0]);
   sleep(1); 
   normal(handle, ID[0]);
   sleep(1); 
   park(handle, CONTINUOUS, ID[0]);
   sleep(1); 
   park(handle, QUICK, ID[0]);
   sleep(1); 
   pinch(handle, THUMB_OPEN, ID[0]);
   sleep(1); 
   pinch(handle, THUMB_CLOSE, ID[0]);
   sleep(1); 
   pinch(handle, STANDARD_OPEN, ID[0]);
   sleep(1); 
   pinch(handle, STANDARD_CLOSE, ID[0]); 
   sleep(1); 
   point(handle, ID[0]);  
   sleep(1);
// Can close
   result = closeNTCAN(handle, ID, 1); 
	if(result != 0) {
   printf("closeNTCAN error \n");
   return 1; }
   printf("Successful uses :]\n"); 
   return 0; 
}


