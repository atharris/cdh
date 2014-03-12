#include <iostream>
#include <unistd.h>

int main()
{

	while(true)
	{
		std ::cout << "hello from child process" << std :: endl;
		sleep(1);
	}
	return 0;

}
