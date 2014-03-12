#ifndef PACKET_GEN_H
#define PACKET_GEN_H
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "heliumconfig.h"
#define FIRMWARE_VERSION_3
#define MAX_PAYLOAD_LENGTH 255

//Configuration Commands 
#define NO_OP_COMMAND           0x01 
#define RESET_SYSTEM            0x02 
#define TRANSMIT_DATA           0x03 
#define RECEIVE_DATA            0x04
#define GET_TRANSCEIVER_CONFIG  0x05
#define SET_TRANSCEIVER_CONFIG  0x06
#define TELEMETRY_QUERY         0x07
#define WRITE_FLASH             0x08 

//Sync bytes are 'H' and 'e'
#define SYNC_1  0x48
#define SYNC_2  0x65

//input and output message ID bytes
#define I_MESSAGE_TYPE 0x10
#define O_MESSAGE_TYPE 0x20

using namespace std;

inline void calculate_header_checksum(unsigned char* buffer)
{
	unsigned char ck_a=0, ck_b=0;
	
	//start from byte 2, skipping the 'H' 'e' sync bytes
	for(int i=2; i<6; i++)
	{
		ck_a = ck_a + buffer[i];
		ck_b = ck_b + ck_a;
	}

	buffer[6] = ck_a;
	buffer[7] = ck_b;
	return;
}


inline void calculate_payload_checksum( unsigned char *data, size_t data_size ) 
{ 
    unsigned char ck_a = 0; 
    unsigned char ck_b = 0; 
 
  for( size_t i = 0; i < data_size; i++ ) 
 	{ 
  		ck_a += data[i]; 
  		ck_b += ck_a; 
 	} 
 
  data[data_size] = ck_a; 
  data[data_size+1] = ck_b; 
 
  return; 
} 

//returns 0 for success, nonzero for failure
inline int send_data(ifstream& source, int fd_target)
{
	//temporary buffer for payload, and payload length counter
	unsigned char payload[MAX_PAYLOAD_LENGTH];
	int foo;
	int data_size=0;

	while(source.good() && data_size < MAX_PAYLOAD_LENGTH)
	{
		foo = source.get();
		if(foo > 0 && foo < 256)
		{
			payload[data_size]=(unsigned char)foo;
			data_size++;
		}
	}

	unsigned char size_upper=0, size_lower=0;

	//most and least significant bytes of data_size
	//MSByte must be zero, despite having two bytes of space for this.  WTF, AstroDev?
	size_upper = data_size & 0xff00u;
	size_lower = data_size & 0x00ffu;
	
	if(size_upper != 0x00)
	{
		cerr << "\n send_data: Got a payload of "<< data_size << " bytes.  Not sending to radio.\n";
		return(1);
	}

	//8 bytes of header, payload, 2 bytes of checksum
	size_t packet_size = data_size + 10;
	unsigned char buffer[packet_size];

	//assemble header
	buffer[0] = SYNC_1;
	buffer[1] = SYNC_2;
	buffer[2] = I_MESSAGE_TYPE;
	buffer[3] = TRANSMIT_DATA;

	buffer[4] = size_upper;
	buffer[5] = size_lower;

	//Checksum the 6-byte-long header; that goes into bytes 6 and 7
	calculate_header_checksum(buffer);

	//Payload field starts at byte 8
	for(int i=0; i<data_size; i++)
	{
		buffer[i + 8] = payload[i];
	}

	//Checksum the packet, starting from the beginning of the meaningful header?? Or payload?
	calculate_payload_checksum(buffer + 2, data_size+6); //stoopid AstroDev, their "payload" means "header+payload"
	
	int result;
	result=write(fd_target, buffer, packet_size);
	if(result == packet_size){
		//looks like the write worked
		return(0);
	} else {
		//Aw crap.
		cerr << "send_data: Failed to write packet to port.\n";
		return(1);
	}
}

inline int send_command(unsigned int msgtype, int fd_target){
	unsigned char buffer[8]; //data-less command packets are all 8 bytes long

	//assemble header
	buffer[0] = SYNC_1;
	buffer[1] = SYNC_2;
	buffer[2] = I_MESSAGE_TYPE;
	buffer[3] = msgtype;

	buffer[4] = 0;
	buffer[5] = 0;
	
	//Checksum the 6-byte-long header; that goes into bytes 6 and 7
	calculate_header_checksum(buffer);

	int result;
	result=write(fd_target, buffer, 8);
	if(result == 8){
		//Okay, cool.
		return(0);
	} else {
		//Dammit.
		cerr << "send_command: Failed to write packet to port.\n";
		return(1);
	}
}

//returns 0 for success, nonzero for failure
//mostly copypasta from send_data.  Maybe woulda been smarter to make that flexible.
//Firmware verison 3 added another two bytes.  The radio is still on version 2 until AstroDev wakes up.
#ifdef FIRMWARE_VERSION_3
#define PACKED_CONFIG_LENGTH 34
#else
#define PACKED_CONFIG_LENGTH 32
#endif
inline int send_config(ifstream& source, int fd_target)
{
	//temporary buffer for payload, and payload length counter
	unsigned char config_buf[PACKED_CONFIG_LENGTH];
	int foo;
	int data_size=0;

	while(source.good() && data_size < PACKED_CONFIG_LENGTH)
	{
		foo = source.get();
			config_buf[data_size]=(unsigned char)foo;
			data_size++;
	}

	cerr << "Read config struct with data_size of " << data_size << endl;
	unsigned char size_upper=0, size_lower=0;

	//most and least significant bytes of data_size
	//MSByte must be zero, despite having two bytes of space for this.  WTF, AstroDev?
	size_upper = data_size & 0xff00u;
	size_lower = data_size & 0x00ffu;
	
	if(size_upper != 0x00)
	{
		cerr << "\n send_config: Got a payload of "<< data_size << " bytes.  Not sending to radio.\n";
		return(1);
	}

	//8 bytes of header, payload, 2 bytes of checksum
	const size_t packet_size = PACKED_CONFIG_LENGTH + 10;
	unsigned char buffer[packet_size];

	//assemble header
	buffer[0] = SYNC_1;
	buffer[1] = SYNC_2;
	buffer[2] = I_MESSAGE_TYPE;
	buffer[3] = SET_TRANSCEIVER_CONFIG;

	buffer[4] = size_upper;
	buffer[5] = size_lower;

	//Checksum the 6-byte-long header; that goes into bytes 6 and 7
	calculate_header_checksum(buffer);

	//Payload field starts at byte 8
	for(int i=0; i<data_size; i++)
	{
		buffer[i + 8] = config_buf[i];
	}

	//Checksum the packet, starting from the beginning of the header data
//	calculate_payload_checksum(buffer + 2, data_size+6);
	calculate_payload_checksum(buffer + 2, PACKED_CONFIG_LENGTH+6);
	
#ifdef DEBUG
   fprintf(stderr,"dumping packet to stdout");
   write(1,buffer,packet_size);//write the raw packet to stdout also
#endif
	int result;
	result=write(fd_target, buffer, packet_size);
	if(result == packet_size){
		//looks like the write worked
		return(0);
	} else {
		//Aw crap.
		cerr << "send_data: Failed to write packet to port.\n";
		return(1);
	}
}

#endif
