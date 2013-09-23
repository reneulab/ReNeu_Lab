/*************************************************/
#include "Leap.h"
#include "LeapMath.h"
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

extern "C" {
// C headers callled in c++
#include "limb.h"
#include "myCan.h"

}

using namespace Leap;

#define WAIT 	20000
#define SPEED 	690
int endProgram;




float palmpos = 0;
float angle[5] = {0,0,0,0,0};
int f_switch[5] = {0,0,0,0,0};
int counter = 0;
class SampleListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
};

void SampleListener::onInit(const Controller& controller) {
  std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
  std::cout << "Connected" << std::endl;
  
}

void SampleListener::onDisconnect(const Controller& controller) {
  //Note: not dispatched when running in a debugger.
  std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
  std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
  // Get the most recent frame and report some basic information
  const Frame frame = controller.frame();
  
  if (!frame.hands().empty()) {
    // Get the first hand
    const Hand hand = frame.hands()[0];

    // Check if the hand has any fingers
    const FingerList fingers = hand.fingers();
    if (!fingers.empty()) {
           if(counter==0){
      // Calculate the hand's average finger tip position
      
			   palmpos = hand.palmPosition()[1];
		       std::cout<<palmpos<<"\n";	   
		       for(int i =0;i<fingers.count();++i)
		       std::cout<<angle[i]<<"\t";
               std::cout<<"\n";
   counter++;
   }
   else
   {counter++;
    if(counter>50) counter=0;
   }
 
   for(int i =0;i<fingers.count();++i)
           {
			   angle[i] = fingers[i].stabilizedTipPosition()[1];
		        
		   }
     
   
    if((palmpos-angle[4]) >10.0) 
          f_switch[0] =0;  
	else
          f_switch[0] =1;
    
    if((palmpos-angle[1]) >10.0) 
          f_switch[1] =0;
	else
          f_switch[1] =1;
        
    
    if((palmpos-angle[0])>10.0) 
          f_switch[2] =0;
    else
          f_switch[2] =1;

	
	if((palmpos-angle[2])>10.0) 
          f_switch[3] =0;
    else
          f_switch[3] =1;
	
	if((palmpos-angle[3])>10.0) 
          f_switch[4] =0;
	else
          f_switch[4] =1;
	
	
   }
   else
   { 

   	for(int i =0;i<5;i++){
     	f_switch[i] =0;
     	}

   }

 }
 else{
	 for(int i =0;i<5;i++){
     	f_switch[i] =1;
 }

}
 //usleep(100000);     
}
void SampleListener::onFocusGained(const Controller& controller) {
  std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
  std::cout << "Focus Lost" << std::endl;
}


}



/* function to end program */

void catch_signal(int sig)
{
	// Remove the sample listener when done
  	
        printf("Quitting\n");
	endProgram = 1;
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



int main()
{
	pthread_t*  digit_T[5]; 
	pthread_t*  switch_T;	
	arg 	  	myDigit[5]; 
	int32_t 	ret[5]; 
	int32_t		ret0;
	int32_t		i=0;
	handMode 	lastCmd[5]; 

/*Leap motion classes init*/
SampleListener listener;
Controller controller;


 // Have the sample listener receive events from the controller
 	controller.addListener(listener);

/* Used for ended the program with ctr-c */ 
	signal(SIGINT,catch_signal);
	signal(SIGTERM,catch_signal);



/* Init thread pointers*/
	for(i=0;i<5;i++) {  digit_T[i] = (pthread_t*)malloc(sizeof(pthread_t)); }
	switch_T  = (pthread_t*)malloc(sizeof(pthread_t)); 
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
/* Setting values */ 
	endProgram = 0; 


/*Args to correct digit with speed and wait time */  
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

	printf("Starting... hit CTR-C to exit\n"); 	
/* intresting part */
	while(endProgram==0)
	{
/* Read all switch inputs in one thread */	
		
		
/* runs finger if state of switch changed, uses up to 5 threads */ 		
		for(i=0;i<5;i++)
		{
			if(f_switch[i] == 0)
				{ myDigit[i].cmd.mode = l_close; }
			else if(f_switch[i] == 1)
				{ myDigit[i].cmd.mode = l_open; }
		
		
			if(lastCmd[i] != myDigit[i].cmd.mode)
				{ ret[i] = pthread_create((digit_T[i]), NULL, &limbThread, &(myDigit[i]));}
		}
		for(i=0;i<5;i++)
		{
			if(lastCmd[i] != myDigit[i].cmd.mode)
				{ pthread_join(*digit_T[i],NULL); }
			lastCmd[i] = myDigit[i].cmd.mode; 
		}
/* Loops back to intresting part until ctr-c is pressed */	
	}

// Remove the sample listener when done
  controller.removeListener(listener);

/* Stopping hand */
    usleep(40000);
	printf("Stopping Hand\n");
	for(i=0;i<5;i++)
	{
		myDigit[i].cmd.mode = l_stop; 	 
	        ret[i] = pthread_create( (digit_T[i]),  NULL, &limbThread, &(myDigit[i]));
		pthread_join(*digit_T[i],NULL);
		usleep(40000);
	}
	usleep(500000);

/* Closing limb hand */
	for(i=0;i<5;i++)
		{ closeLimb(myDigit[i].handle,myDigit[i].digit);}
	
	printf("Successful uses :]\n");
	pthread_exit(NULL);  
return 0;       

}


