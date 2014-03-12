#include <unistd.h>
#include <stdlib.h>
//This will be main control process. Holds system state and multicasts to other processes. Handles many signals.

int mode = 1;

int main(){
  int com_id = fork();
  if(com_id == 0){
    //execlp("com",...)
  }

  int science_id = fork();
  if(science_id == 0){
    //execlp("science",....)
  }

  int adc_id == fork();
  if(adc_id == fork()){
    //execlp("adc",....)
  }

  int health_id == fork();
  if(adc_id == fork()){
    //execlp("health",...)
  }

  while(true){

  }

  return 0;


}
