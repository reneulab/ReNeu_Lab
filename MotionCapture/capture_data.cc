/* fifo_example.c */
#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_fifos.h>
#include <stdio.h>
#include <fstream>
#include <iostream>

// Motion capture library
#include "owl.h"

#include "NiFpga.h"
#include "NiFpga_limb_EMGSample.h"
#define TICK_PERIOD 1000000000
#define TASK_PRIORITY 1
#define STACK_SIZE 10000000
#define FIFO 0


#define CPU_MAP 0xF

// Motion capture constants (change these to match your configuration)
//#define MARKER_COUNT 8
//#define SERVER_NAME "146.6.84.115"
#define FILE_NAME_MOTION_CAPTURE_CONFIG "motion_capture_config.txt"
#define INIT_FLAGS 0
#define CAPTURE_FREQ 480
/*******************************************/
/*    Uncomment define below to force      */
/*    FPGA scans to occur after LEDs       */
/*    have been tracked - EXPERIMENTAL     */                   
//#define SCAN_SYNC                            
/*******************************************/

//MODULE_LICENSE(“GPL”);
static RT_TASK *main_task;
int running_flag = 1;


using namespace std;

void owl_print_error(const char *s, int n)
{
    if(n < 0) printf("%s: %d\n", s, n);
    else if(n == OWL_NO_ERROR) printf("%s: No Error\n", s);
    else if(n == OWL_INVALID_VALUE) printf("%s: Invalid Value\n", s);
    else if(n == OWL_INVALID_ENUM) printf("%s: Invalid Enum\n", s);
    else if(n == OWL_INVALID_OPERATION) printf("%s: Invalid Operation\n", s);
    else printf("%s: 0x%x\n", s, n);
}

