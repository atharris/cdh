#ifndef SERIAL_H_
#define SERIAL_H_

#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <iostream>

//opens serial port as O_RDONLY, O_WRONLY, or O_RDWR.
//port params are hard-coded 9600 8N1 echo-off for now
//returns file descriptor of port, either opened and configured or a negative error indicator
inline int initPort(const char* port_filename, mode_t mode){

	struct termios termAttr;
	int fd_port;

	fd_port = open(port_filename,mode);

	if(fd_port >= 0){
		//We has a port.  Configurateify that thing.

		tcgetattr(fd_port, &termAttr);
		cfmakeraw(&termAttr);
      //8 N 1:
      //no parity
      termAttr.c_cflag &= ~PARENB;
      //clear size bits
      termAttr.c_cflag &= ~CSIZE;
      //set "8 data bits" flag
      termAttr.c_cflag |= CS8;
      termAttr.c_cflag &= ~CSTOPB;
      termAttr.c_cflag |= (CLOCAL | CREAD);
      //ECHO OFF!  Echo off!  echo off  (echo off)
      termAttr.c_cflag &= ~ECHO;
      cfsetspeed(&termAttr, B9600);

      tcsetattr(fd_port, TCSANOW, &termAttr);
	}
	else{
		//Oops.
		cerr << "initPort: Failed to open serial port.  No settings applied.\n";
	}
	return(fd_port);
}

#endif
