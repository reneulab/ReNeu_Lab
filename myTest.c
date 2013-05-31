/* Test code for simple communication with PCI200 */

#include "ntcan.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* Used for error checker */
#define CAN_SET_BAUDRATE		1
#define CAN_GET_BAUDRATE 		2
#define CAN_OPEN	 		3
#define CAN_CLOSE			4
#define CAN_ID_ADD			5
#define CAN_ID_DELETE			6
#define CAN_GET_OVERLAPPED_RESULT 	7
#define CAN_TAKE			8
#define CAN_READ			9
#define CAN_SEND			10
#define CAN_WRITE			11
#define CAN_SENT_T			12
#define CAN_WRITE_T			13
#define CAN_READ_EVENT			14
#define CAN_SEND_EVENT			15
#define CAN_STATUS			16
#define CAN_IO_CTL			17
#define CAN_TAKE_T 			18
#define CAN_READ_T			19
#define CAN_FORMAT_ERROR		20
#define CAN_FORMAT_EVENT		21

#define CMSG_SIZE	1
#define RTR_ENABLE	0
#define MAX_TIMEOUT     50

typedef struct
{
	uint32_t 	baud;   // baudrate of device
	NTCAN_HANDLE	handle; // Handle for NTCAN device
	int32_t		rxTime; // Timeout time in ms for rx
	int32_t		txTime; // Timeout time in ms for tx
	CMSG		obj[CMSG_SIZE]; // pointer to array of CMSG 
} NTCAN;                                 // structs


