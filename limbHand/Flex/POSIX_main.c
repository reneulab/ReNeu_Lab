/*************************************************/
#include "limb.h"
#include "myCan.h"
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


#define WAIT 	20000
#define SPEED 	690
int32_t stop_program;
NiFpga_Session session;

void catch_signal(int sig)
{
	printf("Quitting\n");
	stop_program = 1;
}

void* limb_move_Thread(void *arg1) 
{
	arg *myArg;
	myArg = ((arg *) arg1);
	moveLimb(session,myArg);	
	pthread_exit(NULL);  
}


int32_t main(void)
{
	pthread_t*  digit_T[5]; 
	arg 	  	myDigit[5]; 
	int32_t 	ret[5]; 
	int32_t		i=0;
	int32_t 	data=150;

	signal(SIGINT,catch_signal);
	signal(SIGTERM,catch_signal);
/* Init thread pointers*/
	for(i=0;i<5;i++)
		{ digit_T[i] = malloc(sizeof(pthread_t)); }
/* Init  hand */
	myDigit[0].handle = initLimb(thumb, &myDigit[0].cmd);
	if(myDigit[0].handle == 0xFFFF) 
		{ printf("Error in thumb init\n"); return 1; } 
	usleep(20000);

	myDigit[1].handle = initLimb(index, &myDigit[1].cmd);
	if(myDigit[1].handle == 0xFFFF) 
		{ printf("Error in index init\n"); return 1; } 
	usleep(20000);

	myDigit[2].handle = initLimb(middle, &myDigit[2].cmd);
	if(myDigit[2].handle == 0xFFFF) 
		{ printf("Error in middle init\n"); return 1; } 
	usleep(20000);

	myDigit[3].handle = initLimb(ring, &myDigit[3].cmd);
	if(myDigit[3].handle == 0xFFFF) 
		{ printf("Error in ring init\n"); return 1; } 
	usleep(20000);

	myDigit[4].handle = initLimb(pinkie, &myDigit[4].cmd);
	if(myDigit[4].handle == 0xFFFF) 
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
                                              NiFpga_Signature,
                                              "rio://146.6.84.41/RIO0",
                                              NiFpga_OpenAttribute_NoRun,
                                              &session));
		printf("Session Status: %i\n",status);
      	if (NiFpga_IsNotError(status))
      	{ /* run the FPGA application */
         	printf("Running the FPGA...\n");
         	NiFpga_MergeStatus(&status, NiFpga_Run(session,0));

/* Setting values */ 
	stop_program = 0; 
/* Init all Args to correct digit with speed and wait time */  
	for(i=0;i<5;i++)
	{
		myDigit[i].time = WAIT;
		myDigit[i].cmd.speed = SPEED;
		myDigit[i].cmd.mode = l_stop;
	}

	myDigit[0].digit = thumb;
	myDigit[0].flex  = NiFpga_index_flex;
	myDigit[1].digit = index;
	myDigit[1].flex  = NiFpga_index_flex;
	myDigit[2].digit = middle;
	myDigit[2].flex  = NiFpga_middle_flex;
	myDigit[3].digit = ring;
	myDigit[3].flex  = NiFpga_ring_flex;
	myDigit[4].digit = pinkie;
	myDigit[4].flex  = NiFpga_ring_flex;
	for(i=0;i<5;i++)
	{ NiFpga_ReadI32(session,myDigit[i].flex,&(myDigit[i].initAngle)); }
	printf("Starting... hit CTR-C to exit\n"); 	
/* intresting part */
	while(stop_program==0)
	{
/* runs finger if state of switch changed, uses up to 5 threads */ 	
		for(i=0;i<5;i++)
			{ myDigit[i].angle = 150; }	
//		ret[0] = pthread_create( &(digit_T[0]), NULL, limb_move_Thread, &(myDigit[0]));
		ret[1] = pthread_create( &(digit_T[1]), NULL, limb_move_Thread, &(myDigit[1]));
		ret[2] = pthread_create( &(digit_T[2]), NULL, limb_move_Thread, &(myDigit[2])); 
		ret[3] = pthread_create( &(digit_T[3]), NULL, limb_move_Thread, &(myDigit[3])); 
		ret[4] = pthread_create( &(digit_T[4]), NULL, limb_move_Thread, &(myDigit[4]));
		for(i=1;i<5;i++)
			{ pthread_join(digit_T[i],NULL); }
/* Loops back to intresting part until ctr-c is pressed */	
		for(i=0;i<5;i++)
			{ myDigit[i].angle = -150; }	
//		ret[0] = pthread_create( &(digit_T[0]), NULL, limb_move_Thread, &(myDigit[0]));
		ret[1] = pthread_create( &(digit_T[1]), NULL, limb_move_Thread, &(myDigit[1]));
		ret[2] = pthread_create( &(digit_T[2]), NULL, limb_move_Thread, &(myDigit[2])); 
		ret[3] = pthread_create( &(digit_T[3]), NULL, limb_move_Thread, &(myDigit[3])); 
		ret[4] = pthread_create( &(digit_T[4]), NULL, limb_move_Thread, &(myDigit[4]));
		for(i=1;i<5;i++)
			{ pthread_join(digit_T[i],NULL); }
	}

/* Stopping hand */
    usleep(40000);
	printf("Stopping Hand\n");
	for(i=0;i<5;i++)
	{
		myDigit[i].cmd.mode = l_stop; 	 
		writeLimb(myDigit[i].handle,&(myDigit[i].cmd));
		usleep(40000);
	}
	usleep(500000);
/* Closing NI FPGA */
		printf("Stopping the FPGA...\n");
	    NiFpga_MergeStatus(&status, NiFpga_WriteU32(session, 
						NiFpga_stop,1));
		/* close the session now that we're done */
	    printf("Closing the session...\n");
	    NiFpga_MergeStatus(&status, NiFpga_Close(session, 0));
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
	for(i=0;i<5;i++)
		{ closeLimb(myDigit[i].handle,myDigit[i].digit);}
	printf("Successful uses :]\n");
	pthread_exit(NULL);  
}


