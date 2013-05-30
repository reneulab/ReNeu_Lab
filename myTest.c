/* Test code for simple communication with PCI200 */

#include <ntcan.h>
#include <stdio.h>

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

typedef struct
{
	uint32_t 	baud;
        int32_t		wIndex;
        int32_t		wLen;
        int32_t		rIndex;
        int32_t		rLen;
	NTCAN_HANDLE	handle;
        int32_t		ID[CMSG_SIZE]; 
	int32_t		rxTime;
	int32_t		txTime;
	CMSG		*obj[CMSG_SIZE];
} NTCAN;


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

   int32_t ret_val;
   NTCAN_RESULT new_result;
  
   switch(result) {
      case NTCAN_SUCCESS:
         ret_val = 0; break:
      case NTCAN_INVALID_PARAMETER:  
         printf("Error, passed invalid paramveter\n");
	 ret_val = 1; break; 
      case NTCAN_WRONG_DEVICE_STATE:
	   printf("Error, system is in sleep mode\n"); 
	   ret_val = 1; break;
      default: 
         if(command == CAN_OPEN) {
	    switch(result)
	       case NTCAN_INVALID_DRIVER:
 	          printf("Error, invalid driver\n");
                  ret_val = 1; break; 
	       case NTCAN_INVALID_FIRMWARE:
                  printf("Error, invalid firmware\n");
	          ret_val = 1; break;      	  
               case NTCAN_INVALID_HARDWARE:
                  printf("Error, incompatiable driver\n");
                  ret_val = 1; break;
	       case NTCAN_NET_NOT_FOUND:
		  printf("Error, no net found\n");
    		  ret_val = 1; break;
               case NTCAN_INSUFFICIENT_RESOURCES:
		  printf("Error, decrease handle queue\n"); 
                  ret_val = 1; break;  
	 }

         else if(command == CAN_ID_ADD) {
	    switch(result) {
	       case NTCAN_ID_ALREADY_ENABLED:
		  printf("Error, ID already enabled\n");
		  ret_val = 1; break;             
               case NTCAN_INSUFFICIENT_RESOURCES:
		  printf("Error, ID active with other handles\n"); 
                  ret_val = 1; break; }
         }
  	 else if(command == CAN_ID_DELETE) {
    	    switch(result) {
      	       case NTCAN_ID_NOT_ENABLE:
		  printf("Error, ID not enabled\n");
		  ret_val = 1; break; }
         }

 	 else if(command == CAN_IO_CLT) {
 	    switch(result) {
	       case NTCAN_NOT_IMPLEMENTED:
	          printf("Error, ulCommand error\n"); 
	          ret_val = 1; break; }
	 }

	 else if(command == CAN_WRITE) {
	    switch(result) {
	       case NTCAN_TX_ERROR:
		  printf("TX Error\n"); 
	          ret_val = 1; break; 
	       case NTCAN_TX_TIMEOUT:
		  printf("Error, TX timeout\n"); 
                  ret_val = 1; break; }
	 }
         if((command == CAN_WRITE) || (command == CAN_WRITE_T)) {
	    switch(result) {
	       case NTCAN_CONTR_ERR_PASSIVE:
		  printf("Error, REC or TEC value > 128\n");
                  ret_val = 2; break;
	       case NTCAN_CONTR_OFF_BUS
		  printf("Transmission Error\n"); 
		  ret_val = 2; break; 
	       case NTCAN_CONT_WARN:
                  printf("Reception Error\n"); 
		  ret_val = 2; break;
	       case NTCAN_ERROR_NO_BAUDRATE:
		  printf("Error, baudrate is not set\n");
                  new_result = canSetBaudrate(handle,NTCAN_AUTOBAUD);
                  if(errorCheck(handle,CAN_SET_BAUDRATE,
                                new_result) == 0) {
                     printf("Fixed baudrate\n");
                     ret_val = 2;
                  } else {
                  printf("Baudrate not fixed\n"); 
                  ret_val = 1; }
                  break; 
	       case NTCAN_ERROR_LOM:
		  printf("Error, CAN is listen only mode\n");
                  /* Turns listening mode off to fix error */
                  new_result = canSetBaudrate(handle,NTCAN_AUTOBAUD);
                  if(errorCheck(handle,CAN_SET_BAUDRATE,
                                new_result) == 0) {
                     printf("CAN now in listening mode\n");
                     ret_val = 2;
                  } else {
                  printf("Could not switch to listening mode\n");
                  ret_val = 1; } 
		  break; }
         }
         if( (command == CAN_SEND) || (command == CAN_SEND_EVENT) ||
             (command == CAN_ID_ADD) || (command == CAN_ID_DELETE)) {
            switch(result) {
	       case NTCAN_CONTR_BUSY: 
		  printf("Error, FIFO is full\n");
		  ret_val = 2; break; } 
         }
         if( (command == CAN_WRITE) || (command == CAN_WRITE_T) ||
            (command == CAN_READ) || (command == CAN_READ_T) ) {
            switch(result) {
	       case NTCAN_HANDLE_FORCED_CLOSE:
	   	  printf("Error, CAN was closed in another thread\n"); 
                  ret_val = 1; break;
            /* case NTCAN_IO_PENDING:
		  printf("Error, operation not terminated\n"
                  ret_val = 1; break;    only for windows */
               case NTCAN_OPERATION_ABORTED:
		  printf("Error, operation aborted\n")
		  ret_val = 1; break; }
         }
         if( command != CAN_OPEN) {
            switch(result) {
               case NTCAN_INVALID_HANDLE:
		  printf("Error, invalid hadnle") 
		  ret_val = 1; break; }
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
               case NTCAN_ID_NOT_ENABLE:
		  printf("Error, ID not enabled\n");
                  ret_val = 1; break;   
               case NTCAN_PENDING_READ:
  		  printf("Error, handle in use in about thread\n");
                  ret_val = 1; break; }
         }
         if( (command == CAN_IO_CLT) || (command == CAN_SET_BAUDRATE) ||
             (command == CAN_ID_ADD) || (command == CAN_ID_DELETE)) {
            switch(result) {
	       case NTCAN_NOT_SUPPORTED:
		  printf("Error CAN not supported\n");
                  ret_val = 1; break; }
         }
         if( (command == CAN_SEND) || (command == CAN_SEND_EVENT) ||
             (command == CAN_WRITE)) {
            switch(result) {
	       case NTCAN_PENDING_WRITE:
                  printf("Error, pending write\n");
                  ret_val = 1; break; }
         }
         if( (command == CAN_READ) || (command == CAN_READ_T) ||
             (command == CAN_READ_EVENT)) {
            switch(result) {
	       case NTCAN_RX_TIMEOUT:
	          printf("Error, RX timeout\n");
		  ret_val = 1; break; }
         }
      default:
         printf("Unknown Error"); 
         ret_val = 0xFF; break;
   }
   return ret_val; 
}

