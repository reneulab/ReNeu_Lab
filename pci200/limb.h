#include "myCan.h"

#define THUMB_OPEN 		1
#define THUMB_CLOSE		2
#define STANDARD_OPEN	3 
#define STANDARD_CLOSE 	4
#define CONTINUOUS		5
#define QUICK				6
 
int32_t grip(NTCAN_HANDLE handle, int32_t ID);

int32_t jawChuck(NTCAN_HANDLE handle, int32_t mode, int32_t ID);

int32_t lockout(NTCAN_HANDLE handle, int32_t ID);

int32_t natural(NTCAN_HANDLE handle, int32_t ID);

int32_t normal(NTCAN_HANDLE handle, int32_t ID); 

int32_t park(NTCAN_HANDLE handle, int32_t speed, int32_t ID); 

int32_t pinch(NTCAN_HANDLE handle, int32_t mode, int32_t ID);

int32_t point(NTCAN_HANDLE handle, int32_t ID);





