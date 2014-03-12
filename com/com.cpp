/*
 *Communications process source. 
 *Odds of this compiling currently are slim.
 *
 */
#include "com.h"

std::priority_queue<Packet> transmit_queue;
std::priority_queue<Packet> receive_queue;

pthread_mutex_t transmit_lock;
pthread_mutex_t receive_lock;

Radio radio{"/dev/ttyS1");

int *transmit_thread(void* x){

  while(true){
    
    pthread_mutex_lock(&transmit_lock);
    if(!transmit_queue.empty()){
      //get packet, send to ground
      //radio.transmitPayload(packet);
      //something like that
      //locks for mutext control
    }
    pthread_mutex_unlock(&transmit_lock);

  }

}

int *check_for_packets_thread(void *x){

  //check message queues for com packets, might be able to join this with 
  //transmit_thread and just send straight from kernel's message queue to radio
  //because i'm pretty sure message queues implement priority well.
  while(true){
    //ckeck for shit
  }

}



int main(){

  pthread_t threads[3];
  //start threads



}




