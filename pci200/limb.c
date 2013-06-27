
#include "limb.h" 

/****************************************************************/
/****************************************************************/
int32_t writeLimb(NTCAN_HANDLE handle, command *myCmd) 
{
	CMSG msg;
   int32_t result;

   msg.id = myCmd->digit;
	msg.len = 2;
	msg.data[0] = myCmd->speed;
	msg.data[1] = myCmd->mode; 
   result = writeNTCAN(handle,2, &msg);
   if(result != 0) 
		{  msg.data[0] = l_stop;
			writeNTCAN(handle,2, &msg);
			return 1; }
	return 0;  
}


/***************************************************************/
/***************************************************************/
int32_t readLimb(NTCAN_HANDLE handle, command *myCmd)
{
	CMSG msg;
	int32_t result;

	msg.id = myCmd->digit + 0x100;
	msg.len = 2;   
	result = readNTCAN(handle,&msg,2);
	myCmd->mode = msg.data[1];
	myCmd->speed = msg.data[0]; 
	if(result != 0)
		{ return 1; }
	return 0;
} 
		 
/***************************************************************/
/***************************************************************/
int32_t openLimb(NTCAN_HANDLE handle, finger myDigit) 
{
	command myCmd;
	int32_t result;

	myCmd.digit = myDigit;
	myCmd.mode 	= l_open; 
	myCmd.speed = 200;
	result = writeLimb(handle, &myCmd);  
	if(result != 0)
		{ return 1; }
/* Get feedback until finger is open all the way */	
	do {
		result = readLimb(handle, &myCmd);
		if(result != 0)
			{ return 1; }
		} while (myCmd.mode != s_open);
/* Stop motor on finger */
	myCmd.mode = l_stop;
	myCmd.speed = 100; 
	result = writeLimb(handle, &myCmd);
	if(result != 0)
		{ return 1; }
/* Check to make sure motor is stopped */
	do {
		result = readLimb(handle, &myCmd);
		if(result != 0)
			{ return 1; }
		} while (myCmd.mode != l_stop);
	return 0; 
}
	 
int32_t closeLimb(NTCAN_HANDLE handle, finger myDigit) 
{
	command myCmd;
	int32_t result;

	myCmd.digit = myDigit;
	myCmd.mode 	= l_close; 
	myCmd.speed = 200;
	result = writeLimb(handle, &myCmd);  
	if(result != 0)
		{ return 1; }
/* Get feedback until finger is open all the way */	
	do {
		result = readLimb(handle, &myCmd);
		if(result != 0)
			{ return 1; }
		} while (myCmd.mode != s_close);
/* Stop motor on finger */
	myCmd.mode = l_stop;
	myCmd.speed = 100; 
	result = writeLimb(handle, &myCmd);
	if(result != 0)
		{ return 1; }
/* Check to make sure motor is stopped */
	do {
		result = readLimb(handle, &myCmd);
		if(result != 0)
			{ return 1; }
		} while (myCmd.mode != l_stop);
	return 0; 
}
	










