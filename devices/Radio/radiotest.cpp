#include "Radio.h"
#include <iostream>
#include <unistd.h>

int main(){

  Radio radio("/dev/ttyS1");
  radio.listen();
  int i;
  //  for(i = 0;i<30;i++){
  unsigned char data[10] = {'a','b','c','d','e','f','g','h','i','j'};
  radio.configure_radio("upcfg-hipower");
  radio.transmit_payload(data,sizeof(data));
    //  }

  return 0;
}
