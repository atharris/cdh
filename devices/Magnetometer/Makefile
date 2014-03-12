#Magnetorquer test Makefile

OBJS = magtest.o Magnetometer.o
CC = arm-linux-g++

magtest: $(OBJS)
	$(CC) $(OBJS) -o magtest

magtest.o: magtest.cpp Magnetometer.o
	$(CC) -c magtest.cpp

Magnetometer.o: Magnetometer.h Magnetometer.cpp
	$(CC) -c Magnetometer.cpp