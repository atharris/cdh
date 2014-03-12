#include "Magnetometer.h"

int main(){

  Magnetometer mag;
  MagnetometerVector vec;
  int16_t x, y, z;
  while(true){
    vec = mag.read_vector();
    std::cerr << vec.x << std::endl;
    std::cerr << vec.y << std::endl;
    std::cerr << vec.z << std::endl;
    std::cerr << std::endl;
    x = mag.read_x();
    y = mag.read_y();
    z = mag.read_z();
    std::cerr << x << std::endl;
    std::cerr << y << std::endl;
    std::cerr << z << std::endl;
    std::cerr << std::endl;

  }
  return 0;

}
