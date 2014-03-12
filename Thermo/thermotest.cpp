#include "Thermo.h"

int main(){

  Thermo thermo;
  int16_t* val;
  while(true){
    thermo.read_temp(val);
    std::cerr << *val << std::endl;
    sleep(1);
  }
  return 0;
  


}
