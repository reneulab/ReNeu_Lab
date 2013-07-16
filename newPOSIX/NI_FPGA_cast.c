/*
 * FPGA Interface C API example for GCC for computers running Linux.
 *
 * NOTE: In order to run this example, you must compile a LabVIEW FPGA bitfile
 *       and generate a C API for it. For more information about using this
 *       example, refer to the Examples topic of the FPGA Interface C API Help,
 *       located under
 *       Start>>All Programs>>National Instruments>>FPGA Interface C API.
 */

#include "NiFpga_FPGA.h"
#include "NiFpga.h"
#include <stdio.h>
#include <stdlib.h>
int main()
{
   /* must be called before any other calls */
   printf("Initializing...\n");
   NiFpga_Status status = NiFpga_Initialize();
   if (NiFpga_IsNotError(status))
   {
      NiFpga_Session session;
      /* opens a session, downloads the bitstream, and runs the FPGA */
      printf("Opening a session...\n");
      NiFpga_MergeStatus(&status, NiFpga_Open(Bitfile,
                                              NiFpga_FPGA_Signature,
                                              "rio://146.6.84.251/RIO0",
                                              NiFpga_OpenAttribute_NoRun,
                                              &session));
	  printf("Session Status: %i\n",status);
	  NiFpga_Bool output0 = 0;
	  NiFpga_Bool output1 = 0;
	  NiFpga_Bool output2 = 0;
	  NiFpga_Bool output3 = 0;
      NiFpga_Bool output4 = 0;
	  char c = 0;
      if (NiFpga_IsNotError(status))
      {
         /* run the FPGA application */
         printf("Running the FPGA...\n");
         NiFpga_MergeStatus(&status, NiFpga_Run(session, 0));
		 while(c != ' ') 
		 {
			status = NiFpga_WriteBool(session,NiFpga_FPGA_ControlBool_DIO0,1);
			printf("write status: %d\n", status);
			NiFpga_ReadBool(session,NiFpga_FPGA_IndicatorBool_DIO3,&output3);
			NiFpga_WriteBool(session,NiFpga_FPGA_ControlBool_DIO1,output3);  
            NiFpga_WriteBool(session,NiFpga_FPGA_ControlBool_LED4,output3);
			status = NiFpga_WriteBool(session,NiFpga_FPGA_ControlBool_DIO2,output3);

	        c = getchar(); 
		 }

	    /* stop the FPGA loops */
	    printf("Stopping the FPGA...\n");
	    NiFpga_MergeStatus(&status, NiFpga_WriteU32(session,
	                                                     NiFpga_FPGA_ControlBool_stop,
	                                                     1));
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
}
