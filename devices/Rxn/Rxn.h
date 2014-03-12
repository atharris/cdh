
#ifndef RXN_H_
#define RXN_H_

#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <termios.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <queue>

#define SEND_TELEMETRY 0x00
#define SPEED_COMMAND 0x01
#define TORQUE_COMMAND 0x06
#define RESET_I2C 0x09
#define STOP_X 0x11
#define STOP_Y 0x12
#define STOP_X_Y 0x13
#define STOP_Z 0x14
#define STOP_X_Z 0x15
#define STOP_Y_Z 0x16
#define STOP_X_Y_Z 0x17
#define CLEAR_X 0x21
#define CLEAR_Y 0x22
#define CLEAR_X_Y 0x23
#define CLEAR_Z 0x24
#define CLEAR_X_Z 0x25
#define CLEAR_Y_Z 0x26
#define CLEAR_X_Y_Z 0x27
#define STOP_AUTO_TELEMETRY 0x30
#define START_AUTO_TELEMETRY 0x31

#define PACKET_SIZE 23
#define SYNC_1 0x90
#define SYNC_2 0xEB

struct RxnCommand{
  RxnCommand() : sync1(SYNC_1), sync2(SYNC_2){}
  char sync1;
  char sync2;

  uint16_t xspeed;
  int16_t xpos;
  int16_t xtorque;

  uint16_t yspeed;
  int16_t ypos;
  int16_t ytorque;

  uint16_t zspeed;
  int16_t zpos;
  int16_t ztorque;
  
  char command;
  uint16_t checksum;

}__attribute__((packed));

struct RxnTelemetry{
  
  int16_t xtach;
  int16_t xcmd;
  int8_t motortemp;
  int16_t ytach;
  int16_t ycmd;
  uint8_t resetcount;
  int16_t ztach;
  int16_t zcmd;
  uint8_t status;
  int16_t voltage;
  int16_t pressure;
  uint16_t checksum;

}__attribute__((packed));

class Rxn {

 public:
  Rxn(char* path);
  int write_packet(RxnCommand* command);
  int read_packet(RxnTelemetry* telem);
  int start_telemetry();
  int write_x_speed(uint16_t xspeed);
  int stop_x();
  int stop_telemetry();
  int clear_x();
  int print_telemetry();
  int set_telem(RxnTelemetry* telem);//allows listener thread to set most recent telemetry packet seen on port
  int get_fd();
  int queue_command(RxnCommand* command);
  std::queue<RxnCommand*> get_queue();
  std::queue<RxnCommand*> command_queue;  
  
 private:
  int fd;
  RxnTelemetry* recent_telem;//most recent telemetry packet seen on port


  

};





#endif
