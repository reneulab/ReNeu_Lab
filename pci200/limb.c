#include "limb.h" 

int32_t grip(CMSG *msg, int32_t ID) 
{
   int32_t result;

   msg->id = ID;
   msg->len = 2;
   msg->data[0] = 0x05;
   msg->data[1] = 0x00;
   result = writeNTCAN(handle, msg);
   if(result != 0) 
		{ return 1; }
	return 0;  
}

int32_t jawChuck(CMSG *msg, modes myMode, int32_t ID)
{
   int32_t result; 

	msg->id = ID; 
   msg->len = 2;
   msg->data[1] = 0x00; 
   switch(myMode) {
		case THUMB_OPEN:
			msg->data[0] = 0x0E;
         break;
      case THUMB_CLOSE:
	 		msg->data[0] = 0x0B;
    	 	break;
		case STANDARD_OPEN: 
			msg->data[0] =  0x0D;
			break;
		case STANDARD_CLOSE:
			msg->data[0] = 0x02;
			break;
		default:
			msg->data[0] = 0xFF;
			return 2;
	}
}  
   result = writeNTCAN(handle, msg);
   if(result != 0) 
		{ return 1; }
	return 0;  
	 
int32_t lockout(CMSG *msg, int32_t ID) 
{
	msg->id = ID;
	msg->len = 2;
	msg->data[0] = 0x0C;
	msg->data[1] = 0x00;   
   result = writeNTCAN(handle, msg);
   if(result != 0) 
		{ return 1; }
	return 0;  
}

int32_t natural(CMSG *msg, int32_t ID)
{
	msg->id = ID;
	msg->len = 2;
   msg->data[0] = 0x08;
	msg->data[1] = 0x00;   
   result = writeNTCAN(handle, msg);
   if(result != 0) 
		{ return 1; }
	return 0;  
}

int32_t normal(CMSG *msg, int32_t ID)
{
	msg-id = ID;
	msg->len = 2;
	msg->data[0] = 0x00;
	msg->data [1] = 0x00;   
   result = writeNTCAN(handle, msg);
   if(result != 0) 
		{ return 1; }
	return 0;  
}

int32_t park(CMSG *msg, speed mySpeed, int32_t ID)
{
	msg->id = ID;
	msg->len = 2;
	msg->data[1] = 0x00; 
	switch(mySpeed) {
		case CONTINIOUS:
			msg->data[0] = 0x03;
			break;
		case QUICK:
			msg->data[0] = 0x04;
			break;
		default:
			msg->data[0] = 0x0FF; 
			return 2;
	}
   result = writeNTCAN(handle, msg);
   if(result != 0) 
		{ return 1; }
	return 0;  
}

int32_t pinch(CMSG *msg, modes myMode, int32_t ID) 
{
	msg->id = ID;
	msg->len = 2;
	msg->data[1] = 0x00;
	switch(myMode) {
		case THUMB_OPEN:
			msg->data[0] = 0x0A;
         break;
      case THUMB_CLOSE:
	 		msg->data[0] = 0x09;
    	 	break;
		case STANDARD_OPEN: 
			msg->data[0] =  0x07;
			break;
		case STANDARD_CLOSE:
			msg->data[0] = 0x01;
			break;
		default:
			msg->data[0] = 0xFF;
			return 2;
	} 
   result = writeNTCAN(handle, msg);
   if(result != 0) 
		{ return 1; }
	return 0;  
}

int32_t point(CMSG *msg, int32_t ID) {
	msg->id = ID;
	msg->len = 2;
	msg->data[0] = 0x06;
	msg->data[1] = 0x00; 
   result = writeNTCAN(handle, msg);
   if(result != 0) 
		{ return 1; }
	return 0;  
}



