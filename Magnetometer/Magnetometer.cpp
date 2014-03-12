#include "Magnetometer.h"


Magnetometer::Magnetometer(){
  
  //open device file
  fd = open(I2C_PATH,O_RDWR);
  
  //call ioctl to bind file descriptor to slave address
  int ioctl_return = ioctl(fd,I2C_SLAVE,I2C_ADDRESS);
  if(ioctl_return < 0){
    std::cerr << "ioctl failed for Magnetometer." << std::endl;
  }

  char buffer[2];
  buffer[0] = CONFIG_REG_A;
  buffer[1] = 0b01110000;
  if(write(fd,buffer,2) != 2){
    std::cerr << "Failed to write to Magnetometer." << std::endl;
  }

  buffer[0] = CONFIG_REG_B;
  buffer[1] = 0b00100000;
  if(write(fd,buffer,2) != 2){
    std::cerr << "Failed to write to Magnetometer." << std::endl;
  }

  buffer[0] = MODE_REG;
  buffer[1] = 0x00;
  if(write(fd,buffer,2) != 2){
    std::cerr << "Failed to write to Magnetometer." << std::endl;
  }
  

}

int Magnetometer::magnetometer_read(char regAddr, char *value) { 

  if(write(fd, &regAddr,1) < 1) { 
    std:: cerr << "Failed to write address to device" << std:: endl; 
    return -1; 
 }  

  if(read(fd,value,1) < 0)  { 
    std:: cerr << "Failed to read from device" << std:: endl; 
    return -1; 
 } 
  
  return 0; 

} 

int16_t Magnetometer::merge_register_values(char msb, char lsb){

  int16_t result = (msb*256) + lsb;
  return result;

}

int16_t Magnetometer::read_x(){

  char x_low, x_high;
  if(magnetometer_read(OUT_X_L, &x_low)){
    std::cerr << "Failed to read from OUT_X_LOW." << std::cerr;
    return -1;
  }
  if(magnetometer_read(OUT_X_H, &x_high)){
    std::cerr << "Failed to read from OUT_X_HIGH." << std::cerr;
    return -1;
  }
  int16_t x_result = merge_register_values(x_high,x_low);
  return x_result;


}

int16_t Magnetometer::read_y(){

  char y_low, y_high;
  if(magnetometer_read(OUT_Y_L, &y_low)){
    std::cerr << "Failed to read from OUT_Y_LOW." << std::cerr;
    return -1;
  }
  if(magnetometer_read(OUT_Y_H, &y_high)){
    std::cerr << "Failed to read from OUT_Y_HIGH." << std::cerr;
    return -1;
  }
  int y_result = merge_register_values(y_high,y_low);
  return y_result;


}

int16_t Magnetometer::read_z(){

  char z_low, z_high;
  if(magnetometer_read(OUT_Z_L, &z_low)){
    std::cerr << "Failed to read from OUT_Z_LOW." << std::cerr;
    return -1;
  }
  if(magnetometer_read(OUT_Z_H, &z_high)){
    std::cerr << "Failed to read from OUT_Z_HIGH." << std::cerr;
    return -1;
  }
  int z_result = merge_register_values(z_high,z_low);
  return z_result;

}

MagnetometerVector Magnetometer::read_vector(){

  MagnetometerVector vec;
  vec.x = read_x();
  vec.y = read_y();
  vec.z = read_z();
  return vec;
}
