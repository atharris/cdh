OBJS = epstest.o Eps.o
CC = arm-linux-g++

epstest: $(OBJS)
	$(CC) $(OBJS) -o epstest

epstest.o: epstest.cpp Eps.o
	$(CC) -c epstest.cpp

Eps.o: Eps.cpp Eps.h
	$(CC) -c Eps.cpp