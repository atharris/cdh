OBJS = Lexer.o gs.o 
CC = g++

gs: $(OBJS)
	$(CC) $(OBJS) -o gs

gs.o: gs.cpp Lexer.h
	$(CC) -c gs.cpp