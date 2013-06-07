#include "ntcan.h"
#include "myCan.h"

typedef enum {THUMB_OPEN,	THUMB_CLOSE,
              STANDARD_OPEN, 	STANDARD_CLOSE} modes; 

typedef enum {CONTINIOUS, QUICK} speed; 

int32_t grip(NTCAN_HANDLE handle, int32_t ID);

int32_t jawChuck(NTCAN_HANDLE handle, modes myMode, int32_t ID);

int32_t lockout(NTCAN_HANDLE handle, int32_t ID);

int32_t natural(NTCAN_HANDLE handle, int32_t ID);

int32_t normal(NTCAN_HANDLE handle, int32_t ID); 

int32_t park(NTCAN_HANDLE handle, speed mySpeed, int32_t ID); 

int32_t pinch(NTCAN_HANDLE handle, modes myMode, int32_t ID);

int32_t point(NTCAN_HANDLE handle, int32_t ID);





