#include "Radio.h"
#include <iostream>
#include <unistd.h>

int main(){

  Radio radio("/dev/ttyS1");
  radio.listen();
  int i;
  //  for(i = 0;i<30;i++){
  radio.transmitPayload("testfile");
    //  }

  return 0;
}
