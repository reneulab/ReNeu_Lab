/*************************************************/
#include "limb.h"
#include "myCan.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "NiFpga_ButtonFPGA.h"
#include "NiFpga.h"

typedef struct {
	NiFpga_Session session;
	NiFpga_Bool DIO[5];
} niData; 

 pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER; 

 pthread_cond_t  myCond[5];

int32_t endCond; 

niData mySwitch;

void catch_signal(int sig)
{

	printf("AMQZ, User panicked! ");
	endCond = 1;
	return; 
}

void* niRead(void *data)
{
	while (endCond==0)
	{
		NiFpga_ReadArrayBool(mySwitch.session,NiFpga_input,mySwitch.DIO,5);
		usleep(1000);
	}
	pthread_exit(NULL); 
} 

void* sendCmd(void *arg1) 
{
  	int32_t		result;
	arg 		myArg;
	int32_t		index=0;
	int32_t 	lastState=1;
	int32_t 	change; 

	myArg = (*((arg *) arg1));
	index = findFinger(myArg.digit);  
	
	while (endCond==0)
	{ 
		do {
			//printf("DIO%d:%d,Last:%d\n",index,mySwitch.DIO[index],lastState);
			lastState = mySwitch.DIO[index]; 
		} while((mySwitch.DIO[index] == lastState) && endCond==0); 
	
		if(mySwitch.DIO[index] == 0)
		{ 
			myArg.cmd.mode = l_stop;
			writeLimb(myArg.handle,&(myArg.cmd));
			usleep(5000);
			myArg.cmd.mode = l_close; 
		} 
    	else //if(mySwitch.DIO[index] == 1)
		{ 
			myArg.cmd.mode = l_stop;
			writeLimb(myArg.handle,&(myArg.cmd));
			usleep(5000);
			myArg.cmd.mode = l_open; 
		}
		
		result = writeLimb(myArg.handle,&(myArg.cmd));
		usleep(5000);
	}
// Closing //  
	pthread_exit(NULL);  
}


int32_t main(void)
{
	
   	pthread_t* 	thumb_T,index_T,middle_T,ring_T,pinkie_T;
	pthread_t*	ni_T;
	pthread_t* pthreadPT_index[6] = {thumb_T, index_T, middle_T, ring_T, pinkie_T,ni_T};
	arg 	  	myThumb,myIndex,myMiddle,myRing,myPinkie;
	int32_t 	ret[6];
	int32_t		i=0;

	signal(SIGINT,catch_signal);
	signal(SIGTERM,catch_signal);

/* Init thread pointers*/
	for(i=0;i<6;i++) 
	{
		pthreadPT_index[i] = (pthread_t*) malloc(sizeof(pthread_t));
	}

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
	endCond = 0; 
	
	mySwitch.DIO[0] = 1;
	mySwitch.DIO[1] = 1;
	mySwitch.DIO[2] = 1;
	mySwitch.DIO[3] = 1;
	mySwitch.DIO[4] = 1;
	 
	myThumb.digit = thumb;
	myThumb.cmd.speed = 300;
	myThumb.cmd.mode = l_stop;
	
	myIndex.digit = index;
	myIndex.cmd.speed = 300;
	myIndex.cmd.mode = l_stop;
	
	myMiddle.digit = middle;
	myMiddle.cmd.speed = 300;
	myMiddle.cmd.mode = l_stop;
	
	myRing.digit = ring;
	myRing.cmd.speed = 300;
	myRing.cmd.mode = l_stop;
	
	myPinkie.digit = pinkie;
	myPinkie.cmd.speed = 300;
	myPinkie.cmd.mode = l_stop;

	printf(" About to start\n"); 	
/* intresting part */
   	ret[0] = pthread_create( &thumb_T, NULL, sendCmd, &myThumb);
	ret[1] = pthread_create( &index_T, NULL, sendCmd, &myIndex);
	ret[2] = pthread_create( &middle_T,NULL, sendCmd, &myMiddle);
	ret[3] = pthread_create( &ring_T,  NULL, sendCmd, &myRing);
	ret[4] = pthread_create( &pinkie_T,NULL, sendCmd, &myPinkie);
	ret[5] = pthread_create( &ni_T,NULL, niRead, NULL); 
	pthread_join(thumb_T, NULL);
	pthread_join(index_T, NULL);
	pthread_join(middle_T,NULL);
	pthread_join(ring_T,  NULL);
	pthread_join(pinkie_T,NULL);
	pthread_join(ni_T,NULL);

/* Stoping hand */
    usleep(40000); 
	myThumb.cmd.mode  = l_stop;	
	myIndex.cmd.mode  = l_stop;	
	myMiddle.cmd.mode = l_stop;	
	myRing.cmd.mode   = l_stop;	
	myPinkie.cmd.mode = l_stop;	

    writeLimb(myThumb.handle,&(myThumb.cmd));
    writeLimb(myIndex.handle,&(myIndex.cmd));
    writeLimb(myMiddle.handle,&(myMiddle.cmd));
    writeLimb(myRing.handle,&(myRing.cmd));
    writeLimb(myPinkie.handle,&(myPinkie.cmd));
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
	
	for(i=0;i<6;i++) 
	{
		free(pthreadPT_index[i]);
	}
	printf("Successful uses :]\n");
	pthread_exit(NULL);  
}


