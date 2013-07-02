
#include "myCan.h"

typedef enum {
	l_stop 	= 0,
	l_close	= 2,
	l_open 	= 1,
	s_close = 4,
	s_open 	= 3, 
} handMode;

typedef enum {
	thumb_W		= 0x101,
	thumb_R		= 0x201,
	index_W	 	= 0x105,
	index_R		= 0x205,
	middle_W	= 0x103,
	middle_R	= 0x203,
	ring_W 		= 0x102,
	ring_R		= 0x202,
	pinky_W 	= 0x104,
	pinky_R		= 0x204
} finger;  

typedef struct {
 	finger	digit;
	handMode mode; 
	uint32_t speed;
} command;    
	
typedef struct {
	int32_t vel;
	int32_t time;
	int32_t dist; 
} movement;


 
/***************************************************************/
/*                Function: writeLimb()								*/
/*			In: 	handle for ntcan device									*/
/*					type command of desired movement						*/
/*			Out: 	0 if success, 1 if error 								*/
/***************************************************************/ 
int32_t writeLimb(NTCAN_HANDLE handle, command *myCmd);

 
/***************************************************************/
/*                Function: readLimb()									*/
/*			In: 	handle for ntcan device									*/
/*					type command of desired movement						*/
/*			Out: 	0 if success, 1 if error 								*/
/***************************************************************/ 
int32_t readLimb(NTCAN_HANDLE handle, command *myCmd);

 
/***************************************************************/
/*                Function: writeLimb()								*/
/*			In: 	handle for ntcan device									*/
/*				 	digit to open												*/
/*			Out: 	0 if success, 1 if error 								*/
/***************************************************************/ 
int32_t openLimb(NTCAN_HANDLE handle, finger myDigit, int32_t spd);

 
/***************************************************************/
/*                Function: writeLimb()								*/
/*			In: 	handle for ntcan device									*/
/*					digit to close												*/
/*			Out: 	0 if success, 1 if error 								*/
/***************************************************************/ 
int32_t closeLimb(NTCAN_HANDLE handle, finger myDigit, int32_t spd);

int32_t moveLimb_T(NTCAN_HANDLE handle, finger myDigit, handMode myMode, movement *move); 

