
#include "myCan.h"
#include "NiFpga_limb.h"
#include "NiFpga.h"

typedef enum {
	thumb  = 1,
	index  = 2,
	middle = 3,
	ring   = 4,
	pinkie = 5
} finger; 

typedef enum {
	l_stop 	= 0,
	l_open 	= 1,
	l_close	= 2,
	s_open 	= 3, 
	s_close = 4
} handMode;
	
typedef struct {
	handMode mode; 
	uint32_t speed;
 	int32_t	digit_w;
	int32_t digit_r;
} command;    

typedef struct {
	int32_t 	 time;
	int32_t 	 angle; 
	int32_t 	 initAngle;
	NiFpga_limb_IndicatorI32  	flex; 
	finger 		 digit;
	command		 cmd;
	NTCAN_HANDLE handle;  
} arg; 

NTCAN_HANDLE initLimb(finger myDigit, command *myCmd); 

int32_t closeLimb(NTCAN_HANDLE handle, finger myDigit); 
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

int32_t moveLimb(NiFpga_Session session, arg *myArg);

double convertAngle(int32_t myAngle);  

