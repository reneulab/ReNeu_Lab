#include "ntcan.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


/***************************************************************/
/*    Sets up a NTCAN Device and links ID array to that device */
/*		IN: Baud rate for device, flag for mode 						*/
/*        pointer to ID array for to be linked to NTCAN device	*/
/*        (1st element is the length of array)						*/
/*        number of physical nets										*/
/*        max size of rx buffer, timeout for rx						*/
/*        max size of tx buffer, timeout for tx						*/
/*    OUT: handle if success, 0xFFFF if error						*/
/***************************************************************/

int32_t initNTCAN(uint32_t baud, 	uint32_t flags,
						int32_t ID[], 		int32_t net,
						int32_t rxSize, 	int32_t rxTime,
						int32_t TxSize, 	int32_t txTime);



/***************************************************************/
/*       Closes a NTCAN Device											*/
/*			IN: Handle of device to close 								*/
/*				 ID array for IDs linked to handle						*/
/*				 (1st element is the length of array)					*/
/*       OUT: 0 if success, 1 if error									*/
/***************************************************************/

int32_t closeNTCAN(NTCAN_HANDLE handle, int32_t ID[]);



/***************************************************************/
/*     Reads data from NTCAN device and prints it to screen		*/
/*     IN: 	Handle of NTCAN device										*/
/*				buffer to store data read 									*/
/*				Length of data to read in bytes							*/
/*		 OUT: 0 if success, 1 if error, 2 if Timeout error			*/
/*******nt32_t initNTCAN(uint32_t baud,    uint32_t flags,
                  int32_t ID[],     int32_t net,
********************************************************/

int32_t readNTCAN(NTCAN_HANDLE handle, CMSG *msg, int32_t len);



/***************************************************************/
/*		      Writes data in msg to NTCAN device 						*/
/*    IN: Handle of NTCAN device											*/
/*			 length of message to write in bytes						*/
/*			 MSG struct with ID of device and length preset			*/
/*		OUT: 0 if success, 1 if error 									*/
/***************************************************************/
int32_t writeNTCAN(NTCAN_HANDLE handle,int32_t len, CMSG *msg);



