#include <iostream>
#include <fstream>
#include <cstring>
#include <pthread.h>
//#include "receive.h"
#include "packetgen.h"
#include "serial.h"
#include "Radio.h"

#define MAX_PACKET_LENGTH 265
using namespace std;

//int parsemessagetype(string mtype);

//int preparesourcestream( string ifilename, ifstream& source);

void wait_for(int fd_port, const unsigned char* text, size_t text_size);

Radio::Radio(string path){
  
  fd_target = initPort(path.c_str(), O_WRONLY);
  if(fd_target < 0){
    cerr <<"Bad port file descriptor.\n";
  }
  device_path = path;

}

Packet Radio::receive(){
  

  if(!receive_queue.empty()){
    Packet *packet = receive_queue.front();
    receive_queue.pop();
    return *packet;
  }
  else{
    Packet packet = NULL;
    return packet;
  }

}


/*int Radio::configureRadio(string configurationFilePath){

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
 
  }*/

int Radio::configure_radio(string configure_file_path){
  const char* file = configure_file_path.c_str();
  int fd = open(file,O_RDONLY);
  if(fd < 0){
    std::cerr << "Radio: configure file " << configure_file_path
	      << " does not exist." << std::endl;
    return -1;
  }
  unsigned char* configure_bytes;
  int bytes_read = read(fd,configure_bytes,CONFIG_SIZE);
  if(bytes_read < CONFIG_SIZE){
    std::cerr << "Radio received invalid config file size." << std::endl;
    return -1;
  }
  Packet *packet = new Packet(configure_bytes,SET_TRANSCEIVER_CONFIG,CONFIG_SIZE);
  send_packet(packet);
  return 0;

}


int Radio::send_packet(Packet *packet){
  
  
  unsigned char* packet_bytes = packet->get_bytes();
  int bytes = packet->get_size();
  if(write(fd_target,packet_bytes,bytes)<bytes){
    std::cerr << "Could not write entire packet, not good." << std::endl;
    return -1;
  }
  return 0;
  
  
}


int Radio::transmit_payload(unsigned char payload[], unsigned char payload_size){
  
  Packet *packet = new Packet(payload,TRANSMIT_DATA,payload_size);
  send_packet(packet);
  return 0;

}


/*
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
*/


void *listen_thread(void *x){

  string targetfile = "/dev/ttyS1";

  Radio *radio = (Radio*) x;

  int fd_port;
  fd_port = initPort(targetfile.c_str(), O_RDONLY);
  if(fd_port < 0){
    cerr << "Failed to open port. Adios, amigo.\n";
    pthread_exit(NULL);
  }

  unsigned char payload_size;
  unsigned char packet_bytes[MAX_PACKET_LENGTH];

  while(true){

    packet_bytes[0] = SYNC_1;
    packet_bytes[1] = SYNC_2;

    wait_for(fd_port, packet_bytes ,2);
    int i;
    for(i=2; i < 8; i++){
      read(fd_port,packet_bytes+i,1);
    }
    
    if(packet_bytes[4] == 0){
      payload_size = packet_bytes[5];
      for(i=0;i<payload_size+2;i++){
	read(fd_port,packet_bytes+8+i,1);
      }
    }
    
    Packet *packet = new Packet(packet_bytes);
    radio->receive_queue.push(packet);
    

  }
  close(fd_port);
  pthread_exit(NULL);

}


int Radio::listen(){

  pthread_t thread;
  pthread_create(&thread, NULL, listen_thread,this);
  return 0;

}


void wait_for(int fd_port, const unsigned char* text, size_t text_size){
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


