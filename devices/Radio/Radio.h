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

#define CONFIG_SIZE 34

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
  std::queue<Packet*> receive_queue;
  Packet receive();

 private:
  string device_path;
  int fd_target;


};

#endif
