#include "canLib/myCan.h"

#define  THUMB_OPEN 		1
#define 	THUMB_CLOSE 	2 
#define  STANDARD_OPEN 	3
#define	STANDARD_CLOSE 4 
#define  CONTINUOUS 		5
#define  QUICK				6 
 
/***************************************************************/
/*                Function: grip() 										*/
/*			In: 	handle for ntcan device									*/
/*					ID of i-limb device to be controlled				*/
/*			Out: 	0 if success, 1 if error 								*/
/***************************************************************/ 
int32_t grip(NTCAN_HANDLE handle, int32_t ID);


/***************************************************************/
/*                Function: jawChuck()									*/
/*			In: 	handle for ntcan device									*/
/*					mode (as defined in enum mode) of movement		*/
/*					ID of i-limb device to be controlled				*/
/*			Out: 	0 if success, 1 if error 								*/
/***************************************************************/
int32_t jawChuck(NTCAN_HANDLE handle, int32_t mode, int32_t ID);


/***************************************************************/
/*                Function: lockout() 									*/
/*			In: 	handle for ntcan device									*/
/*					ID of i-limb device to be controlled				*/
/*			Out: 	0 if success, 1 if error 								*/
/***************************************************************/
int32_t lockout(NTCAN_HANDLE handle, int32_t ID);


/***************************************************************/
/*                Function: natural 									*/
/*			In: 	handle for ntcan device									*/
/*					ID of i-limb device to be controlled				*/
/*			Out: 	0 if success, 1 if error 								*/
/***************************************************************/
int32_t natural(NTCAN_HANDLE handle, int32_t ID);


/***************************************************************/
/*                Function: normal() 									*/
/*			In: 	handle for ntcan device									*/
/*					ID of i-limb device to be controlled				*/
/*			Out: 	0 if success, 1 if error 								*/
/***************************************************************/
int32_t normal(NTCAN_HANDLE handle, int32_t ID); 


/***************************************************************/
/*                Function: park() 										*/
/*			In: 	handle for ntcan device									*/
/*					speed(defined by enum speed) of movement			*/
/*					ID of i-limb device to be controlled				*/
/*			Out: 	0 if success, 1 if error 								*/
/***************************************************************/
int32_t park(NTCAN_HANDLE handle, int32_t speed, int32_t ID); 


/***************************************************************/
/*                Function: pinch() 									*/
/*			In: 	handle for ntcan device									*/
/*             mode(defined in enum mode) for desired movement	*/			
/*					ID of i-limb device to be controlled				*/
/*			Out: 	0 if success, 1 if error 								*/
/***************************************************************/
int32_t pinch(NTCAN_HANDLE handle, int32_t mode, int32_t ID);


/***************************************************************/
/*                Function: point() 									*/
/*			In: 	handle for ntcan device									*/
/*					ID of i-limb device to be controlled				*/
/*			Out: 	0 if success, 1 if error 								*/
/***************************************************************/
int32_t point(NTCAN_HANDLE handle, int32_t ID);


