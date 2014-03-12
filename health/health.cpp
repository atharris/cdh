#include "health.h"
#include <Thermo.h>

//everything written in this file is stupid but gives the general idea of what this process will do.

int main(){
  Thermo thermo;
  while(true){

    float temp = thermo.read_temp();
    if(temp < min_temp_table[GYRO]){
      //send appropriate signal 
    }
    else if(temp > max_temp_table[GYRO]){
      //send appropriate signal
    }
    //check for out of bounds amperage or temperature levels and act accordingly.
    sleep(1);

  }
  


}
