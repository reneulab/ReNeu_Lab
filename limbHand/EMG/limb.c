/***************************************************************/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "limb.h"
#include "myCan.h" 
/************** DEFINES **************/
#define MESSAGE_LEN 1
#define ADC_DEGREE 1
#define ADC_OFFSET 1
#define ADC_GAIN 1000

int32_t thumb_ID[3] = {3,0x101,0x201};
int32_t index_ID[3] = {3,0x105,0x205};
int32_t middle_ID[3] = {3,0x103,0x203};
int32_t ring_ID[3] = {3,0x102,0x202};
int32_t pinkie_ID[3] = {3,0x104,0x204};
/****************************************************************/
/****************************************************************/
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
int32_t moveLimb(NiFpga_Session session, NiFpga_limb_IndicatorI32 myFlex, arg *myArg)
{
	int32_t result;
 	int32_t realAngle;

	do 
	{	
		result = writeLimb(myArg->handle,&(myArg->cmd));
  		if(result != 0)
  			{ return 1; }
		NiFpga_ReadI32(session,myFlex,&realAngle); 
	//	realAngle = convertAngle(realAngle);
		if(realAngle > myArg->angle)
			{ myArg->cmd.mode = l_close; }
		else if(realAngle < myArg->angle)
			{ myArg->cmd.mode = l_open; }
	} while(realAngle != myArg->angle); 
	
	myArg->cmd.mode = l_stop;
	writeLimb(myArg->handle,&(myArg->cmd));	
	return 0; 
}

/***************************************************************/
/***************************************************************/
double convertAngle(int32_t myAngle)
{
	double newAngle; 

	newAngle = myAngle; ///ADC_DEGREE; 	// ADC_DEGREE needs to be tested out 
	return newAngle; 
}
/* end of fuctions */



