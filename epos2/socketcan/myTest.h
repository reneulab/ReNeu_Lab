#include "ntcan.h"
 int32_t errorCheck(int32_t command, NTCAN_RESULT result);

 int32_t initNTCAN(uint32_t baud,	uint32_t flags,
                  int32_t ID[],		int32_t idLen, 
		  int32_t net,   	int32_t rxSize,  	
		  int32_t rxTime,	int32_t txSize,	
		  int32_t txTime); 

