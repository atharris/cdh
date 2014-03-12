

#include "Gyro.h"



Gyro::Gyro(){

  //open device file
  fd = open(I2C_PATH, O_RDWR);
  if(fd < 0){
    std::cerr << "Failed to open device file at " << I2C_PATH << " for Gyro." << std::endl;
  }
  
  //call ioctl to bind file descriptor to slave address
  int ioctl_return = ioctl(fd,I2C_SLAVE, SLAVE_ADDRESS);
  if(ioctl_return < 0){
    std::cerr << "ioctl failed for Gyro." << std::endl;
  }
  
  //write config values to config registers, see Datasheet
  u8 buffer[2];
  buffer[0] = CTRL_REG4;
  buffer[1] = HARDWARE_CURRENT;
  if(write(fd, buffer, 2) != 2){
    std::cerr << "Failed to write to Gyro." << std::endl;
  }
  
  //This powers the gyro on.
  buffer[0] = CTRL_REG1;
  buffer[1] = 0x0f;
  if(write(fd, buffer, 2) != 2){
    std::cerr << "Failed to write to Gyro." << std::endl;
  }
  
}

int Gyro::gyro_read(u8 regAddr, u8 *value){

  if(write(fd,&regAddr,1)!=1){
    std::cerr << "Failed to write to Gyro in gyro_read." << std::endl;
    return -1;
  }
  
  if(read(fd,value,1) < 0){
    std::cerr << "Failed to read from Gyro in gyro_read." << std::endl;
    return -1;
  }

  return 0;

}

int Gyro::merge_register_values(u8 msb, u8 lsb){
  
  int16_t result = (msb * 256) + lsb;
  return result;

}

int Gyro::gyro_read_x(){

  u8 x_low, x_high;
  gyro_read(OUT_X_L,&x_low);
  gyro_read(OUT_X_H,&x_high);
  
  int merged = merge_register_values(x_high,x_low);
  int result = SOFTWARE_CURRENT * merged; // TODO: -ZERO_RATE_X
  return result;

}

int Gyro::gyro_read_y(){

  u8 y_low, y_high;
  gyro_read(OUT_X_L,&y_low);
  gyro_read(OUT_X_H,&y_high);
  
  int merged = merge_register_values(y_high,y_low);
  int result = SOFTWARE_CURRENT * merged; // TODO: -ZERO_RATE_Y
  return result;

}

int Gyro::gyro_read_z(){

  u8 z_low, z_high;
  gyro_read(OUT_X_L,&z_low);
  gyro_read(OUT_X_H,&z_high);
  
  int merged = merge_register_values(z_high,z_low);
  int result = SOFTWARE_CURRENT * merged; // TODO: -ZERO_RATE_Z
  return result;

}

GyroVector Gyro::gyro_read_vector(){

  GyroVector vec;
  vec.x = gyro_read_x();
  vec.y = gyro_read_y();
  vec.z = gyro_read_z();

  return vec;
  
}