/****************************************************************/
/*             Function: errorCheck 				*/
/*    command is one of the funtions in ntcan.h                 */
/*    Notation for command is in #define                        */
/*    Out: 1 if error, system should shutdown                   */
/*         2 if error, wait and retry                           */
/*   (only if command canWrite,canWrite_t,canSend               */
/*     canSendEvent, canIdAdd,canIdDelete)                      */
/*					                        */
/*         0 if success :-)                                     */      
/*         0xFF if unknown error                                */
/****************************************************************/
int32_t errorCheck( int32_t command,
                   NTCAN_RESULT result) {

   printf("command: %d,result: %d\n",command,result);
   switch(result) {
      case NTCAN_SUCCESS: 
         return 0; break;
      case NTCAN_INVALID_PARAMETER:  
         printf("Error, passed invalid parameter\n");
	 return 1; break; 
      case NTCAN_WRONG_DEVICE_STATE:
	   printf("Error, system is in sleep mode\n"); 
	   return 1; break;
      default: 
         if(command == CAN_OPEN) {
	    switch(result)
	       case NTCAN_INVALID_DRIVER:
 	          printf("Error, invalid driver\n");
                  return 1; break; 
	       case NTCAN_INVALID_FIRMWARE:
                  printf("Error, invalid firmware\n");
	          return 1; break;      	  
               case NTCAN_INVALID_HARDWARE:
                  printf("Error, incompatiable driver\n");
                   return 1; break;
	       case NTCAN_NET_NOT_FOUND:
		  printf("Error, no net found\n");
    		  return 1; break;
               case NTCAN_INSUFFICIENT_RESOURCES:
		  printf("Error, decrease handle queue\n"); 
                  return 1; break;  
	 }

         else if(command == CAN_ID_ADD) {
	    switch(result) {
	       case NTCAN_ID_ALREADY_ENABLED:
		  printf("Error, ID already enabled\n");
		  return 1; break;             
               case NTCAN_INSUFFICIENT_RESOURCES:
		  printf("Error, ID active with other handles\n"); 
                  return 1; break; }
         }
  	 else if(command == CAN_ID_DELETE) {
    	    switch(result) {
      	       case NTCAN_ID_NOT_ENABLED:
		  printf("Error, ID not enabled\n");
		  return 1; break; }
         }

 	 else if(command == CAN_IO_CTL) {
 	    switch(result) {
	       case NTCAN_NOT_IMPLEMENTED:
	          printf("Error, ulCommand error\n"); 
	          return 1; break; }
	 }

	 else if(command == CAN_WRITE) {
	    switch(result) {
	       case NTCAN_TX_ERROR:
		  printf("TX Error\n"); 
	          return 1; break; 
	       case NTCAN_TX_TIMEOUT:
		  printf("Error, TX timeout\n"); 
                  return 1; break; }
	 }
         if((command == CAN_WRITE) || (command == CAN_WRITE_T)) {
	    switch(result) {
	       case NTCAN_CONTR_ERR_PASSIVE:
		  printf("Error, REC or TEC value > 128\n");
                  return 2; break;
	       case NTCAN_CONTR_OFF_BUS:
		  printf("Transmission Error\n"); 
		  return 2; break; 
	       case NTCAN_CONTR_WARN:
                  printf("Reception Error\n"); 
		  return 2; break;
	       case NTCAN_ERROR_NO_BAUDRATE:
		  printf("Error, baudrate is not set\n");
                  return 1; break; 
	       case NTCAN_ERROR_LOM:
		  printf("Error, CAN is listen only mode\n");
                  return 1; } 
		  break;
         }
         if( (command == CAN_SEND) || 
             (command == CAN_SEND_EVENT) ||
             (command == CAN_ID_ADD) || 
             (command == CAN_ID_DELETE)) {
            switch(result) {
	       case NTCAN_CONTR_BUSY: 
		  printf("Error, FIFO is full\n");
		  return 2; break; } 
         }
         if( (command == CAN_WRITE) || (command == CAN_WRITE_T) ||
            (command == CAN_READ) || (command == CAN_READ_T) ) {
            switch(result) {
	       case NTCAN_HANDLE_FORCED_CLOSE:
	   	  printf("Error, CAN was closed in another thread\n"); 
                  return 1; break;
            /* case NTCAN_IO_PENDING:
		  printf("Error, operation not terminated\n"
                  ret_val = 1; break;    only for windows */
               case NTCAN_OPERATION_ABORTED:
		  printf("Error, operation aborted\n");
		  return 1; break; }
         }
         if( command != CAN_OPEN) {
            switch(result) {
               case NTCAN_INVALID_HANDLE:
		  printf("Error, invalid hadnle"); 
		  return 1; break; }
         }
      /* if( (command == CAN_GET_OVERLAPPED_RESULT) || 
             (command == CAN_GET_OVERLAPPED_RESULT_T)) {
            switch(result) {
               case NTCAN_IO_INCOMPLETE:
                 printf("Error, operation not teminated\n")
                 ret_val = 1; break; } 
         } only for windows */
         if( (command == CAN_TAKE) || (command == CAN_TAKE_T) ||
             (command == CAN_READ) || (command == CAN_READ_T) ) {
            switch(result) { 
               case NTCAN_ID_NOT_ENABLED:
		  printf("Error, ID not enabled\n");
                  return 1; break;   
               case NTCAN_PENDING_READ:
  		  printf("Error, handle in use in about thread\n");
                  return 1; break; }
         }
         if( (command == CAN_IO_CTL) ||
             (command == CAN_SET_BAUDRATE) ||
             (command == CAN_ID_ADD) ||
             (command == CAN_ID_DELETE)) {
            switch(result) {
	       case NTCAN_NOT_SUPPORTED:
		  printf("Error CAN not supported\n");
                  return 1; break; }
         }
         if( (command == CAN_SEND) || (command == CAN_SEND_EVENT) ||
             (command == CAN_WRITE)) {
            switch(result) {
	       case NTCAN_PENDING_WRITE:
                  printf("Error, pending write\n");
                  return 1; break; }
         }
         if( (command == CAN_READ) || (command == CAN_READ_T) ||
             (command == CAN_READ_EVENT)) {
            switch(result) {
	       case NTCAN_RX_TIMEOUT:
	          printf("Error, RX timeout\n");
		  return 1; break; }
         }
         
   }
   printf("Unknown Error"); 
   return 1; 
}

