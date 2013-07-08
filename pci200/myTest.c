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
  int32_t	thumb_ID[3] = {3,0x101,0x201};
  int32_t	index_ID[3] = {3,0x105,0x205};
  int32_t	middle_ID[3] = {3,0x103,0x203};
  int32_t	ring_ID[3] = {3,0x102,0x202};
  int32_t	pinkie_ID[3] = {3,0x104,0x204};
  command 	thumb_cmd, index_cmd, middle_cmd, ring_cmd, pinkie_cmd;
  int32_t     	i;
  CMSG		msg; 
	 
// Open can 
  thumb_H = initNTCAN(NTCAN_BAUD_1000, 0,thumb_ID,0, NTCAN_MAX_RX_QUEUESIZE,
						1000,NTCAN_MAX_TX_QUEUESIZE, 1000);
  if(thumb_H == 0xFFFF) {
    printf("Error in open\n"); return 1; }

  index_H = initNTCAN(NTCAN_BAUD_1000, 0,index_ID,0, NTCAN_MAX_RX_QUEUESIZE,
						1000,NTCAN_MAX_TX_QUEUESIZE, 1000);
  if(index_H == 0xFFFF) {
    printf("Error in open\n"); return 1; }

  middle_H = initNTCAN(NTCAN_BAUD_1000, 0,middle_ID,0, NTCAN_MAX_RX_QUEUESIZE,
						1000,NTCAN_MAX_TX_QUEUESIZE, 1000);
  if(middle_H == 0xFFFF) {
    printf("Error in open\n"); return 1; }

  ring_H = initNTCAN(NTCAN_BAUD_1000, 0,ring_ID,0, NTCAN_MAX_RX_QUEUESIZE,
						1000,NTCAN_MAX_TX_QUEUESIZE, 1000);
  if(ring_H == 0xFFFF) {
    printf("Error in open\n"); return 1; }

  pinkie_H = initNTCAN(NTCAN_BAUD_1000, 0,pinkie_ID,0, NTCAN_MAX_RX_QUEUESIZE,
						1000,NTCAN_MAX_TX_QUEUESIZE, 1000);
  if(pinkie_H == 0xFFFF) {
    printf("Error in open\n"); return 1; }
	sleep(2);
// Running limb hand commands
// Writing and Reading //
	printf("Closing\n");
	thumb_cmd.digit_w = thumb_ID[1];
	thumb_cmd.digit_r = thumb_ID[2];
	index_cmd.digit_w = index_ID[1];
	index_cmd.digit_r = index_ID[2];
	middle_cmd.digit_w = middle_ID[1];
	middle_cmd.digit_r = middle_ID[2];	
	ring_cmd.digit_w = ring_ID[1];
	ring_cmd.digit_r = ring_ID[2];
	pinkie_cmd.digit_w = pinkie_ID[1];
	pinkie_cmd.digit_r = pinkie_ID[2];

	thumb_cmd.speed = 690;
	index_cmd.speed = 690;
	middle_cmd.speed = 690;
	ring_cmd.speed = 690;
	pinkie_cmd.speed = 690;

	thumb_cmd.mode = l_close;
	index_cmd.mode = l_close;
	middle_cmd.mode = l_close;
	ring_cmd.mode = l_close;
	pinkie_cmd.mode = l_close;

	printf("******THUMB******\n");
	result = writeLimb(thumb_H,&thumb_cmd);
	usleep(40000);
	result = readLimb(thumb_H,&thumb_cmd);
	result = readLimb(index_H,&index_cmd);
	sleep(1);

	printf("******INDEX******\n");
	result = writeLimb(index_H,&index_cmd);
	usleep(40000);
	result = readLimb(index_H,&index_cmd);
	sleep(1);

	printf("******MIDDLE******\n");
	result = writeLimb(middle_H,&middle_cmd);
	usleep(40000);
	result = readLimb(middle_H,&middle_cmd);
	sleep(1);

	printf("******RING******\n");
	result = writeLimb(ring_H,&ring_cmd);
	usleep(40000);
	result = readLimb(ring_H,&ring_cmd);
	sleep(1);

	printf("******PINKIE******\n"); 
	result = writeLimb(pinkie_H,&pinkie_cmd);
	usleep(40000);
	result = readLimb(pinkie_H,&pinkie_cmd);
	sleep(1);

	sleep(2); 
	thumb_cmd.mode = l_open;
	index_cmd.mode = l_open;
	middle_cmd.mode = l_open;
	ring_cmd.mode = l_open;
	pinkie_cmd.mode = l_open;
	
	printf("******PINKIE******\n");
	result = writeLimb(pinkie_H,&pinkie_cmd);
	usleep(40000);
	result = readLimb(pinkie_H,&pinkie_cmd);
	sleep(1);

	printf("******RING******\n");
	result = writeLimb(ring_H,&ring_cmd);
	usleep(40000);
	result = readLimb(ring_H,&ring_cmd);
	sleep(1);

	printf("******MIDDLE******\n");
	result = writeLimb(middle_H,&middle_cmd);
	usleep(40000);
	result = readLimb(middle_H,&middle_cmd);
	sleep(1);

	printf("******INDEX******\n");
	result = writeLimb(index_H,&index_cmd);
	usleep(40000);
	result = readLimb(index_H,&index_cmd);
	sleep(1);

	printf("******THUMB******\n");
	result = writeLimb(thumb_H,&thumb_cmd);
	usleep(40000);
	result = readLimb(thumb_H,&thumb_cmd);
	sleep(1);

	sleep(2); 
	thumb_cmd.mode = l_stop;
	index_cmd.mode = l_stop;
	middle_cmd.mode = l_stop;
	ring_cmd.mode = l_stop;
	pinkie_cmd.mode = l_stop;

	printf("******THUMB******\n");
	result = writeLimb(thumb_H,&thumb_cmd);
	usleep(40000);
	result = readLimb(thumb_H,&thumb_cmd);
	sleep(1);

	printf("******INDEX******\n");
	result = writeLimb(index_H,&index_cmd);
	usleep(40000);
	result = readLimb(index_H,&index_cmd);
	sleep(1);

	printf("******MIDDLE******\n");
	result = writeLimb(middle_H,&middle_cmd);
	usleep(40000);
	result = readLimb(middle_H,&middle_cmd);
	sleep(1);

	printf("******RING******\n");
	result = writeLimb(ring_H,&ring_cmd);
	usleep(40000);
	result = readLimb(ring_H,&ring_cmd);
	sleep(1);

	printf("******PINKIE******\n"); 
	result = writeLimb(pinkie_H,&pinkie_cmd);
	usleep(40000);
	result = readLimb(pinkie_H,&pinkie_cmd);
	sleep(1);
// Closing //   
	result = closeNTCAN(thumb_H, thumb_ID);
  	if(result != 0) 
	{ printf("closeNTCAN error \n"); return 1; }   
 
	result = closeNTCAN(index_H, index_ID);
  	if(result != 0) 
	{ printf("closeNTCAN error \n"); return 1; }
	
	result = closeNTCAN(middle_H, middle_ID);
  	if(result != 0) 
	{ printf("closeNTCAN error \n"); return 1; }
	
	result = closeNTCAN(ring_H, ring_ID);
  	if(result != 0) 
	{ printf("closeNTCAN error \n"); return 1; }
	
	result = closeNTCAN(pinkie_H, pinkie_ID);
  	if(result != 0) 
	{ printf("closeNTCAN error \n"); return 1; }
  	
	printf("Successful uses :]\n"); 
  	return 0; 
}


