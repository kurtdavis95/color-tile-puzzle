CC	= g++
CFLAGS	= -std=c++11
LFLAGS	= -lGL -lGLU -lGLEW -DGLEW_STATIC -lSDL2

all: main cleano

main: main.o boardtile.o boardedge.o boardselector.o
	$(CC) -o $@ $^ $(CFLAGS) -w $(LFLAGS)

main.o: main.cpp
	$(CC) -c $(CFLAGS) $(LFLAGS) $^

boardtile.o: boardtile.cpp
	$(CC) -c $(CFLAGS) $(LFLAGS) $^

boardedge.o: boardedge.cpp
	$(CC) -c $(CFLAGS) $(LFLAGS) $^

boardselector.o: boardselector.cpp
	$(CC) -c $(CFLAGS) $(LFLAGS) $^

clean:
	rm -f main *.o

cleano:
	rm -f *.o
