/***************************************************************/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "limb.h"
#include "myCan.h" 
#define MESSAGE_LEN 1
int32_t thumb_ID[3] = {3,0x101,0x201};
int32_t index_ID[3] = {3,0x105,0x205};
int32_t middle_ID[3] = {3,0x103,0x203};
int32_t ring_ID[3] = {3,0x102,0x202};
int32_t pinkie_ID[3] = {3,0x104,0x204};
// 1: 5,4,3,2,1
// 2: 5,3,1,4,2
// 3: 5,2,4,1,3
// 4: 5,1,2,3,4
// 5: 5,5,5,5,5
/****************************************************************/
/****************************************************************/

 
int32_t findFinger(finger myDigit)
{
	switch(myDigit)
	{
		case thumb:
			return 0; break;
		case index:
			return 1; break;
		case middle:
			return 2; break;
		case ring: 
			return 3; break;
		case pinkie:
			return 4; break;
		default:
			return 0xFF;
  	}
}

NTCAN_HANDLE initLimb(finger myDigit, command *myCmd)
{
	NTCAN_HANDLE handle;

	switch(myDigit) 
	{
		case thumb:
			handle = initNTCAN(NTCAN_BAUD_1000, 0,thumb_ID,0, NTCAN_MAX_RX_QUEUESIZE,
						       1000,NTCAN_MAX_TX_QUEUESIZE, 1000);
			myCmd->digit_w = thumb_ID[1];
			myCmd->digit_r = thumb_ID[2];
			myCmd->speed   = 200;
			myCmd->mode    = l_stop; 
			break;
		  	
		case index:
			handle = initNTCAN(NTCAN_BAUD_1000, 0,index_ID,0, NTCAN_MAX_RX_QUEUESIZE,
						       1000,NTCAN_MAX_TX_QUEUESIZE, 1000);
			myCmd->digit_w = index_ID[1];
			myCmd->digit_r = index_ID[2];
			myCmd->speed   = 200;
			myCmd->mode    = l_stop; 
			break;

		case middle:
			handle = initNTCAN(NTCAN_BAUD_1000, 0,middle_ID,0, NTCAN_MAX_RX_QUEUESIZE,
						       1000,NTCAN_MAX_TX_QUEUESIZE, 1000);
			myCmd->digit_w = middle_ID[1];
			myCmd->digit_r = middle_ID[2];
			myCmd->speed   = 200;
			myCmd->mode    = l_stop; 
			break;

		case ring:
			handle = initNTCAN(NTCAN_BAUD_1000, 0,ring_ID,0, NTCAN_MAX_RX_QUEUESIZE,
						       1000,NTCAN_MAX_TX_QUEUESIZE, 1000);
			myCmd->digit_w = ring_ID[1];
			myCmd->digit_r = ring_ID[2];
			myCmd->speed   = 200;
			myCmd->mode    = l_stop; 
			break;
		
		case pinkie:
			handle = initNTCAN(NTCAN_BAUD_1000, 0,pinkie_ID,0, NTCAN_MAX_RX_QUEUESIZE,
						       1000,NTCAN_MAX_TX_QUEUESIZE, 1000);
			myCmd->digit_w = pinkie_ID[1];
			myCmd->digit_r = pinkie_ID[2];
			myCmd->speed   = 200;
			myCmd->mode    = l_stop; 
			break;
		default: 
			printf("invaild finger\n");
	}

	if(handle == 0xFFFF) 
		{ printf("Error in open\n"); return 0xFFFF; }
	return handle; 
}
/**************************************************************************/
/**************************************************************************/
int32_t closeLimb(NTCAN_HANDLE handle, finger myDigit)
{
	NTCAN_RESULT result; 

	switch(myDigit) 
	{
		case thumb: 
			result = closeNTCAN(handle, thumb_ID);
  			if(result != 0) 
				{ printf("closeNTCAN error \n"); return 1; } 
			break;
		  	
		case index:
			result = closeNTCAN(handle, index_ID);
  			if(result != 0) 
				{ printf("closeNTCAN error \n"); return 1; } 
			break;

		case middle:
			result = closeNTCAN(handle, middle_ID);
  			if(result != 0) 
				{ printf("closeNTCAN error \n"); return 1; } 
			break;

		case ring:
			result = closeNTCAN(handle, ring_ID);
  			if(result != 0) 
				{ printf("closeNTCAN error \n"); return 1; } 
			break;
		
		case pinkie:
			result = closeNTCAN(handle, pinkie_ID);
  			if(result != 0) 
				{ printf("closeNTCAN error \n"); return 1; } 
			break;
		default: 
			printf("invaild finger\n");
	}
	return 0; 
}
/**************************************************************************/
/**************************************************************************/
int32_t writeLimb(NTCAN_HANDLE handle, command *myCmd) 
{
  CMSG msg;
  int32_t result;

  //printf("IN WRITE\n");
  msg.id = myCmd->digit_w;
  msg.len = 4;
  msg.data[3] = (uint8_t)(myCmd->speed & 0x00FF);
  msg.data[2] = (uint8_t)((myCmd->speed) >> 8);
  msg.data[1] = (uint8_t)((myCmd->mode) & 0x00FF);
  msg.data[0] = (uint8_t)0x00; 
  //printf("Mode %d\n", msg.data[1]);
  //printf("Speed %d\n", msg.data[3] + (msg.data[2]<<8)); 
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

	for(i=0;i<MESSAGE_LEN;i++){ 
  	msg[i].id = myCmd->digit_r; 
  	msg[i].len = 4;  } 
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
int32_t openDigit(NTCAN_HANDLE handle, int32_t myDigit[], int32_t spd) 
{
  command myCmd;
  int32_t result;

  myCmd.digit_w = myDigit[1];
  myCmd.digit_r = myDigit[2]; 
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
int32_t closeDigit(NTCAN_HANDLE handle, int32_t myDigit[], int32_t spd) 
{
  command myCmd;
  int32_t result;

  myCmd.digit_w = myDigit[1];
  myCmd.digit_r = myDigit[2];
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

int32_t moveLimb_T(NTCAN_HANDLE handle, int32_t myDigit, handMode myMode, movement *move)
{
 /*	command myCmd;
	int32_t result;

	if((myMode==s_open) || (myMode==s_close)) {
		printf("Error invaild mode"); 
		return 1;
	}
	
	if(move->angle == NULL) 
	{
// move based on time and speed
		myCmd.digit_w = myDigit[1];
		myCmd.digit_r = myDigit[2];
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
		myCmd.digit_w = myDigit[1];
		myCmd.digit_r = myDigit[2];
		myCmd.mode = myMode;
		myCmd.speed = move->vel;
		if(myCmd.speed == NULL)
			{ myCmd.speed = 400; }
		
	}


	myCmd.digit_w = myDigit[1];
	myCmd.digit_r = myDigit[2];
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
	return 0; */
}
	

