
#include "myCan.h"

<<<<<<< HEAD
=======
#define  THUMB_OPEN 		1
#define 	THUMB_CLOSE 	2 
#define  STANDARD_OPEN 	3
#define	STANDARD_CLOSE 4 
#define  CONTINUOUS 		5
#define  QUICK				6 

>>>>>>> b2db34a880966da68ed898239b36b1945b475766
typedef enum {
	l_stop 	= 0,
	l_close	= 1,
	l_open 	= 2,
<<<<<<< HEAD
	s_close = 3,
=======
	s_close 	= 3,
>>>>>>> b2db34a880966da68ed898239b36b1945b475766
	s_open 	= 4, 
} handMode;

typedef enum {
	thumb 	= 0x101,
<<<<<<< HEAD
	index   = 0x102,
	middle	= 0x103,
	ring 	= 0x104,
=======
	pointer 	= 0x102,
	middle	= 0x103,
	index 	= 0x104,
>>>>>>> b2db34a880966da68ed898239b36b1945b475766
	pinky 	= 0x105 
} finger;  

typedef struct {
 	finger	digit;
	handMode mode; 
<<<<<<< HEAD
	uint32_t speed;
=======
	int32_t 	speed;
>>>>>>> b2db34a880966da68ed898239b36b1945b475766
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



