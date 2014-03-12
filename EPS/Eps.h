#include <iostream>
#include <stdint.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

#define I2C_ADDRESS 0x2B

struct TempVec{

  float x_pos;
  float x_neg;
  float y_pos;
  float y_neg;
  float z_pos;
  float z_neg;

};

class Eps {

 public:
  Eps();
  int16_t read_adc(char reg);
  void read_temp(TempVec* vec);
  float read_battery();
  float read_bus();

 private:
  int fd;
  

};



