#include "SDO.h"
#include "socketcan/socketcan.h"
#include "socketcan/printd.h"


#define SDO_RESPONSE_ERROR     0x80
#define SDO_RESPONSE_WRITE_OK  0x60
#define SDO_RESPONSE_READ_OK   0x40


/** Automatic calculation of ccd-field for requests **/
static uint8_t SDO_calculate_ccd(char rw, int32_t size) {
	uint8_t base = 0x40;
	if(rw == 'w') {
		base = 0x20;
	}

	switch(size) {
		case 1: return base + 0x0F;
		case 2: return base + 0x0B;
		case 3: return base + 0x07;
		case 4: return base + 0x03;
	}
	return 0;
}


int32_t SDO_write(int32_t handle, const SDO_data* d) {
	int32_t err;
	int32_t fillerbytes;
	const int32_t timeout = 1000;
	const int32_t buffer = 5;
	uint16_t cob, cob_r;
	uint8_t ccd, msb, lsb;
	CMSG f;

	// Define data
	cob = SDO_RX + d->nodeid;
	ccd = SDO_calculate_ccd('w', d->data.size);
	fillerbytes = 8 - 4 - d->data.size;
	Socketcan_t data[5] = {
		{1, ccd},
		{2, d->index},
		{1, d->subindex},
		d->data,
		{fillerbytes, 0x00}
	};
	
	

	// Send write request
	err = socketcan_write(handle, cob, 5, data);
	if (err != 0) {
	   printd(LOG_ERROR, "socketcan SDO: Could not write to the CAN-bus handle=%d.\n", handle);
		return err;
	}

	cob_r = SDO_TX + d->nodeid;
	msb = (d->index>>8 & 0xff);
	lsb = (d->index & 0xff);

	// Wait for result
	for(int i=0; i<buffer; i++) {
		err = socketcan_read(handle, &f, timeout/buffer);
		//printf("err=%d node=0x%x index=0x%x sub=0x%x from=0x%x res=0x%x\n", err, d->nodeid, d->index, d->subindex, f.id, f.data[0]);
		
		
		if(err == 0 && f.len >= 4 && f.id == cob_r && f.data[1] == lsb && f.data[2] == msb && f.data[3] == d->subindex) {
			// Response recived
			if(f.data[0] == SDO_RESPONSE_WRITE_OK) {
				//printf("ok\n\n");
				return 0;
			} else {
				printd(LOG_ERROR, "socketcan SDO: response error node=%d index=0x%x\n", d->nodeid, d->index);
				return SOCKETCAN_ERROR;
			}
		}
	}

	printd(LOG_WARN, "socketcan SDO: timeout node=%d index=0x%x\n", d->nodeid, d->index);
	return SOCKETCAN_TIMEOUT;
}


int32_t SDO_acknowledge(int32_t handle, const CMSG* f) {
	Socketcan_t ack[4];
	ack[0].size = 1;
	ack[0].data = SDO_RESPONSE_WRITE_OK;

	ack[1].size = 1;
	ack[1].data = f->data[1]; // index lsb

	ack[2].size = 1;
	ack[2].data = f->data[2]; // index msb

	ack[3].size = 1;
	ack[3].data = f->data[3]; // subindex

	uint16_t nodeid = f->id-SDO_RX;
	return socketcan_write(handle, SDO_TX+nodeid, 4, ack);
}
