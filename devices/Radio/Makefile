#Radio test makefile
OBJS = Radio.o radiotest.o Packet.o
CC = g++

test: $(OBJS)
	$(CC)  -pthread $(OBJS) -o test

radiotest.o: radiotest.cpp Radio.o
	$(CC) -c radiotest.cpp	

Radio.o: Radio.h Radio.cpp Packet.o
	$(CC) -c Radio.cpp

Packet.o: Packet.h Packet.cpp
	$(CC) -c Packet.cpp