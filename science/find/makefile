CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`


all: find_space.cpp find_space.h
	g++ find_space.cpp $(CFLAGS) $(LIBS) -o findspace

clean:
	rm findspace


