#include <linux/types.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <iostream>
#include <stdint.h>

#define CONFIG_REG_A 0x00
#define CONFIG_REG_B 0x01
#define MODE_REG     0x02
#define OUT_X_H      0x03
#define OUT_X_L      0x04
#define OUT_Z_H      0x05
#define OUT_Z_L      0x06
#define OUT_Y_H      0x07
#define OUT_Y_L      0x08
#define STATUS_REG   0x09
#define ID_REG_A     0x0A
#define ID_REG_B     0x0B
#define ID_REG_C     0x0C

#define I2C_ADDRESS  0x1E //MAYBE NOT

#define I2C_PATH     "/dev/i2c-0"

typedef struct{

  int16_t x;
  int16_t y;
  int16_t z;

}MagnetometerVector;

class Magnetometer {

 public:
  Magnetometer();
  int16_t read_x();
  int16_t read_y();
  int16_t read_z();
  MagnetometerVector read_vector();

 private:
  int16_t merge_register_values(char msb, char lsb);
  int fd;
  int magnetometer_read(char regAddr, char *value);
};
