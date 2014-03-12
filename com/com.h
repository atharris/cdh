#include <queue>
#include <pthread>
#include <Radio.h>

#define DATA_SIZE 245;

struct Packet{

  int priority;
  char data[DATA_SIZE];

  bool operator<(Packet* a, Packet* b){
    return a.priority < b.priority;
  }

};



