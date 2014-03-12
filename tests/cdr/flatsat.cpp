#include "../Radio/Radio.h"
#include "../EPS/Eps.h"


int main(){

  Eps eps;
  float reading = eps.read_bus();
  
  FILE * f = fopen("cdrtestfile","w");
  //  if(fd<0)
  //    std::cerr << "open file faied" << std::endl;
  fprintf(f,"EPS 5V bus Reading: %f",reading);
  //  if(write(fd,&reading,2)<0)
  //    std::cerr << "failed to write" << std::endl;
  fclose(f);
  Radio radio("/dev/ttyS1");
  radio.listen();
  //  radio.configureRadio("config_medpwr");
  int i;
  for(i=0;i<2;i++)
    radio.transmitPayload("cdrtestfile");
  
  return 0;
  


}
