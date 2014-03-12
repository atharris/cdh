//#include <cstring>
#include <linux/types.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <string.h>
#include <iostream>

//slave address for i2c bus
#define SLAVE_ADDRESS 0x68
#define I2C_PATH "/dev/i2c-0"
//register addresses                                                                                                           
#define CTRL_REG1 0x20
#define CTRL_REG4 0x23
#define OUT_X_L   0x28
#define OUT_X_H   0x29
#define OUT_Y_L   0x2A
#define OUT_Y_H   0x2B
#define OUT_Z_L   0x2C
#define OUT_Z_H   0x2D

//Sensitivity for use in final calculation                                                                                     
//software//                                                                                                                   
#define HIGHSENSITIVITY   .00875//use in final calculation for configure sensitivity in software                               
#define MEDSENSITIVITY     .0175
#define LOWSENSITIVITY       .07
//hardware//                                                                                                                   
//write to CTRL_REG4 to configure sensitivity in hardware                                                                      
#define HARDHIGHSENSITIVITY 0x00
#define HARDMEDSENSITIVITY  0x20
#define HARDLOWSENSITIVITY  0x30

//set XX_CURRENT to change all usage of these                                                                                  
//defs with two quick changes.                                                                                                 
#define SOFTWARE_CURRENT HIGHSENSITIVITY
#define HARDWARE_CURRENT HARDHIGHSENSITIVITY

typedef char u8;

typedef struct{

  int x;
  int y;
  int z;

}GyroVector;

class Gyro {

 public:
  Gyro();
  int gyro_read_x();
  int gyro_read_y();
  int gyro_read_z();
  GyroVector gyro_read_vector();
  

 private:
  int merge_register_values(u8 msb, u8 lsb);
  int fd;
  int gyro_read(u8 regAddr, u8 *value);

};



