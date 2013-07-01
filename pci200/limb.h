
#include "myCan.h"

typedef enum {
	l_stop 	= 0,
	l_close	= 2,
	l_open 	= 1,
	s_close = 4,
	s_open 	= 3, 
} handMode;

typedef enum {
	thumb 	= 0x101,
	index   = 0x105,
	middle	= 0x103,
	ring 	= 0x102,
	pinky 	= 0x104
 
} finger;  

typedef struct {
 	finger	digit;
	handMode mode; 
	uint32_t speed;
} command;    
	



 
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
int32_t openLimb(NTCAN_HANDLE handle, finger myDigit);

 
/***************************************************************/
/*                Function: writeLimb()								*/
/*			In: 	handle for ntcan device									*/
/*					digit to close												*/
/*			Out: 	0 if success, 1 if error 								*/
/***************************************************************/ 
int32_t closeLimb(NTCAN_HANDLE handle, finger myDigit);



