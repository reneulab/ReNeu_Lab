/*************************************************/
#include "limb.h"
#include "myCan.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "NiFpga_ButtonFPGA.h"
#include "NiFpga.h"

typedef struct {
	NiFpga_Session session;
	NiFpga_Bool DIO0;
	NiFpga_Bool DIO1;
	NiFpga_Bool DIO2;
	NiFpga_Bool DIO3;
	NiFpga_Bool DIO4;
} niData; 

void* niThread(void *niSwitch);

void* myThread(void *arg1); 

void* niThread(void *mySwitch)
{
	 
   	NiFpga_ReadBool((*(niData*)mySwitch).session,NiFpga_thumb, &((*(niData*)mySwitch).DIO0));
	NiFpga_ReadBool((*(niData*)mySwitch).session,NiFpga_index, &((*(niData*)mySwitch).DIO1));
	NiFpga_ReadBool((*(niData*)mySwitch).session,NiFpga_middle,&((*(niData*)mySwitch).DIO2));
	NiFpga_ReadBool((*(niData*)mySwitch).session,NiFpga_ring,  &((*(niData*)mySwitch).DIO3));
	NiFpga_ReadBool((*(niData*)mySwitch).session,NiFpga_pinkie,&((*(niData*)mySwitch).DIO4));
   
/*	(((niData*)mySwitch)->DIO0) = 1; 
	(((niData*)mySwitch)->DIO1) = 1; 
	(((niData*)mySwitch)->DIO2) = 1; 
	(((niData*)mySwitch)->DIO3) = 1; 
	(((niData*)mySwitch)->DIO4) = 1; */
	pthread_exit(NULL);

}

void* myThread(void *arg1) 
{
  	int32_t		result;
	arg 		myArg;

	myArg = (*((arg *) arg1));
	result = writeLimb(myArg.handle,&(myArg.cmd));
	usleep(myArg.time);

	myArg.cmd.mode = l_stop;
	result = writeLimb(myArg.handle,&(myArg.cmd));
	usleep(20000);

// Closing //  
	pthread_exit(NULL);  
  	return 0; 
}


