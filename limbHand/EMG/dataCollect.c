/*************************************************/
#include "NiFpga.h"
#include "NiFpga_limb.h"
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


int32_t main(int argc, char* argv[])
{ 
	pthread_t*  emg_T;	
	int32_t 	ret[5]; 
	int32_t		ret0;
	niData 		myNI; 
	int32_t		i=0,j=0,n=0; 
	int32_t     restSignal[5]={0,0,0,0,0};
	int32_t		closeSignal[5]={0,0,0,0,0};
	int32_t 	temp[TRAINING_VALUE]; 
	NiFpga_Status status; 

/* Used for ended the program with ctr-c */ 
	signal(SIGINT,catch_signal);
	signal(SIGTERM,catch_signal);
/* Init thread pointers*/
	emg_T  = malloc(sizeof(pthread_t)); 

/* Init FPGA */
   	printf("Initializing...\n");			fputc(myNI.Analog[0],pThumbOut);
			fclose(pThumbOut);
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
 
/* Init all Args to correct digit with speed and wait time */  

	printf("relax\n");
	sleep(1);
	//NiFpga_ReadI32(myNI.session,NiFpga_thumb_flex, &restSignal[n]);
	pthread_create(&emg_T, NULL, niThread, &myNI);
	pthread_join(emg_T, NULL);
	for(n=0; n<5; n++) 
	{
	  restSignal[n] = myNI.Analog[n];
	}

// File Pointer
	FILE* pThumbOut,pIndexOut,pMidOut;
/* intresting part */
	while(endProgram==0)
	{
		ret0 = pthread_create(&emg_T,  NULL, niThread, &myNI);
		pthread_join(emg_T,NULL);

/* Read ADC values and input into text files */
		pThumbOut = fopen("thumbADCOut.txt","w");
		if(pThumbOut!=NULL)
		{
			fputc(myNI.Analog[0],pThumbOut);
			//fputs("\n",pThumbOut);
			fclose(pThumbOut);			
		}			
	}

/* Stopping hand */
    usleep(40000);
	printf("Stopping Hand\n");
	usleep(500000);
/* Closing NI FPGA */
		printf("Stopping the FPGA...\n");
	    NiFpga_MergeStatus(&status, NiFpga_WriteU32(myNI.session, 
						NiFpga_stop,1));
		/* close the session now that we're done */
	    printf("Closing th:e session...\n");
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
	
	printf("Successful uses :]\n");
	pthread_exit(NULL);  
}


