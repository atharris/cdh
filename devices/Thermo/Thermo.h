#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <iostream>
#include <stdint.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define CONVERSION_REG 0x0
#define CONFIG_REG     0x1
#define LO_THRESH_REG  0x2
#define HI_THRESH_REG  0x3

#define I2C_ADDRESS 0b1001000


class Thermo{

 public:
  Thermo();
  void read_temp(int16_t *value);

 private:
  int fd;

};

