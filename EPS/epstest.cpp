#include "Eps.h"


int main(void){
  
  Eps eps;
  float reading = eps.read_battery();
  std::cout << reading << std::endl;
  return 0;

}
