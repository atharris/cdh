#include "Eps.h"
#include <errno.h>

Eps::Eps(){

  fd = open("/dev/i2c-0",O_RDWR);
  if(fd<0)
    std::cerr << "could not open file" << std::endl;
  
  if(ioctl(fd,I2C_SLAVE,I2C_ADDRESS)<0){
    std::cerr << "Eps ioctl failed." << std::endl;
  }

}

int16_t Eps::read_adc(char reg){

  char buffer[2];
  buffer[0] = 0x00;
  buffer[1] = reg;
  if(write(fd,&buffer,2)<0)
    perror("Write error occured: ");
  usleep(2000);
  int16_t msb;
  int16_t lsb;
  int16_t data;
  if(read(fd,&msb,1)!=1)
    perror("Read error occured: ");
  msb = msb << 8;
  if(read(fd,&lsb,1)!=1)
    perror("Read error occured: ");

  data = msb + lsb;
  data = data & 0b0000001111111111;
  printf("raw: %x\n",data);
  printf("raw: %p\n",data);
  return data; 

}

float convert_temp(float raw){
  return (-.1619*raw)+110.119;
}

void Eps::read_temp(TempVec* vec){
  

}

float Eps::read_battery(){

  float battery = read_adc(0x20);
  battery = (-5.10791 * battery) + 5273.393;
  return battery;

}

float Eps::read_bus(){

  float bus = read_adc(0x1F);
  bus = (bus * -5.01841) + 5053.055;
  return bus;

}



