OBJS = flatsat.o Eps.o Radio.o
CC = arm-linux-g++

flatsat: $(OBJS)
	$(CC) $(OBJS) -pthread -o flatsat

flatsat.o: flatsat.cpp Eps.o Radio.o
	$(CC) -c flatsat.cpp

Radio.o: ../Radio/Radio.cpp ../Radio/Radio.h
	$(CC) -c ../Radio/Radio.cpp

Eps.o: ../EPS/Eps.cpp ../EPS/Eps.h
	$(CC) -c ../EPS/Eps.cpp