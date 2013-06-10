/*************************************************/
#include "./limb/limb.h"
#include <stdio.h>
#include <unistd.h>

int32_t main(void) 
{
   NTCAN_HANDLE 	handle;
   int32_t			result;
   int32_t  		ID[2] = {2,0x601}; 
// Open can 
   handle = initNTCAN(NTCAN_AUTOBAUD,  NTCAN_MODE_NO_INTERACTION,
		 ID, 0, NTCAN_MAX_RX_QUEUESIZE,10000,
		 NTCAN_MAX_TX_QUEUESIZE, 10000);
   if(handle == 0xFFFF) {
      printf("Error in open\n");
      return 1; }
// Running all limb hand commands 	
   result = grip(handle, ID[1]);
   if(result != 0) {
   	printf("Error in grip\n"); 
   	return 1; }
   sleep(1); 
   jawChuck(handle, THUMB_OPEN, ID[1]);
   if(result != 0) {
   	printf("Error in jawChuck1\n"); 
   	return 1; }
   sleep(1); 
   jawChuck(handle, THUMB_CLOSE, ID[1]);
   if(result != 0) {
   	printf("Error in jawChuck2\n"); 
   	return 1; }
   sleep(1); 
   jawChuck(handle, STANDARD_OPEN, ID[1]);
   if(result != 0) {
   	printf("Error in jawChuck3\n"); 
   	return 1; }
   sleep(1); 
   jawChuck(handle, STANDARD_CLOSE, ID[1]);
   if(result != 0) {
   	printf("Error in jawChuck4\n"); 
   	return 1; }
   sleep(1); 
   natural(handle, ID[1]);
   if(result != 0) {
   	printf("Error in natural\n"); 
   	return 1; }
   sleep(1); 
   normal(handle, ID[1]);
   if(result != 0) {
   	printf("Error in normal\n"); 
   	return 1; }
   sleep(1); 
   park(handle, CONTINUOUS, ID[1]);
   if(result != 0) {
   	printf("Error in park\n"); 
   	return 1; }
   sleep(1); 
   park(handle, QUICK, ID[1]);
   if(result != 0) {
   	printf("Error in park\n"); 
   	return 1; }
   sleep(1); 
   pinch(handle, THUMB_OPEN, ID[1]);
   if(result != 0) {
   	printf("Error in pinch1\n"); 
   	return 1; }
   sleep(1); 
   pinch(handle, THUMB_CLOSE, ID[1]);
   if(result != 0) {
   	printf("Error in pinch2\n"); 
   	return 1; }
   sleep(1); 
   pinch(handle, STANDARD_OPEN, ID[1]);
   if(result != 0) {
   	printf("Error in pinch3\n"); 
   	return 1; }
   sleep(1); 
   pinch(handle, STANDARD_CLOSE, ID[1]); 
   if(result != 0) {
   	printf("Error in pinch4\n"); 
   	return 1; }
   sleep(1); 
   point(handle, ID[1]);  
   if(result != 0) {
   	printf("Error in point\n"); 
   	return 1; }
   sleep(1);
// Can close
   result = closeNTCAN(handle, ID); 
	if(result != 0) {
	   printf("closeNTCAN error \n");
  		return 1; }
   printf("Successful uses :]\n"); 
   return 0; 
}


