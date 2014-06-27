#include <iostream>
#include "Packet.h"

//Constructor for outgoing packet
Packet::Packet(unsigned char payload_bytes[],unsigned char packet_type, unsigned char payload_bytes_size){

  //fill header
  header[0] = SYNC_1;
  header[1] = SYNC_2;
  header[2] = O_MESSAGE_TYPE;
  header[3] = packet_type;
  header[4] = 0;
  header[5] = payload_size;
  header[6] = 0;
  header[7] = 0;
  calculate_header_checksum(header);
  
  type = packet_type;
  payload = payload_bytes;
  payload_size = payload_bytes_size;
  
  if(payload_bytes_size > 255 || payload_bytes_size < 0){
    std::cerr << "Invalid payload size for packet." << std::endl;
  }
  if(payload != NULL){
    unsigned char payload_with_checksum[payload_size+2];
    int i;
    int size = (int)payload_bytes_size;
    for(i=0;i<size;i++){
      payload_with_checksum[i] = payload[i];
    }
    calculate_payload_checksum(payload_with_checksum,size);
    payload = payload_with_checksum;
  }

}


//Constructor for incoming packet
Packet::Packet(unsigned char packet_bytes[]){

  if(!check_packet_format(packet_bytes)){
    std::cerr << "Constructor: incorrect packet format." << std::endl;
    return;
  }
  int i;
  for(i=0;i<8;i++){
    header[i] = packet_bytes[i];
  }
  type = header[3];
  if(is_ack()){
    switch(type){
    case NO_OP_COMMAND:
      std::cerr << "NO_OP ACK received." << std::endl;
      break;
    case RESET_SYSTEM:
      std::cerr << "RESET_SYSTEM ACK received." << std::endl;
      break;
    case TRANSMIT_DATA:
      std::cerr << "TRANSMIT_DATA ACK received." << std::endl;
      break;
    case SET_TRANSCEIVER_CONFIG:
      std::cerr << "SET_TRANSCEIVER_CONFIG ACK received." << std::endl;
      break;
    case WRITE_FLASH:
      std::cerr << "WRITE_FLASH ACK received." << std::endl;
      break;
    default:
      std::cerr << "ACK for nothing..weird." << std::endl;
    }
  }
  else if(is_nack()){
    switch(type){
    case NO_OP_COMMAND:
      std::cerr << "NO_OP NACK received." << std::endl;
      break;
    case RESET_SYSTEM:
      std::cerr << "RESET_SYSTEM NACK received." << std::endl;
      break;
    case TRANSMIT_DATA:
      std::cerr << "TRANSMIT_DATA NACK received." << std::endl;
      break;
    case SET_TRANSCEIVER_CONFIG:
      std::cerr << "SET_TRANSCEIVER_CONFIG NACK received." << std::endl;
     break;
    case WRITE_FLASH:
      std::cerr << "WRITE_FLASH NACK received." << std::endl;
      break;
    case GET_TRANSCEIVER_CONFIG:
      std::cerr << "GET_TRANSCEIVER_CONFIG NACK received." << std::endl;
      break;
    case TELEMETRY_QUERY:
      std::cerr << "TELEMERY_QUERY NACK received." << std::endl;
      break;
    default:
      std::cerr << "NACK for nothing...weird." << std::endl;
    }
  }
  else{
    if(type == RECEIVE_DATA){
      payload_size = header[5];
      unsigned char payload_bytes[payload_size+2];
      int i;
      int size = (int) payload_size;
      for(i=0;i<size;i++){
	payload_bytes[i] = packet_bytes[i+8];
      }
      calculate_payload_checksum(payload_bytes,size);
      payload = payload_bytes;
    } 
  }
  
}


//checks if packet is a nack; if it is, returns type of nack.
bool Packet::is_nack(){
  
  return header[4] == 0xFF;

}

//checks if packet is an ack; if it is, returns type of ack.
bool Packet::is_ack(){
  
  return header[4] == 0x0A;
  
}

bool Packet::check_packet_format(unsigned char* packet){

  if(sizeof(packet)<8){
    std::cerr << "Constructor: Packet size less than 8." << std::endl;
    return false;
  }

  if(packet[0] != SYNC_1 || packet[1] != SYNC_2){
    std::cerr << "Constructor: sync packets incorrect." << std::endl;
    return false;
  }

  if(packet[2] != I_MESSAGE_TYPE && packet[2] != O_MESSAGE_TYPE){
    std::cerr << "Constructor: incorrect IO type, need I or O" << std::endl;
    return false;
  }
  
  if(packet[3] > 8 || packet[3] < 1){
    std::cerr << "Constructor: invalid message type." << std::endl;
    return false;
  }

  if(packet[4] != 0x00 && packet[4] != 0xFF && packet[4] != 0x0A){
    std::cerr <<"Constructor: invalid first size byte." << std::endl;
    return false;
  }

  if(!test_header_checksum(packet)){
    std::cerr << "Constructor: checksum incorrect" << std::endl;
    return false;
  }
  
  return true;

}

bool Packet::test_header_checksum(unsigned char* packet){

  unsigned char ck_a=0, ck_b=0;
  for(int i=2; i<6; i++){
      ck_a += packet[i];
      ck_b += ck_a;
  }

  if( (packet[6] == ck_a) && (packet[7] == ck_b) ){
    return(true);
  } else {
    return(false);
  }

}


bool Packet::test_payload_checksum(unsigned char* payload, int size){

  unsigned char ck_a=0, ck_b=0;
  for(int i=0; i<size; i++){
      ck_a += payload[i];
      ck_b += ck_a;
  }

  if( (payload[size] == ck_a) && (payload[size+1] == ck_b) )
    return(true);
  else{
    return(false);
  }
}


void Packet::calculate_header_checksum(unsigned char* header){

  char ck_a = 0;
  char ck_b = 0;
  int i;
  for( i = 2; i < 6; i++ )
    {
      ck_a += header[i];
      ck_b += ck_a;
    }
  header[6] = ck_a;
  header[7] = ck_b;
  return;

}

void Packet::calculate_payload_checksum(unsigned char *payload,int size){
  
  char ck_a = 0;
  char ck_b = 0;
  
  for(int i=0;i<size;i++){
    ck_a += payload[i];
    ck_b += ck_a;
  }

  payload[size] = ck_a;
  payload[size+1] = ck_b;

}


unsigned char* Packet::get_bytes(){
  
  if(payload_size == 0){
    //return pointer to only header because there's no payload. assuming no checksum for null payloads
    return header;
  }
  else{
    //move header and payload and payload checksum into same byte array. 
    int p_size = (int)payload_size;
    unsigned char *packet_bytes = new unsigned char[p_size+10];
    memcpy(packet_bytes,header,8); 
    memcpy(packet_bytes+8,payload,p_size+2);
    return packet_bytes;
  }

}


int Packet::get_size(){

  if(payload_size == 0){
    //no payload, only header. header has 8 bytes.
    return 8;
  }
  else{
    return payload_size+10;
  }

}
