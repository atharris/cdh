#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

#define MODEL_SIZE 5

int model[MODEL_SIZE][MODEL_SIZE];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static unsigned long x=123456789, y=362436069, z=521288629;

unsigned long xorshf96(void) {          //period 2^96-1
  unsigned long t;
  x ^= x << 16;
  x ^= x >> 5;
  x ^= x << 1;

  t = x;
  x = y;
  y = z;
  z = t ^ x ^ y;

  return z;
}

void *determination(void* x){

  int i,j;
  model[2][3] = 999;
  while(true){

    pthread_mutex_lock(&lock);
    std::cout << "det locked" << std::endl;

    for(i=0;i<MODEL_SIZE;i++){
      for(j=0;j<MODEL_SIZE;j++){
	std::cout << model[i][j] << " ";
      }
      std::cout << std::endl;
    }

    pthread_mutex_unlock(&lock);
    std::cout << "det unlocked" << std::endl;

  }

}

void *control(void *x){

  int i,j;
  int seed;
  unsigned long random;
  int fd = open("/dev/urandom",O_WRONLY);

  while(true){
    
    pthread_mutex_lock(&lock);
    std::cout << "control locked" << std::endl;

    for(i=0;i<MODEL_SIZE;i++){

      for(j=0;j<MODEL_SIZE;j++){
	//        read(fd,&seed,4);
	//	srand(seed);
	random = xorshf96();
	std::cout << random << seed << std::endl;
	model[i][j] = random;
      }
    }
    pthread_mutex_unlock(&lock);
    std::cout << "control unlocked" << std::endl;

  }

}


int main(){

  pthread_t threads[2];
  pthread_create(&threads[0],NULL,control,NULL);
  pthread_create(&threads[1],NULL,determination,NULL);
  pthread_join(threads[0],NULL);
  pthread_join(threads[1],NULL);
  return 0;


}
