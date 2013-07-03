/***************************************************************/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "limb.h"
#include "myCan.h" 
#define MESSAGE_LEN 5
// 1: 5,4,3,2,1
// 2: 5,3,1,4,2
// 3: 5,2,4,1,3
// 4: 5,1,2,3,4
// 5: 5,5,5,5,5
/****************************************************************/
/****************************************************************/
int32_t writeLimb(NTCAN_HANDLE handle, command *myCmd) 
{
  CMSG msg;
  int32_t result;

  printf("IN WRITE\n");
  msg.id = myCmd->digit;
  msg.len = 4;
  msg.data[3] = (uint8_t)(myCmd->speed & 0x00FF);
  msg.data[2] = (uint8_t)((myCmd->speed) >> 8);
  msg.data[1] = (uint8_t)((myCmd->mode) & 0x00FF);
  msg.data[0] = (uint8_t)0x00; 
  printf("Mode %d\n",msg.data[2]);
  printf("Speed %d\n", msg.data[3] + (msg.data[2]<<8)); 
  result = writeNTCAN(handle,1, &msg);
  if(result != 0) 
  { 
    printf("Error in Write\n"); 
    msg.data[2] = l_stop;
    writeNTCAN(handle,1, &msg);
    return 1;
  }
  return 0;  
}


/***************************************************************/
/***************************************************************/
int32_t readLimb(NTCAN_HANDLE handle, command *myCmd)
{
	CMSG msg[MESSAGE_LEN];
  	int32_t result,i;

  	printf("IN READ\n");
	for(i=0;i<MESSAGE_LEN;i++){ 
  	msg[i].id = myCmd->digit; 
  	msg[i].len = 8;  } 
  	result = readNTCAN(handle,msg,MESSAGE_LEN);
  	//myCmd->mode = msg.data[1] | (msg.data[0]<<8);
  	//myCmd->speed = msg.data[3] | (msg.data[2]<<8);
  	//printf("Mode: %d\n", myCmd->mode); 
  	if(result != 0)
  		{ return 1; }
  	return 0;
}	 
		 
/***************************************************************/
/***************************************************************/
int32_t openLimb(NTCAN_HANDLE handle, finger myDigit, int32_t spd) 
{
  command myCmd;
  int32_t result;

  myCmd.digit = myDigit;
  myCmd.mode = l_open; 
  myCmd.speed = spd;
  result = writeLimb(handle, &myCmd);  
  if(result != 0)
    { return 1; }
/* Get feedback until finger is open all the way */
  do {
    result = readLimb(handle, &myCmd);
    if(result != 0)
      { return 1; }
	 usleep(20000);
    } while (myCmd.mode != s_open);
/* Stop motor on finger */
  myCmd.mode = l_stop;
  myCmd.speed = 100; 
  result = writeLimb(handle, &myCmd);
  if(result != 0)
  	{ return 1; }
  return 0; 
}
/**********************************************/	 
int32_t closeLimb(NTCAN_HANDLE handle, finger myDigit, int32_t spd) 
{
  command myCmd;
  int32_t result;

  myCmd.digit = myDigit;
  myCmd.mode = l_close; 
  myCmd.speed = spd;
  result = writeLimb(handle, &myCmd);  
  if(result != 0)
    { return 1; }
/* Get feedback until finger is open all the way */
  do {
    result = readLimb(handle, &myCmd);
    if(result != 0)
      { return 1; }
	 usleep(20000); 	
  } while (myCmd.mode != s_close);
/* Stop motor on finger */
  myCmd.mode = l_stop;
  myCmd.speed = 100; 
  result = writeLimb(handle, &myCmd);
  if(result != 0)
    { return 1; }
  return 0; 
}

int32_t moveLimb_T(NTCAN_HANDLE handle, finger myDigit, handMode myMode, movement *move)
{
	command myCmd;
	int32_t result;

	if((myMode==s_open) || (myMode==s_close)) {
		printf("Error invaild mode"); 
		return 1;
	}
	
	if(move->angle == NULL) 
	{
// move based on time and speed
		myCmd.digit = myDigit;
		myCmd.mode = myMode;
		myCmd.speed = move->vel;
		if(myCmd.speed == NULL)
			{ myCmd.speed = 400; }
		result = writeLimb(handle, &myCmd);
		if(result != 0) 
		{ 
			myCmd.mode = l_stop;
			writeLimb(handle, &myCmd);
		 	return 1; 
		}
		usleep(move->time);
		myCmd.mode = l_stop;
		result = writeLimb(handle, &myCmd);  
		if(result != 0) 
			{ return 1; }
		return 0;
	}
	else if( move->time == NULL)
	{
// move a distance
		myCmd.digit = myDigit;
		myCmd.mode = myMode;
		myCmd.speed = move->vel;
		if(myCmd.speed == NULL)
			{ myCmd.speed = 400; }
		
	}


	myCmd.digit = myDigit;
	myCmd.mode = myMode; 
	myCmd.speed = move->vel;
	result = writeLimb(handle, &myCmd);
	if(result != 0) { 
		myCmd.mode = l_stop;
		writeLimb(handle, &myCmd);
		 return 1; 
	}
	usleep(move->time);
	myCmd.mode = l_stop;
	result = writeLimb(handle, &myCmd);  
	if(result != 0) 
		{ return 1; }
	return 0; 
}
	

