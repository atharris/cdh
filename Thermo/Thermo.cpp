#include "Thermo.h"

#define I2C_PATH "/dev/i2c-0"

Thermo::Thermo(){

  fd = open(I2C_PATH,O_RDWR);
  std::cerr << fd << std:: endl;
  int ioctl_return = ioctl(fd,I2C_SLAVE,I2C_ADDRESS);
  if(ioctl_return<0){
    std::cerr << ioctl_return;
    std::cerr << "Thermocouple ioctl failed. Should throw exception." << std::endl;
  }

  char buffer[4];
  buffer[0]=CONFIG_REG;
  buffer[1]=0b10001000;
  buffer[2]=CONFIG_REG;
  buffer[3]=0b11100011;
  if(write(fd,buffer,3)<3){
   std::cerr << "Read error." << std::endl;
  }

}

void Thermo::read_temp(int16_t *value){
  int zero = 0;
  int* zeroPtr = &zero;
  int bytesWrote = write(fd,zeroPtr, 1);
  if (bytesWrote != 1){
  
    std::cerr << "Did not write correct amount of bytes." << std::endl;
  }

  if(read(fd,value,2)<0){
    std::cerr << "Read error." << std::endl;
  }//std::cerr << "This is Value: " << value;
}
