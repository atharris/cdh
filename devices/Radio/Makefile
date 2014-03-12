#Radio test makefile
OBJS = Radio.o radiotest.o
CC = arm-linux-g++

test: $(OBJS)
	$(CC)  -pthread $(OBJS) -o test

radiotest.o: radiotest.cpp Radio.o
	$(CC) -c radiotest.cpp	

Radio.o: Radio.h Radio.cpp
	$(CC) -c Radio.cpp