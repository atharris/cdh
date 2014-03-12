#include <unistd.h>
#include <iostream>

int main() {
  const char *charpointer;
  int child_id = fork();
  std::cout << child_id << std::endl; 
  sleep(2);
  if (child_id == 0)
  {
    execlp("./child", charpointer, NULL);
  }
  else 
  {
    while (true)
    {
      std::cout << "in the main process\n";
      sleep(1);
    }
  }
}