/****************************************************************/
/*        Function: initNTCAN					*/
/*   Creates NTCAN struct with CMSG_SIZE IDs and objects 	*/
/*   In: baudrate, mode flags, pointer to array of desired IDs  */
/*       net number, rx buffer size,rx timeout in ms,   	*/
/*       tx buffer size, tx timeout in ms			*/
/*   Out: NUll pointer if error					*/
/*	  Pointer to NTCAN struct if successful			*/
/****************************************************************/
NTCAN* initNTCAN(uint32_t baud,  uint32_t flags,
		 int32_t ID[],    int32_t net,
		 int32_t rxSize, int32_t rxTime,
		 int32_t txSize, int32_t txTime)
{
   NTCAN		*myNTCAN;  // Pointer to NTCAN device
   NTCAN_HANDLE		handle;    // 
   NTCAN_RESULT		result;    // Used for checking for error
   int32_t		i;         // Index for multiple objects
   int32_t		timeout=0;   // Current number of timeouts
/* Open can */  
   result = canOpen(net,flags,txSize,rxSize,   // Opens device
                    txTime,rxTime,&handle);
   if(errorCheck(CAN_OPEN,result) != 0)  // Error check
      { return NULL; } // returns NULL if error
   printf("canOpen() success\n");
/* Setting baudrate */   
   result = canSetBaudrate(handle,baud);   // sets baudrate
   if(errorCheck(CAN_SET_BAUDRATE,result) != 0) // Error check
      { return NULL; } // returns NULL if error
   result = canGetBaudrate(handle,&baud);  // Reads buadrate
   printf("canSetBaudrate() success. Baudrate is %d\n",baud);

/* Making NTCAN Device */
   myNTCAN =  malloc(sizeof(NTCAN)); // allocation space for device
   if(myNTCAN == NULL){   // memory check
      printf("Error, not enough memory\n");
      return NULL; }
   myNTCAN->baud = baud;     
   myNTCAN->handle = handle;
   myNTCAN->rxTime = rxTime;
   myNTCAN->txTime = txTime;
/* Creating CMSG_SIZE number of objects for NTCAN device */
   for(i=0; i<CMSG_SIZE; i++) {
      myNTCAN->obj[i] = malloc(sizeof(CMSG)); // allocating space 
      if(myNTCAN->obj[i] == NULL){  // memeory check
         printf("Not enough memory for object %d\n",i); 
         return NULL; }
      myNTCAN->obj[i].id = ID[i]; 
      result = canIoctl(myNTCAN->handle, NTCAN_IOCTL_TX_OBJ_CREATE,
	                myNTCAN->obj[i]); // creating CMSG_SIZE
      if(errorCheck(CAN_IO_CTL,result) != 0)  // objects
         { return NULL; }  // error check 
/* Adding an ID */
      do  { result = canIdAdd(handle,myNTCAN->obj[i].id);
            // repeat if Timeout 
            timeout++;
            if(timeout>MAX_TIMEOUT) {
               printf("Max timeout out reached, Aborting addID\n");
               return NULL;}  // return if repeated error
          } while( errorCheck(CAN_ID_ADD,result) == 2); 
      if(errorCheck(CAN_ID_ADD,result) != 0) // error check
         { return NULL; }
   printf("canIdAdd() successful\n"); 
   }
   printf("Initializing sucessfull\n");
/* Flushing FIFO buffer */
   result = canIoctl(myNTCAN->handle,NTCAN_IOCTL_FLUSH_RX_FIFO,NULL);   
   if(errorCheck(CAN_IO_CTL,result) != 0) // flushing FIFO
      { return NULL; }  // error check
   printf("System flushed, device ready to use\n");
   return myNTCAN; 
}

