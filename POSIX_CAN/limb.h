
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
	int32_t 		 time;
	double 		 angle; 
	finger 		 digit;
	command		 cmd;
	NTCAN_HANDLE handle;  
} movement; 

/***************************************************************/
/*                Function: initLimb()									*/
/*			Links finger to a NTCAN handle and command				*/
/*			In: 	finger to init												*/
/*					empty pointer to command to link with finger		*/
/*			Out: 	handle for finger, 0xFFFF if error 					*/
/***************************************************************/
NTCAN_HANDLE initLimb(finger myDigit, command *myCmd); 



/***************************************************************/
/*                Function: closeLimb()								*/
/*							Shutdowns a finger 								*/
/*			In: 	finger to init												*/
/*					empty pointer to command to link with finger		*/
/*			Out: 	handle for finger, 0xFFFF if error 					*/
/***************************************************************/
int32_t closeLimb(NTCAN_HANDLE handle, finger myDigit); 



/***************************************************************/
/*                Function: writeLimb()								*/
/*		  Moves finger according to handmode in command				*/
/*			In: 	handle for ntcan device									*/
/*					type command of desired movement						*/
/*			Out: 	0 if success, 1 if error 								*/
/***************************************************************/ 
int32_t writeLimb(NTCAN_HANDLE handle, command *myCmd);


 
/***************************************************************/
/*                Function: readLimb()									*/
/*		     Reads feedback data from limb finger						*/
/*			In: 	handle for ntcan device									*/
/*					type command of desired movement						*/
/*			Out: 	0 if success, 1 if error 								*/
/***************************************************************/ 
int32_t readLimb(NTCAN_HANDLE handle, command *myCmd);



/***************************************************************/
/*                Function: moveLimb()									*/
/*     Moves a finger to an angle with input from NI device		*/
/*			In: 	session for NI device									*/
/*					Indicator name for NI device							*/
/*					type movement to specify desired angle				*/
/*			Out: 	0 if success, 1 if error 								*/
/*	NOTE:	NI device must to initialized along with finger 		*/
/*		Command linked to movement must be linked to the finger 	*/
/***************************************************************/
int32_t moveLimb(NiFpga_Session session, NiFpga_IndicatorI32 myFlex, movement *myMove);



/***************************************************************/
/*                Function: convertAngle()							*/
/*		Converts angle generated from ADC to angle in degrees		*/
/*			In: 	angle to convert to degrees							*/
/*			Out: 	0 if success, 1 if error 								*/
/***************************************************************/
double convertAngle(int32_t myAngle);  



