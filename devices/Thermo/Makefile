#Makefile for Thermocouple

OBJS = thermotest.o Thermo.o
CC = arm-linux-g++

thermotest: $(OBJS)
	$(CC) $(OBJS) -o thermotest

thermotest.o: thermotest.cpp Thermo.o
	$(CC) -c thermotest.cpp

Thermo.o: Thermo.cpp Thermo.h
	$(CC) -c Thermo.cpp Thermo.h
