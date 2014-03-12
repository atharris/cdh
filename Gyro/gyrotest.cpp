#include "Gyro.h"


int main(void){

  Gyro gyro;
  int x = gyro.gyro_read_x();
  int y = gyro.gyro_read_y();
  int z = gyro.gyro_read_z();
  std::cout << "x is: " << x << std::endl;
  std::cout << "y is: " << y << std::endl;
  std::cout << "z is: " << z << std::endl;

  GyroVector vec = gyro.gyro_read_vector();
  std::cout << "vector: " << vec.x << " " << vec.y << " " << " " << vec.z << std::endl;
  return 0;
  

}
