#include <iostream> 
#include <pthread.h>

int x; 

pthread_t thread;


void *func(void* v) { 
 while(true) { 
    std:: cout << "Hello World from other thread" << std:: endl; 
   }
 } 

int main() { // main thread 

   pthread_create(&thread,NULL,func,NULL);
   while(true) { 

      std:: cout << "Hello World  from main thread "<<std:: endl;  
   }
      return 0; 
} 