/****************************************************************/
/*        Function: initNTCAN					*/
/*   Creates NTCAN struct with CMSG_SIZE IDs and objects 	*/
/*   In: baudrate, mode flags, pointer to array of desired IDs  */
/*       net number, rx buffer size,rx timeout in ms,   	*/
/*       tx buffer size, tx timeout in ms, index for where	*/
/*       messages to be written/read start(first element 0),	*/
/*       max length of message to be read/written		*/
/*   Out: NUll pointer if error					*/
/*	  Pointer to NTCAN struct if successful			*/
/****************************************************************/
NTCAN* initNTCAN(uint32_t baud,  uint32_t flags,
		 int32_t *ID[]     int32_t net,
		 int32_t rxSize, int32_t rxTime,
		 int32_t txSize, int32_t txTime,
                 int32_t wIndex, int32_t wLen,
                 int32_t rIndex, int32_t rLen)
{
   NTCAN		*myNTCAN;  // Pointer to NTCAN device
   NTCAN_HANDLE		handle;    // 
   int32_t 		result;    // Used for checking for error
   int32_t		i;         // Index for multiple objects
   int32_t		timeout=0;   // Current number of timeouts
/* Open can */  
   result = canOpen(net,flags,txSize,rxSize,   // Opens device
                    txTime,rxTime,&handle);
   if(errorCheck(handle,CAN_OPEN,result) != 0);  // Error check
      { return NULL; } // returns NULL if error
   printf("canOpen() success\n");
/* Setting baudrate */   
   result = canSetBaudrate(handle,baud);   // sets baudrate
   if(errorCheck(handle,CAN_SET_BAUDRATE,result) != 0) // Error check
      { return NULL; } // returns NULL if error
   result = canGetBaudrate(handle,&baud);  // Reads buadrate
   printf("canSetBaudrate() success. Baudrate is %d\n",baud);
/* Adding an ID */
   for(i=0;i<CMSG_SIZE;i++) {  //  sets up CMSG_SIZE number of IDs
      do  { result = canIdAdd(handle,ID); // repeat if Timeout error
            timeout++;
            if(timeout>MAX_TIMOUT) {
               printf("Max timeout out reached, Aborting addID\n")
               return 2; }  // return if repeated error
          } while( errorCheck(handle,CAN_ID_ADD,result) == 2) 
      if(errorCheck(handle,CAN_ID_ADD,result) != 0) // error check
         { return NULL; }
   } 
   printf("canIdAdd() successful\n"); 
/* Making NTCAN Device */
   myNTCAN = malloc(sizeof(NTCAN)); // allocation space for device
   if(myNTCAN == NULL){   // memory check
      printf("Error, not enough memory\n");
      return NULL; }
   myNTCAN->baud = baud;     // Putting data into NTCAN Device Struct
   myNTCAN->handle = handle;
   myNTCAN->ID = ID;
   myNTCAN->rxTime = rxTime;
   myNTCAN->wIndex = wIndex;
   myNTCAN->wLen = wLen;
   myNTCAN->rIndex = rIndex;
   myNTCAN->rLen = rLen;
/* Creating CMSG_SIZE number of objects for NTCAN device */
   for(i=0; i<CMSG_SIZE; i++) {
      myNTCAN->obj[i] = malloc(sizeof(CMSG)); // allocating space 
      if(myNTCAN->obj[i] == NULL){  // memeory check
         printf("Not enough memory for object %d\n",i); 
         return NULL; }
      result = canIoctl(myNTCAN->handle, NTCAN_IOCTL_TX_OBJ_CREATE,
	                myNTCAN->obj[i]); // creating CMSG_SIZE
      if(errorCheck(handle,CAN_IO_CTL,result) != 0)  // objects
         { return NULL; }  // error check
   }
   printf("Initializing sucessfull\n");
/* Flushing FIFO buffer */
   result = canIoctl(myNTCAN->handle,NTCAN_IOCTL_FLUSH_RX_FIFO,NULL);   
   if(errorCheck(handle,NTCAN_IO_CTL,result) != 0) // flushing FIFO
      { return NULL; }  // error check
   printf("System flushed, device ready to use\n");
   return myNTCAN; 
}


