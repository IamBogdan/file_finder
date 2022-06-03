CC=g++
CFLAGS=-Wall -std=c++17 -pthread

all: mstart

mstart: main.cpp ../finder/finder.cpp
	$(CC) $(CFLAGS) main.cpp ../finder/finder.cpp -o exec.out

clean:
	rm -rf *.o mstart