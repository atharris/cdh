#Gyro test makefile

OBJS = Gyro.o gyrotest.o
CC = arm-linux-g++

gyrotest: $(OBJS)
	$(CC) $(OBJS) -o gyrotest

gyrotest.o: gyrotest.cpp Gyro.o
	$(CC) -c gyrotest.cpp

Gyro.o: Gyro.h Gyro.cpp
	$(CC) -c Gyro.cpp