/*************************************************************/
int32_t closeNTCAN(NTCAN *myNTCAN)
{
   NTCAN_HANLDE tempHandle; 
   NTCAN_RESULT	result;
/* Flushing system */  
   result = canIoctl(myNTCAN->handle,NTCAN_IOCTL_FLUSH_RX_FIFO,NULL);   
   if(errorCheck(handle,NTCAN_IO_CTL,result) != 0) 
      { return -1; }
/* Deleting NTCAN object */   
   result = canIoctl(myNTCAN->handle, NTCAN_IOCTL_TX_OBJ_DESTROY,
                     myNTCAN->ID); 
   if(errorCheck(handle,NTCAN_IO_CTL,result) != 0) 
      { return 1; }  
   printf("Deleted NTCAN object\n");
/* Deleting ID */      
   do  { result = canIdDelete(myNTCAN->handle,myNTCAN->ID); }
      while( errorCheck(myNTCAN->handle,CAN_ID_DELETE,result) == 2)
   if(errorCheck(myNTCAN->handle,CAN_ID_DELETE,result) != 0)
      { return 1; }
   printf("Deleted NTCAN ID\n");
/* Deallocating memory from myNTCAN and myNTCAN->obj */
   for(i=0;i<CMSG_SIZE;i++)
      { free(myNTCAN->obj[i]); }
   tempHandle = myNTCAN->handle;
   free(myNTCAN); 
/* Closing NTCAN device */
   result = canClose(tempHandle);
   if(errorCheck(tempHandle,CAN_CLOSE,result) != 0)
      { return 1; }
   printf("Deleted NTCAN device. Device successfully closed.\n");
   return 0;
}

