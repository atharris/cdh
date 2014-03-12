#include <iostream>
#include <iomanip>

using namespace std;

//argument is the root address of the packet
bool test_header_checksum(unsigned char* packet)
{
	unsigned char ck_a=0, ck_b=0;
	
	for(int i=2; i<6; i++)
	{
		ck_a += packet[i];
		ck_b += ck_a;
	}

	if( (packet[6] == ck_a) && (packet[7] == ck_b) ){
		return(true);
	} else {
		return(false);
	}
}

//arguments are address to beginning of payload field and payload size
bool test_payload_checksum(unsigned char* payload, size_t size)
{
	unsigned char ck_a=0, ck_b=0;

	for(int i=0; i<size; i++)
	{
		ck_a += payload[i];
		ck_b += ck_a;
	}

	if( (payload[size] == ck_a) && (payload[size+1] == ck_b) )
		return(true);
	else{
		cerr << "test_payload_checksum: I calculated " << int(ck_a) << ' ' << int(ck_b);
		cerr << " but I read " << int(payload[size]) << ' ' << int(payload[size+1]) << endl;
		return(false);
	}
}
