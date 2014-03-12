OBJS = Rxn.o rxntest.o Lexer.o
CC = g++

rxntest: $(OBJS)
	$(CC) -pthread $(OBJS) -o rxntest

rxntest.o: rxntest.cpp Rxn.o Lexer.h
	$(CC) -c rxntest.cpp

Rxn.o: Rxn.h Rxn.cpp
	$(CC) -c Rxn.cpp

Lexer.o: Lexer.h Lexer.cpp
	$(CC) -c $(CFLAGS) Lexer.cpp