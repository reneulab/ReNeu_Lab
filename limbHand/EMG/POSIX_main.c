/*************************************************/
#include "limb.h"
#include "myCan.h"
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*************************************************/
// Comment out define below to allow multiple finger
// control

//#define SINGLECTRL

/*************************************************/



#define WAIT 	20000
#define SPEED 	690
#define TRAINING_VALUE	100
#define NUM_FINGERS		3
int32_t endProgram;

typedef struct {
	NiFpga_Session session;
	int32_t	Analog[5]; 
} niData; 
/* function to end program */
void catch_signal(int sig)
{
	printf("Quitting\n");
	endProgram = 1;
}

/* Thread to read input */
void* niThread(void *myNI)
{
   	NiFpga_ReadI32((*(niData*)myNI).session,NiFpga_thumb_flex, &((*(niData*)myNI).Analog[0]));
	NiFpga_ReadI32((*(niData*)myNI).session,NiFpga_index_flex, &((*(niData*)myNI).Analog[1]));
	NiFpga_ReadI32((*(niData*)myNI).session,NiFpga_middle_flex,&((*(niData*)myNI).Analog[2]));
	NiFpga_ReadI32((*(niData*)myNI).session,NiFpga_middle_flex,  &((*(niData*)myNI).Analog[3]));
	NiFpga_ReadI32((*(niData*)myNI).session,NiFpga_middle_flex,  &((*(niData*)myNI).Analog[4]));
	
	pthread_exit(NULL);
}
/* Thread for each finger that sends command then sends stop command */
void* limbThread(void *arg1) 
{
  	int32_t		result;
	arg 		myArg;
	handMode	myCmd;

	myArg = (*((arg *) arg1));
	myCmd = myArg.cmd.mode; 

	myArg.cmd.mode = l_stop;
	result = writeLimb(myArg.handle,&(myArg.cmd));
	usleep(20000);	

	myArg.cmd.mode = myCmd;
	result = writeLimb(myArg.handle,&(myArg.cmd));
	usleep(myArg.time);

	pthread_exit(NULL);  
}


int32_t main(void)
{
	pthread_t*  digit_T[5]; 
	pthread_t*  emg_T;	
	arg 	  	myDigit[5]; 
	int32_t 	ret[5]; 
	int32_t		ret0;
	niData 		myNI; 
	int32_t		i=0,j=0,n=0; 
	handMode 	lastCmd[5]; 
	int32_t     restSignal[5]={0,0,0,0,0};
	int32_t		closeSignal[5]={0,0,0,0,0};
	int32_t 	temp[TRAINING_VALUE]; 
	NiFpga_Status status; 

/* Used for ended the program with ctr-c */ 
	signal(SIGINT,catch_signal);
	signal(SIGTERM,catch_signal);
/* Init thread pointers*/
	for(i=0;i<5;i++)
		{ digit_T[i] = malloc(sizeof(pthread_t)); }
	emg_T  = malloc(sizeof(pthread_t)); 
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
  	status = NiFpga_Initialize();
   	if (NiFpga_IsNotError(status))
   	{
/* opens a session, downloads the bitstream, and runs the FPGA */
      	printf("Opening a session...\n");
      	NiFpga_MergeStatus(&status, NiFpga_Open(Bitfile,
                                              NiFpga_Signature,
                                              "rio://146.6.84.41/RIO0",
                                              NiFpga_OpenAttribute_NoRun,
                                              &(myNI.session)));
		printf("Session Status: %i\n",status);
      	if (NiFpga_IsNotError(status))
      	{ /* run the FPGA application */
         	printf("Running the FPGA...\n");
         	NiFpga_MergeStatus(&status, NiFpga_Run(myNI.session, 0));

/* Setting values */ 
	endProgram = 0; 
/* Init all DIO pins to 1 (open switch) */	
	for(i=0;i<5;i++)
		{ myNI.Analog[i] = 0; }  
/* Init all Args to correct digit with speed and wait time */  
	for(i=0;i<5;i++)
	{
		myDigit[i].time = WAIT;
		myDigit[i].cmd.speed = SPEED;
		myDigit[i].cmd.mode = l_stop;
		lastCmd[i] = l_stop; 
	}

	myDigit[0].digit = thumb;
	myDigit[1].digit = index;
	myDigit[2].digit = middle;
	myDigit[3].digit = ring;
	myDigit[4].digit = pinkie;

	printf("relax\n");
	sleep(1);
	//NiFpga_ReadI32(myNI.session,NiFpga_thumb_flex, &restSignal[n]);
	pthread_create(&emg_T, NULL, niThread, &myNI);
	pthread_join(emg_T, NULL);
	for(n=0; n<5; n++) 
	{
	  restSignal[n] = myNI.Analog[n];
	}
/* Training */
	for(j=0;j<NUM_FINGERS;j++)
	{
		printf("Move next finger, starting with thumb\n"); 
		getchar(); 
		for(i=0;i<TRAINING_VALUE;i++)
		{
			NiFpga_ReadI32(myNI.session,NiFpga_thumb_flex, &temp[i]);
			closeSignal[j] += temp[i];
		}
		closeSignal[j] = closeSignal[j]/TRAINING_VALUE;
		printf("Relax ^.^ closeSignal: %d for Digit: %d\n", closeSignal[j], j); 
		getchar();
	}
	printf("Starting... hit CTR-C to exit\n"); 	
	sleep(1);

/* intresting part */
	while(endProgram==0)
	{
		ret0 = pthread_create(&emg_T,  NULL, niThread, &myNI);
		pthread_join(emg_T,NULL);
/* runs finger if state of switch changed, uses up to 5 threads */ 		
		for(i=0;i<5;i++)
		{
		/* for all open or all close */
		#ifdef SINGLECTRL
			for(j=1;j<5;j++)
				{ myNI.Analog[j] = myNI.Analog[0]; }
		/*****************************/		
		#endif
		//	printf("Signal: %d\n", myNI.Analog[0]); 
			if( (myNI.Analog[i] >= closeSignal[i]-50) )
				{ myDigit[i].cmd.mode = l_close; }
			else { myDigit[i].cmd.mode = l_open; }
/* checking for last command */
			if(lastCmd[i] != myDigit[i].cmd.mode)
				{ ret[i] = pthread_create( &(digit_T[i]), NULL, limbThread, &(myDigit[i]));}
		}
		for(i=0;i<5;i++)
		{
			if(lastCmd[i] != myDigit[i].cmd.mode)
				{ pthread_join(digit_T[i],NULL); }
			lastCmd[i] = myDigit[i].cmd.mode; 
		}
/* Loops back to intresting part until ctr-c is pressed */	
	}

/* Stopping hand */
    usleep(40000);
	printf("Stopping Hand\n");
	for(i=0;i<5;i++)
	{
		myDigit[i].cmd.mode = l_stop; 	 
	    ret[i] = pthread_create( &(digit_T[i]),  NULL, limbThread, &(myDigit[i]));
		pthread_join(digit_T[i],NULL);
		usleep(40000);
	}
	usleep(500000);
/* Closing NI FPGA */
		printf("Stopping the FPGA...\n");
	    NiFpga_MergeStatus(&status, NiFpga_WriteU32(myNI.session, 
						NiFpga_stop,1));
		/* close the session now that we're done */
	    printf("Closing the session...\n");
	    NiFpga_MergeStatus(&status, NiFpga_Close(myNI.session, 0));
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
/* Closing limb hand */
	for(i=0;i<5;i++)
		{ closeLimb(myDigit[i].handle,myDigit[i].digit);}
	printf("Successful uses :]\n");
	pthread_exit(NULL);  
}


