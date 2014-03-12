#include <iostream>
#include <fstream>
#include <cstring>
#include <pthread.h>
#include "receive.h"
#include "packetgen.h"
#include "serial.h"
#include "Radio.h"

#define MAX_PACKET_LENGTH 265
using namespace std;

//int parsemessagetype(string mtype);

int preparesourcestream( string ifilename, ifstream& source);

Radio::Radio(string path){
  
  fd_target = initPort(path.c_str(), O_WRONLY);
  if(fd_target < 0){
    cerr <<"Bad port file descriptor.\n";
  }
  device_path = path;

}

int Radio::configureRadio(string configurationFilePath){

  ifstream source;
  int result=preparesourcestream(configurationFilePath, source);
  if(result < 0){ //that would mean the source stream failed
    cerr << "preparesourcestream screwed up" <<endl;
    return(-1);
  }
  int ret = send_config(source, fd_target);
  if(ret < 0)
    cerr << "send_config failed" << endl;
  source.close();
  return 0;
 
}

int Radio::transmitPayload(string payloadFile){

  ifstream source;
  int result=preparesourcestream(payloadFile, source);
  if(result < 0){ //that would mean the source stream failed
    cerr << "prepare failed in transmit Payload"<<endl;
    return(-1);

  }
  send_data(source, fd_target);
  source.close();

}

void *listenThread(void *x){

  string targetfile = "/dev/ttyS1";
 
  int fd_port;
  fd_port = initPort(targetfile.c_str(), O_RDONLY);
  if(fd_port < 0){
    cerr << "Failed to open port. Adios, amigo.\n";
    pthread_exit(NULL);
  }

  unsigned char payload_size;
  unsigned char packet[MAX_PACKET_LENGTH];

  while(true){
    std::cout << "listening" << std::endl;
    packet[0] = SYNC_1;
    packet[1] = SYNC_2;

    waitfor(fd_port, packet ,2);
    std::cout << "saw packet" << std::endl;
    //Next six bytes should be the rest of the header
    //I don't know why this doesn't work, but it gets junk data: read(fd_port, packet+2, 6);

    //let's try this one char at a time:
    for(int i=2; i < 8; i++){ read(fd_port,packet+i,1); }

    if(test_header_checksum(packet)){

      if(has_payload(packet)){
	payload_size = packet[5]; //put size_lower byte directly into payload_size
	for(int i=0; i < (payload_size+2); i++){
	  read(fd_port,packet+8+i,1); //read payload and its checksum
	}
	if(test_payload_checksum(packet+2,payload_size+6)){ //Trying checksum over header too
#ifdef DEBUG
	  write(1,packet,payload_size+10); //write the whole received packet to stdout
#endif
	  handle_payload(packet, payload_size);
	} else {
	  cerr << "Payload checksum failed.\n";
	}

      } else { //Stuff for packets with no payload
	handle_command(packet);
      }

    } else { //Bad header
      cerr << "Bad packet header.  I'm not part of your system!  I threw that packet on the ground!\n";
    }
  }
  close(fd_port);
  pthread_exit(NULL);
}

int Radio::listen(){

  pthread_t thread;
  pthread_create(&thread, NULL, listenThread,NULL);
  return 0;

}