int32_t main(void)
{
	pthread_t* 	thumb_T,index_T,middle_T,ring_T,pinkie_T,switch_T;
	arg 	  	myThumb,myIndex,myMiddle,myRing,myPinkie;
	int32_t 	ret0,ret1,ret2,ret3,ret4,ret5;
	niData 		mySwitch; 
	int32_t		i=0;
/* Init thread pointers*/
	thumb_T  = malloc(sizeof(pthread_t)); 
	index_T  = malloc(sizeof(pthread_t)); 
	middle_T  = malloc(sizeof(pthread_t)); 
	ring_T  = malloc(sizeof(pthread_t)); 
	pinkie_T  = malloc(sizeof(pthread_t)); 
	switch_T  = malloc(sizeof(pthread_t)); 
/* Init  hand */
	myThumb.handle = initLimb(thumb, &myThumb.cmd);
	if(myThumb.handle == 0xFFFF) 
		{ printf("Error in thumb init\n"); return 1; } 
	usleep(20000);

	myIndex.handle = initLimb(index, &myIndex.cmd);
	if(myIndex.handle == 0xFFFF) 
		{ printf("Error in index init\n"); return 1; } 
	usleep(20000);

	myMiddle.handle = initLimb(middle, &myMiddle.cmd);
	if(myMiddle.handle == 0xFFFF) 
		{ printf("Error in middle init\n"); return 1; } 
	usleep(20000);

	myRing.handle = initLimb(ring, &myRing.cmd);
	if(myRing.handle == 0xFFFF) 
		{ printf("Error in ring init\n"); return 1; } 
	usleep(20000);

	myPinkie.handle = initLimb(pinkie, &myPinkie.cmd);
	if(myPinkie.handle == 0xFFFF) 
		{ printf("Error in pinkie init\n"); return 1; } 
	usleep(20000);
/* Init FPGA */
   	printf("Initializing...\n");
  	NiFpga_Status status = NiFpga_Initialize();
   	if (NiFpga_IsNotError(status))
   	{
/* opens a session, downloads the bitstream, and runs the FPGA */
      	printf("Opening a session...\n");
      	NiFpga_MergeStatus(&status, NiFpga_Open(Bitfile,
                                              NiFpga_FPGA_Signature,
                                              "rio://146.6.84.41/RIO0",
                                              NiFpga_OpenAttribute_NoRun,
                                              &(mySwitch.session)));
		printf("Session Status: %i\n",status);
      	if (NiFpga_IsNotError(status))
      	{ /* run the FPGA application */
         	printf("Running the FPGA...\n");
         	NiFpga_MergeStatus(&status, NiFpga_Run(mySwitch.session, 0));

/* Setting values */ 
	mySwitch.DIO0 = 0;  
	mySwitch.DIO1 = 0;  
	mySwitch.DIO2 = 0;  
	mySwitch.DIO3 = 0;  
	mySwitch.DIO4 = 0;
  
	myThumb.digit = thumb;
	myThumb.time  = 100000;
	myThumb.cmd.speed = 550;
	myThumb.cmd.mode = l_stop;
	
	myIndex.digit = index;
	myIndex.time  = 100000;
	myIndex.cmd.speed = 550;
	myIndex.cmd.mode = l_stop;
	
	myMiddle.digit = middle;
	myMiddle.time  = 100000;
	myMiddle.cmd.speed = 550;
	myMiddle.cmd.mode = l_stop;
	
	myRing.digit = ring;
	myRing.time  = 100000;
	myRing.cmd.speed = 550;
	myRing.cmd.mode = l_stop;
	
	myPinkie.digit = pinkie;
	myPinkie.time  = 100000;
	myPinkie.cmd.speed = 550;
	myPinkie.cmd.mode = l_stop;

	printf(" About to start\n"); 	
/* intresting part */
	for(i=0;i<500;i++)
	{
	printf("Loop: %d\n",i);	
	ret0 = pthread_create( &switch_T,  NULL, niThread, &mySwitch);
	pthread_join(switch_T,NULL);
	printf("Thread mySwitch returns: %d\n",ret1);
 
    if(mySwitch.DIO0 == 0)
		{ myThumb.cmd.mode = l_close; } 
    else if(mySwitch.DIO0 == 1)
		{ myThumb.cmd.mode = l_open; }

	if(mySwitch.DIO1 == 0)
		{ myIndex.cmd.mode = l_close; }
	else { myIndex.cmd.mode = l_open; }
    
	if(mySwitch.DIO2 == 0)
		{ myMiddle.cmd.mode = l_close; }
	else { myMiddle.cmd.mode = l_open; }
    
	if(mySwitch.DIO3 == 0)
		{ myRing.cmd.mode = l_close; }
	else { myRing.cmd.mode = l_open; }
    
	if(mySwitch.DIO4 == 0)
		{ myPinkie.cmd.mode = l_close; }
	else { myPinkie.cmd.mode = l_open; }

	ret1 = pthread_create( &thumb_T,  NULL, myThread, &myThumb);
	ret2 = pthread_create( &index_T,  NULL, myThread, &myIndex);
	ret3 = pthread_create( &middle_T, NULL, myThread, &myMiddle);
	ret4 = pthread_create( &ring_T,   NULL, myThread, &myRing);
	ret5 = pthread_create( &pinkie_T, NULL, myThread, &myPinkie);
	pthread_join(thumb_T,NULL);
	pthread_join(index_T,NULL);
	pthread_join(middle_T,NULL);
	pthread_join(ring_T,NULL);
	pthread_join(pinkie_T,NULL);
	printf("DIO0: %d\n",mySwitch.DIO0); 
	printf("DIO1: %d\n",mySwitch.DIO1); 
	printf("DIO2: %d\n",mySwitch.DIO2); 
	printf("DIO3: %d\n",mySwitch.DIO3); 
	printf("DIO4: %d\n",mySwitch.DIO4);
	}

    usleep(40000); 
	myThumb.cmd.mode = l_stop;	
	myIndex.cmd.mode = l_stop;	
	myMiddle.cmd.mode = l_stop;	
	myRing.cmd.mode = l_stop;	
	myPinkie.cmd.mode = l_stop;	
	ret1 = pthread_create( &thumb_T,  NULL, myThread, &myThumb);
	ret2 = pthread_create( &index_T,  NULL, myThread, &myIndex);
	ret3 = pthread_create( &middle_T, NULL, myThread, &myMiddle);
	ret4 = pthread_create( &ring_T,   NULL, myThread, &myRing);
	ret5 = pthread_create( &pinkie_T, NULL, myThread, &myPinkie);
	pthread_join(thumb_T,NULL);
	pthread_join(index_T,NULL);
	pthread_join(middle_T,NULL);
	pthread_join(ring_T,NULL);
	pthread_join(pinkie_T,NULL);
	printf("Thread 1 returns: %d\n",ret1); 
	printf("Thread 2 returns: %d\n",ret2); 
	printf("Thread 3 returns: %d\n",ret3); 
	printf("Thread 4 returns: %d\n",ret4); 
	printf("Thread 5 returns: %d\n",ret5);

/* Closing NI FPGA */
		printf("Stopping the FPGA...\n");
	    NiFpga_MergeStatus(&status, NiFpga_WriteU32(mySwitch.session, 
						NiFpga_stop,1));
		/* close the session now that we're done */
	    printf("Closing the session...\n");
	    NiFpga_MergeStatus(&status, NiFpga_Close(mySwitch.session, 0));
	    }
      /* must be called after all other calls */
      printf("Finalizing...\n");
      NiFpga_MergeStatus(&status, NiFpga_Finalize());
   }
   /* check if anything went wrong */
   if (NiFpga_IsError(status))
   {
      printf("Error %d!\n", status);
      printf("Press <Enter> to quit...\n");
      getchar();
   }
   return status;
/* Closing limb hand */
	closeLimb(myThumb.handle,myThumb.digit); 
	closeLimb(myIndex.handle,myIndex.digit); 
	closeLimb(myMiddle.handle,myMiddle.digit); 
	closeLimb(myRing.handle,myRing.digit); 
	
	closeLimb(myPinkie.handle,myPinkie.digit); 
	printf("Successful uses :]\n");
	pthread_exit(NULL);  
}


