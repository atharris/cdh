#include "Rxn.h"

Rxn::Rxn(char* path){

  fd = open(path,O_RDWR | O_NOCTTY | O_NONBLOCK);
  
  struct termios termAttr;//Attributes for serial port
  if(tcgetattr(fd,&termAttr)){
    std::cerr << "Error getting attributes of serial port." << std::endl;
  }
  tcflush(fd, TCIOFLUSH);
  //No parity
  termAttr.c_cflag &= ~PARENB;
  //clear size bits
  termAttr.c_cflag &= ~CSIZE;
  //8 bits of data
  termAttr.c_cflag |= CS8;
  termAttr.c_cflag &= ~CRTSCTS;
  //1 stop bit
  termAttr.c_cflag &= ~CSTOPB;
  termAttr.c_cc[VMIN] = 1;
  termAttr.c_cc[VTIME] = 5;
  //ignore modem lines, enable receiver
  termAttr.c_cflag |= (CLOCAL | CREAD);
  //disable echo
  termAttr.c_cflag &= ~ECHO;
  //set speed at 9600 baud
  cfsetispeed(&termAttr,(speed_t)B9600);
  cfsetospeed(&termAttr,(speed_t)B9600);
  cfmakeraw(&termAttr);
  tcflush(fd,TCIOFLUSH);
  //set attributes now fuck
  tcsetattr(fd,TCSANOW, &termAttr);

  recent_telem = NULL;

}


uint16_t calculate_checksum(RxnCommand* command){

  char* command_bytes = (char*) command;
  int i;
  uint16_t sum = 0;
  for(i=0;i<21;i++){
    sum += *(command_bytes++);
  }
  command->checksum = sum;
  return sum;

}


std::queue<RxnCommand*> Rxn::get_queue(){

  return command_queue;

}



int Rxn::queue_command(RxnCommand* command){

  command_queue.push(command);

}


int Rxn::write_packet(RxnCommand* command){
  command->checksum =  calculate_checksum(command);

  char* command_packet = (char*)command;
  delete command;
  
  if(write(fd,command_packet,PACKET_SIZE) != PACKET_SIZE){
    std::cerr << "Failed to write to rxn wheels." << std::endl;
    return -1;
  }

}


int Rxn::write_x_speed(uint16_t xspeed){

  RxnCommand* command = new RxnCommand();
  command->xspeed = xspeed;
  command->command = SPEED_COMMAND;
  queue_command(command);

}


int Rxn::clear_x(){

  RxnCommand* command = new RxnCommand();
  command->command = CLEAR_X;
  queue_command(command);

}


int Rxn::stop_x(){

  RxnCommand* command = new RxnCommand();
  command->command = STOP_X;
  queue_command(command);

}

int Rxn::read_packet(RxnTelemetry *telem){

  telem = recent_telem;

} 


int Rxn::print_telemetry(){

  
  if(recent_telem != NULL){

    std::cout << "xtach: " << recent_telem->xtach << std::endl;
    std::cout << "xcmd: " << recent_telem->xcmd << std::endl;
    std::cout << "motortemp: " << recent_telem->motortemp << std::endl;
    std::cout << "resetcount: " << recent_telem->resetcount << std::endl;
    std::cout << "status: " << recent_telem->status << std::endl;
    std::cout << "voltage: " << recent_telem->voltage << std::endl;
    std::cout << "pressure: " << recent_telem->pressure << std::endl;
    std::cout << "checksum: " << recent_telem->checksum << std::endl;

  }
  else
    std::cout << "no recent telemetry data." << std::endl;

}


int Rxn::set_telem(RxnTelemetry* telem){

  //  if(recent_telem != NULL)
  //    delete recent_telem;
  recent_telem = telem;
  return 0;

}


int Rxn::get_fd(){
  return fd;
}


void *telemetry_thread(void* x){

  Rxn* rxn = (Rxn*) x;
  int fd = rxn->get_fd();
  char packet[21];//our packet
  
  while(true){

    //Wait for sync bytes
    packet[0] = SYNC_1;
    packet[1] = SYNC_2;
    int i;
    char potential_sync;
    for(i=0;i<2;i++){
      while(true){

	read(fd,&potential_sync,1);

	sleep(1);
	if(potential_sync == packet[i])
	  break;
	else
	  i = 0;
      }
    }

    //got sync bytes. next 21 bytes should be packet. 
    read(fd,packet,21);

    RxnTelemetry* new_telem = (RxnTelemetry*) packet;
    rxn->set_telem(new_telem);
    
    //Get the Rxn command queue, if there is something on it, send it. FIFO

    if(!(rxn->command_queue.empty())){
      std::cout << rxn->command_queue.size() << std::endl;
      RxnCommand* command = rxn->command_queue.front();
      printf("%x\n",command);
      rxn->command_queue.pop();
      std::cout << "Got command, popped." << std::endl;
      rxn->write_packet(command);
      std::cout << "Wrote packet." << std::endl;
    }
    
  }

}


int Rxn::start_telemetry(){

  //send command to start iHz telemetry 
  RxnCommand* command = new RxnCommand();
  std::cout << sizeof(*command) << std::endl;
  command->command = START_AUTO_TELEMETRY;
  write_packet(command);

  //start thread to listen for telemetry on port
  pthread_t thread;
  pthread_create(&thread,NULL, telemetry_thread,this);
  return 0;

}