/****************************************************************/
/*		Function: closeNTCAN				*/
/*   Frees memory used by a NTCAN devices, IDs and its objects	*/
/*   and closes the NTCAN device				*/
/*       In: pointer to a NTCAN device				*/
/*       Out: 0 is successful, 1 if error			*/
/*   Warning: Device can not be used after this function 	*/
/*	      unless reinitialized				*/
/****************************************************************/
int32_t closeNTCAN(NTCAN * myNTCAN)
{
   NTCAN_HANDLE		tempHandle; 
   int32_t		*tempID;
   NTCAN_RESULT		result;
   int32_t 		timeout = 0;
   int32_t		i;
   CMSG                 *ptr;

   tempHandle = myNTCAN->handle;
   for(i=0;i<CMSG_SIZE;i++); // iterates through all ojects and IDs
   {
      printf("ID:%d\n",myNTCAN->obj[i].id); 
/* Flushing system */  
      result = canIoctl(tempHandle, 
                        NTCAN_IOCTL_FLUSH_RX_FIFO,NULL);   
      if(errorCheck(CAN_IO_CTL,result) != 0) // error check 
         { return 1; }
      printf("System Flushed\n");
/* Deleting NTCAN object */   
      result = canIoctl(tempHandle, NTCAN_IOCTL_TX_OBJ_DESTROY,
                        &(myNTCAN->obj[i].id));// destroys object
      if(errorCheck(CAN_IO_CTL,result) != 0) // error check
         { return 1; }  
      printf("Deleted NTCAN object\n");  
/* Deleting ID */
      // Deletes all ids and retrys if timeout error       
      do  { result = canIdDelete(tempHandle,myNTCAN->obj[i].id); 
            timeout++;
            if(timeout>MAX_TIMEOUT) {   // checks for max timeout
               printf("Max timeout reached, aborting\n");
               return 2; }
      } while( errorCheck(CAN_ID_DELETE,result) == 2);
      if(errorCheck(CAN_ID_DELETE,result) != 0)  // error check
         { return 1; }
      printf("Deleted NTCAN ID\n");
/* Deallocating memory from myNTCAN and myNTCAN->obj */
      free(myNTCAN->obj[i]); // free's all objects memory
   }
   free(myNTCAN); // free NTCAN device memory
/* Closing NTCAN device */
   result = canClose(tempHandle);  // closes device 
   if(errorCheck(CAN_CLOSE,result) != 0) // error check
      { return 1; }
   printf("Deleted NTCAN device. Device successfully closed.\n");
   return 0;
}
/****************************************************************/
/*        Function: readNTCAN					*/
/*   Reads values from a NTCAN devices and stores data in 	*/
/*   NTCAN->obj->struct	 read object rIndex to rIndex+rLen-1    */
/*      In:  Pointer to NTCAN device				*/
/*           j is index of object in NTCAN device (0 is first   */
/*      Out: 0 if successfull, 1 if error			*/
/* Warning: make sure you set rIndex and rLen to desired	*/
/****************************************************************/
int32_t readNTCAN(NTCAN *myNTCAN, int32_t j)
 {
   NTCAN_RESULT 	result;
   int32_t		i;
   int32_t 		timeout;
   int32_t 		len;  
/* Iterates through all read objects of NTCAN device */ 
   len = myNTCAN->obj[j].len; 
/* Reading Object of NTCAN device */  
      do { result = canRead(myNTCAN->handle,myNTCAN->obj[j],
                         &len, NULL); 
         timeout++;
/* If timeout error is recieved repeatly then read is aborted */
         if(timeout > MAX_TIMEOUT) {
            result = canIoctl(myNTCAN->handle,
                              NTCAN_IOCTL_ABORT_RX, NULL);
            if(errorCheck(CAN_IO_CTL,result != 0))
              { return 1; } // error check
            printf("Repeated Timeout, read aborted\n");
            return 2; }
      } while(errorCheck(CAN_READ,result) == 2); 
      if(errorCheck(CAN_READ,result) != 0)
         { return 1; }  // error check
/* Printing read object of NTCAN device to screen */
      printf("readNTCAN() successfull\n") ;
      printf("ID of NTCAN device: %d\n", myNTCAN->obj[j].id);
      printf("Length of message recieved: %d\n", (len & 0x0F) );
      for(i=0;i<(myNTCAN->obj[j].len & 0x0F);i++) {
         printf("Byte %d of recieved message: %d\n", i, 
                 myNTCAN->obj[j].data[i]);
      } 
   return 0; 
 }

/****************************************************************/
/*                    Function: writeNTCAN			*/
/*     writes values in NTCAN->obj->data to NTCAN device	*/
/*            In: pointer to NTCAN device 			*/
/*                i is index of object to write to (0 is first) */
/*            Out: 0 if successfull, 1 if error			*/
/* Warning: Set txIndex and txLen to ojects you want to write to*/	
/****************************************************************/
int32_t writeNTCAN(NTCAN *myNTCAN, int32_t i) 
{
   NTCAN_RESULT		result;
   int32_t		len;
   len = myNTCAN->obj[i]->len;
/* Writes to NTCAN device */
   myNTCAN->obj[i].len |= myNTCAN->obj[i].len + (RTR_ENABLE<<4);
   result = canWrite(myNTCAN->handle,myNTCAN->obj[i],
                        &len, NULL); 
   if(errorCheck(CAN_WRITE,result) != 0)
      { return 1; }
   myNTCAN->obj[i].len = len; 
   printf("Message sent %d successful frames\n",
              myNTCAN->obj[i].len);
   return 0; 
}


int32_t main(void) {
   NTCAN	 *myNT;
   int32_t	result;
   int32_t      ID[1] = {0x601}; 
/* Open can */
   myNT = initNTCAN(NTCAN_AUTOBAUD,  NTCAN_MODE_NO_INTERACTION,
		 ID, 0, NTCAN_MAX_RX_QUEUESIZE,1000,
		 NTCAN_MAX_TX_QUEUESIZE, 1000);
   if(myNT == NULL) {
   printf("Error in open\n");
   return 1; }
/* close Can */
   result = closeNTCAN(myNT);
   if(result != 0) { 
   printf("Error in close"); 
   return 1; }
   printf("Successful uses :]\n"); 
}




