#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <time.h>

#include "checkpacket.h"
#include "serial.h"

#include "memmem.c"

//input and output message ID bytes
#define I_MESSAGE_TYPE 0x10
#define O_MESSAGE_TYPE 0x20

//Configuration Commands
#define NO_OP_COMMAND 0x01
#define RESET_SYSTEM 0x02
#define TRANSMIT_DATA 0x03
#define RECEIVE_DATA 0x04
#define GET_TRANSCEIVER_CONFIG 0x05
#define SET_TRANSCEIVER_CONFIG 0x06
#define TELEMETRY_QUERY 0x07
#define WRITE_FLASH 0x08

//Sync bytes are 'H' and 'e'
#define SYNC_1 0x48
#define SYNC_2 0x65

#define MAX_PACKET_LENGTH 265

#define CONFIG_TARGET_FILENAME "configfile"

using namespace std;

void write_command_type(unsigned char cmdbyte){

  switch(cmdbyte){
  case NO_OP_COMMAND:
    cerr << "NOP";
    break;
  case RESET_SYSTEM:
    cerr << "RESET";
    break;
  case TRANSMIT_DATA:
    cerr << "TRANSMIT";
    break;
  case RECEIVE_DATA:
    cerr << "RECEIVE";
    break;
  case GET_TRANSCEIVER_CONFIG:
    cerr << "GETCONF";
    break;
  case SET_TRANSCEIVER_CONFIG:
    cerr << "SETCONF";
    break;
  case TELEMETRY_QUERY:
    cerr << "TELEM";
    break;
  case WRITE_FLASH:
    cerr << "WRFLASH";
    break;
  default:
    cerr << "Invalid command type" << int(cmdbyte);
  }
  cerr << endl;
}

int run_command_in_payload(unsigned char* packet, size_t payload_size){

  unsigned char commandflag[] = "COMMAND";
  unsigned char *commandloc;
  unsigned char cmd[] = "/bin/sh ";
  const size_t commandlength = 4;

  //search for a command
  //searches for characters of commandflag within payload region of packet
  commandloc = memmem(packet+8,payload_size, commandflag, sizeof(commandflag)-1 );
  if(commandloc == NULL){
    return(-1); //no commandflag here
  }
  commandloc += sizeof(commandflag)-1; //move this pointer past the flag to the command text
  if(commandloc + commandlength > packet + payload_size + 8){
    //command flag is too close to the end of the packet, reading command flag would overrun
    //the payload field
    return(-1);
  }
  memcpy(cmd+8,commandloc,commandlength); //copy command text into last 4 chars of cmd
  char* friendly_cmd = reinterpret_cast<char*>(cmd);
  /* Splitting the command functionality -- system(script) will only handle local stuff like LED operations
Replies over the radio will be handled by passing cmdhe the command name through stdout
and it will look up the proper reply. */
  system(friendly_cmd);
  unsigned char shortcmd[] = " ";
  memcpy(shortcmd,commandloc,commandlength);
  char* friendly_shortcmd = reinterpret_cast<char*>(shortcmd);
  cout << friendly_shortcmd << endl;
  return(0);
}

void handle_payload(unsigned char* packet, size_t payload_size){
  //Placeholder function to do stuff with payloads. Will probably also care about the message type
  //cerr << "Someday I'll do something with packet payloads. Not today.\n";
  FILE* config_targetF;
  const char targetFmode[]= "wb+";
  int runresult;

  if(packet[3] == GET_TRANSCEIVER_CONFIG){ //This must be a reply to a config request
    if(packet[4]!= 255){ //255 would indicate a NACK, which doesn't come with a config struct
      config_targetF = fopen(CONFIG_TARGET_FILENAME,targetFmode);
      if(config_targetF == NULL) {
	cerr << "handle_payload: Failed to open " << CONFIG_TARGET_FILENAME << endl;
      }
      if(fwrite(packet+8,1,payload_size, config_targetF)>0){
	cerr << "Wrote config struct to " << CONFIG_TARGET_FILENAME << endl;
      }
      else{
	cerr << "Failed to write struct to " << CONFIG_TARGET_FILENAME << endl;
	if( feof(config_targetF) ) cerr << " with an EOF " << endl;
	cerr << "with ferror = " << ferror(config_targetF) << endl;
      }
      fclose(config_targetF);
    }
  }

  if( packet[3] == RECEIVE_DATA ){
    runresult = run_command_in_payload(packet,payload_size);
    if(runresult == -1){
      cerr << "No command found in this packet.\n";
    }
  }

  return;
}

void handle_command(unsigned char* packet){
  //Placeholder function to do stuff with payloadless replies. Distinguish between ack/nack, etc.
  if(packet[2] == I_MESSAGE_TYPE){
    cerr << "handle_command: Why am I getting an I_MESSAGE_TYPE? Skipping it.\n";
    return;
  }
  else if (packet[2] == O_MESSAGE_TYPE) {
    if(packet[4] == 0x0a && packet[5] == 0x0a){
      cerr << "ACK ";
    }
    else if(packet[4] == 0xff && packet[5] == 0xff){
      cerr << "NAK ";
    }
    write_command_type(packet[3]);
  }
  return;
}

int has_payload( unsigned char *buffer);

int has_payload( unsigned char *buffer){

  if(buffer[2] == I_MESSAGE_TYPE){ // message is a command to the radio
    switch(buffer[3]){
    case NO_OP_COMMAND:
    case RESET_SYSTEM:
    case GET_TRANSCEIVER_CONFIG:
    case TELEMETRY_QUERY:
      return(0);
      break;
    case TRANSMIT_DATA:
    case SET_TRANSCEIVER_CONFIG:
    case WRITE_FLASH:
      return(1);
      break;
    default:
      cerr << "has_payload: Invalid command type\n";
      return(0);
    }
  }
  else if(buffer[2] == O_MESSAGE_TYPE){ // message is a report from the radio
    switch(buffer[3]){
    case NO_OP_COMMAND:
    case RESET_SYSTEM:
    case SET_TRANSCEIVER_CONFIG:
    case TRANSMIT_DATA:
    case WRITE_FLASH:
      return(0); // These reports never have payload
      break;
    case TELEMETRY_QUERY:
    case GET_TRANSCEIVER_CONFIG:
    case RECEIVE_DATA:
      if(buffer[4] == 255 && buffer[5] == 255){
	return(0); // That's a NACK packet, no payload.
      } else if (buffer[4] == 0){
	return(1); // That's a valid-looking size
      } else {
	cerr << "has_payload: invalid size bytes: " << int(buffer[4]);
	cerr << " " << int(buffer[5]) << endl;
      }
      break;
    default:
      cerr << "has_payload: Invalid message type\n";
      return(0);
    }
  }
}

void waitfor(int fd_port, const unsigned char* text, size_t text_size){
  int i;
  char foo;
  for(i=0; i<text_size; i++){
    while(1) {
      read(fd_port, &foo, 1); //grab a character
      if(foo == text[i]) //if it matches
	break; //break out of the wait loop and move to the next char
      else
	i=0; //if it doesn't, go back to watching for the first char
    }
  }
}