void* capture_motion(void *args)
{

    RT_TASK *task;
    unsigned long task_name =nam2num("MOTION");
    NiFpga_Session session;
    uint32_t ADCdata[2]; 
    OWLMarker markers[32];
    int tracker;
    int err, n, MARKER_COUNT;
    char SERVER_NAME[15];

    ofstream Motion_Capture_data;   // to save file
    ofstream EMG_Output;
    ifstream file_motion_capture_config;

    file_motion_capture_config.open(FILE_NAME_MOTION_CAPTURE_CONFIG);
    printf("Initializing FPGA...\n");
    NiFpga_Status status = NiFpga_Initialize();
    if (NiFpga_IsNotError(status))
    {
/* opens a session, downloads the bitstream, and runs the FPGA */
      	printf("Opening a session...\n");
      	NiFpga_MergeStatus(&status, NiFpga_Open(Bitfile,
                                              NiFpga_Signature,
                                              "rio://146.6.84.143/RIO0",
                                              NiFpga_OpenAttribute_NoRun,
                                              &(session)));
	printf("Session Status: %i\n",status);
      	if (NiFpga_IsNotError(status))
      	{ /* run the FPGA application */
            printf("Running the FPGA...\n");
            NiFpga_MergeStatus(&status, NiFpga_Run(session, 0));

	    if(file_motion_capture_config.fail())
	    {
		printf("File not found!");
		return 0;
	    }

	    file_motion_capture_config>>MARKER_COUNT; // read IP address and marker numbers from text
	    file_motion_capture_config>>SERVER_NAME;  // read IP address and marker numbers from text
/* Opening File */
	    EMG_Output.open("EMG_Output.txt");
	    Motion_Capture_data.open("Marker_Data.txt"); // open file to write

	    RTIME old_time, current_time, counter_time;

	    int flag, i;
	    //float marker_data[24];

	    if(owlInit(SERVER_NAME, INIT_FLAGS) < 0) return 0;

	    // create tracker 0
	    tracker = 0;
	    owlTrackeri(tracker, OWL_CREATE, OWL_POINT_TRACKER);

	    // set markers
	    for(int i = 0; i < MARKER_COUNT; i++)
		owlMarkeri(MARKER(tracker, i), OWL_SET_LED, i);

	    // activate tracker
	    owlTracker(tracker, OWL_ENABLE);

	    // flush requests and check for errors
	    if(!owlGetStatus())
	    {
		owl_print_error("error in point tracker setup", owlGetError());
		return 0;
	    }

	    if (!(task = rt_task_init_schmod(task_name, 0, 0, 0, SCHED_FIFO, CPU_MAP-1)))
	    {
		printf("CANNOT INIT TASK %lu\n", task_name);
		return 0;
	    }

	    printf("THREAD INIT MOTION CAPTURE: name = %lu, address = %p.\n", task_name, task);
	    mlockall(MCL_CURRENT | MCL_FUTURE);

	    rt_make_hard_real_time();

	    // set default frequency
	    owlSetFloat(OWL_FREQUENCY, OWL_MAX_FREQUENCY);

	    // start streaming
	    owlSetInteger(OWL_STREAMING, OWL_ENABLE);
	    counter_time = rt_get_time_ns()/1000;
	    // main loop
	    while(running_flag)
	    {	
		old_time = rt_get_time_ns();
		// get some markers
		n = owlGetMarkers(markers, 32);

		// check for error
		if((err = owlGetError()) != OWL_NO_ERROR)
		{
		    owl_print_error("error", err);
		    break;
		}

		// no data yet
		if(n == 0)
		{
		    continue;
		}


		if(n > 0)
		{
		    flag = 1;
		    for(i = 0; i < n; i++)
		    {
		        if(markers[i].cond < 0)
		            flag = 0;
		            printf("%d) %f %f %f %f\n", i, markers[i].x, markers[i].y, markers[i].z, markers[i].cond);
			/* Read LED data from Motion Capture to text file */
		            Motion_Capture_data << i << "\t" << markers[i].x << "\t" << markers[i].y << "\t" << markers[i].z << "\t" << markers[i].cond << "\t" << rt_get_time_ns() << endl;
			
			/* Scan ADC ports on FPGA, write to text file */
		       	    NiFpga_ReadArrayU32(session,NiFpga_EMGSample_Array,&ADCdata[0],NiFpga_Size_AnalogIn); 
			    EMG_Output << ADCdata[0] << "\t" << ADCdata[1] << "\t" << rt_get_time_ns() << endl;
		    }

		}

		rt_sleep_until(nano2count(old_time+1000000000/CAPTURE_FREQ));
		current_time = rt_get_time_ns();//clock();
		printf("(%lld)\n", (current_time-old_time));
	    }

	    // cleanup
	    file_motion_capture_config.close();
	    EMG_Output.close();
	    Motion_Capture_data.close();
	   /* closing NiFpga */
	    printf("Stopping the FPGA...\n");
	    NiFpga_MergeStatus(&status, NiFpga_WriteU32(session, 
						NiFpga_stop,1));
	   /* close the session now that we're done */
	    printf("Closing the session...\n");
	    NiFpga_MergeStatus(&status, NiFpga_Close(session, 0));

	    owlDone();
	    rt_make_soft_real_time();
	    /* must be called after all other calls */
            printf("Finalizing...\n");
            NiFpga_MergeStatus(&status, NiFpga_Finalize());
	    rt_task_delete(task);
        }
    }  
    return 0;
}

int main(int argc, char *argv[])
{
    int result = 0;
    RTIME tick_period;
    pthread_t capture_motion_thread;
    rt_allow_nonroot_hrt();
    if (!(main_task = rt_task_init_schmod(nam2num("MNTSK"), 0, 0, 0,SCHED_FIFO, CPU_MAP)))
    {
        printf("CANNOT INIT MNTSK TASK \n");
        exit(1);
    }
    rt_set_oneshot_mode();
    tick_period = start_rt_timer(0);

    pthread_create(&capture_motion_thread, 0, capture_motion, (void *)(result));

    getchar();
    running_flag = 0;
    stop_rt_timer();
    //    rtf_destroy(FIFO);
    rt_task_delete(main_task);

    return 0;
}
