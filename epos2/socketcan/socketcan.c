#include "socketcan.h"
#include "printd.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>


/*
#ifndef AF_CAN
#define AF_CAN PF_CAN
#endif
*/


/*int32_t socketcan_open(uint16_t filter[], uint16_t filtermask[], uint16_t num_filters) {
	NTCAN_HANDLE handle =  -1;
	// Create the socket
	handle = socket( PF_CAN, SOCK_RAW, CAN_RAW );
	if(handle  == -1) {
		printd(LOG_ERROR, "socketcan: Error opening socket\n");
		return handle;
	}

	// Locate the interface you wish to use
	struct ifreq ifr;
	strcpy(ifr.ifr_name, "can0");
	ioctl(handle, SIOCGIFINDEX, &ifr); // ifr.ifr_ifindex gets filled with that device's index

	// Set Filter for this conection
	struct can_filter rfilter[num_filters];
	for(int i=0; i<num_filters; i++) {
		rfilter[i].can_id   = filter[i];
		rfilter[i].can_mask = filtermask[i];
	}
	setsockopt(handle, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
	
	// Select that CAN interface, and bind the socket to it.
	struct sockaddr_can addr;
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	bind(handle, (struct sockaddr*)&addr, sizeof(addr));

	// Set to non blocking
	fcntl(handle, F_SETFL, O_NONBLOCK);

	return handle;
} */


void socketcan_close(NTCAN_HANDLE handle) {
	canClose(handle);
}


int32_t socketcan_read(NTCAN_HANDLE handle, CMSG* frame,
                   int32_t timeout) {
	// Wait for data or timeout
	int32_t t;
        int32_t len; 
	struct pollfd p[1];
        NTCAN_RESULT result;  

	p[0].fd = handle;
	p[0].events = POLLIN;

	t = poll(p, 1, timeout);
        len = frame->len;
	// Try to read available data
	result = canRead(handle, frame, &len, NULL);
        if(result != 0) {
		// Error, no bytes read
		frame->id = 0;
		frame->len = 0;
		if (result == NTCAN_RX_TIMEOUT) {
			return SOCKETCAN_TIMEOUT;
		}
		//printd(LOG_ERROR, "socketcan: Could not read data from CAN-bus\n");
		return SOCKETCAN_ERROR;
	}
	return 0;
}


int32_t socketcan_write(NTCAN_HANDLE handle, uint16_t id,
                         uint8_t length, Socketcan_t data[])
 {
	NTCAN_RESULT result;
	uint8_t byte, n;
        CMSG frame;
        int32_t temp=0; 
	frame.id = id;
        
	byte = 0;
	for(int i=0; i<length; i++) {
		n = 0;
		while(n < data[i].size) {
			frame.data[byte] = (data[i].data >> 8*n);
			n++;
			byte++;
		}
	}
        temp = (int32_t) frame.len;
	frame.len = byte;
	result = canWrite(handle, &frame, &temp,  NULL);
	if(result != 0) {
		// Error, no data written
		printd(LOG_ERROR, "socketcan: Could not write data to CAN-bus\n");
		return SOCKETCAN_ERROR;
	}
	return 0;
}
