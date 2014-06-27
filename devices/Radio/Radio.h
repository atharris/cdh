#include <iostream>
#include <fstream>
#include <cstring>

#include "packetgen.h"
#include "serial.h"
#include "Packet.h"
#include <queue>

using namespace std;
#ifndef RADIO_H_
#define RADIO_H_ 

/*enum message_type{

  NO_OP_COMMAND,
  GET_TRANCEIVER_CONFIG,
  TELEMETRY_QUERY,
  RESET_SYSTEM,
  TRANSMIT_DATA,
  SET_TRANSCEIVER_CONFIG,
  WRITE_FLASH
  
  }*/

class Radio{

 public:
  Radio(string path="/dev/ttyS1");
  //  int configureRadio(string configurationFilePath);
  //  int transmitPayload(string payloadFile);
  int configure_radio(string configuration_file_path);
  int listen();
  int transmit_payload(unsigned char payload[],unsigned char payload_size);
  int send_packet(Packet *packet);
  
  

 private:
  string device_path;
  int fd_target;
};


inline int parsemessagetype(string mtype){
  if(mtype.length()>1){
    cerr << "Invalid message type. Valid options are [ntcmRSF]\n";
    return(-1);
  } else if(mtype.length() == 0){
    cerr << "No message type on command line. Using interactive transmit mode\n";
    return(-2);
  }

  char mt = mtype.c_str()[0];
  switch(mt){
  case 'n': return(NO_OP_COMMAND);
    break; //heh, like it matters. Yay habits.
  case 'R': return(RESET_SYSTEM);
    break;
  case 't': return(TRANSMIT_DATA);
    break;
  case 'c': return(GET_TRANSCEIVER_CONFIG);
    break;
  case 'S': return(SET_TRANSCEIVER_CONFIG);
    break;
  case 'm': return(TELEMETRY_QUERY);
    break;
  case 'F': return(WRITE_FLASH);
    break;
  default:
    cerr << "Invalid message type. Valid options are [ntcmRSF]\n";
    return(-1);
  }
}

inline int preparesourcestream( string ifilename, ifstream& source){

  if( ifilename.compare("") == 0)
    {
      cerr << "No \"-i inputfilename\" given, using stdin\n";
      cerr << "Enter payload, ^D to end: ";
      ifilename = "/dev/stdin";
    }

  source.open(ifilename.c_str(), ios::in);

  if(source.fail())
    {
      cerr << "Failed to open source file\n";
      return(-1);
    }
  else return(0);
}


#endif