/****************************************************************/
int32_t readNTCAN(NTCAN *myNTCAN)
 {
   NTCAN_RESULT 	result;
   int32_t		i;
   int32_t 		timeout; 
/* Updating Object of NTCAN device */
   for(j=myNTCAN->rIndex;j<(myNTCAN->rLen + myNTCAN->rIndex);j++) 
   {
      result = canIoctl(myNTCAN->handle, NTCAN_IOCTL_TX_OBJ_UPDATE,
                        myNTCAN->obj[j]); 
      if(errorCheck(NTCAN->handle,CAN_IO_CTL,result) != 0) 
         { return 1; }
/* Reading Object of NTCAN device */  
      do { result = canRead(myNTCAN->handle, myNTCAN->obj[j],
                         &(myNTCAN->obj[j]->len), NULL);
         timeout++;
/* If timeout error is recieved repeatly then read is aborted */
         if(timeout > MAX_TIMEOUT) {
            result = canToctl(myNTCAN->handle,
                              NTCAN_IOCTL_ABORT_RX, NULL);
            if(errorCheck(NTCAN->handle,CAN_IO_CTL,result != 0))
              { return 1; } 
            printf("Repeated Timeout, read aborted"\n);
            return 2; }
      }
      while(errorCheck(NTCAN->handle,CAN_READ,result) == 2); 
      if(errorCheck(NTCAN->handle,CAN_READ,result) != 0)
         { return 1; }  
/* Printing read object of NTCAN device to screen */
      printf("readNTCAN() successfull\n") ;
      printf("ID of NTCAN device: %d\n", myNTCAN->obj[j]->id);
      printf("Length of message recieved: %d\n",
              (myNTCAN->obj[j]->len & 0x0F) );
      for(i=0;i<(myNTCAN->obj[j]->len & 0x0F);i++) {
         printf("Byte %d of recieved message: %d\n", i, 
                 myNTCAN->obj[j]->data[i]);
      }
   } 
   return 0; 
 }


/***************************************************************/
int32_t writeNTCAN(NTCAN *myNTCAN) 
{
   NTCAN_RESULT		result;
   int32_t 		i;
   for(i=myNTCAN->wIndex;i<(myNTCAN->wLen + myNTCAN->wIndex);i++)
   {
      myNTCAN->obj[i]->len |= myNTCAN->obj[i]->len + (RTR_ENABLE<<4);
      result = canWrite(myNTCAN->handle, myNTCAN->obj[i],
                        &(myNTCAN->obj[i]->len), NULL); 
      if(errorCheck(NTCAN->handle,CAN_WRITE,result) =! 0)
         { return 1; }
      printf("Message sent %d successful frames\n",
              myNTCAN->obj[i]->len);
      return 0;
   } 
}

#define CMSG_SIZE	1
#define RTR_ENABLE	0

int32_t main(void) {
   int32_t		ret_val = 0; 
   NTCAN_RESULT 	result;
   int32_t		check;  
   int32_t		net = NTCAN_MAX_NET;
   uint32_t		flags = NTCAN_MODE_OBJECT;
   int32_t		tx_size = NTCAN_MAX_TX_QUEUESIZE;
   int32_t		rx_size = NTCAN_MAX_TX_QUEUESIZE;
   int32_t		tx_time = 1000;
   int32_t		rx_time = 1000;
   NTCAN_HANDLE		*handle;  
   uint32_t		baud = NTCAN_AUTOBAUD;
   int32_t 		ID = 0;  
/* Open can */ 
   handle = malloc(sizeof(NTCAN_HANDLE)); 
   result = canOpen(net,flags,tx_size,rx_size,
                         tx_time,rx_time,*handle);
   if(errorCheck(handle,CAN_OPEN,result) != 0);  
      { return -1; } 
   printf("canOpen() success\n");
/* Setting baudrate */   
   result = canSetBaudrate(handle,baud); 
   if(errorCheck(handle,CAN_SET_BAUDRATE,result) != 0) 
      { return -1; }
   result = canGetBaudrate(handle,&baud); 
   printf("canSetBaudrate() success. Baudrate is %d\n",baud);
/* Adding an ID */
   do  { result = canIdAdd(handle,ID); }
      while( errorCheck(handle,CAN_ID_ADD,result) == 2)
   if(errorCheck(handle,CAN_ID_ADD,result) != 0)
      { return -1; }   
}




