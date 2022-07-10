CC=g++
INCLUDE=../finder/
CFLAGS=-I$(INCLUDE) -Wall -std=c++17 -pthread -O2
TARGET=exec

all: $(TARGET)

$(TARGET): main.o finder.o
	$(CC) $(CFLAGS) main.o finder.o -o $(TARGET).out

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -c

finder.o: $(INCLUDE)finder.cpp $(INCLUDE)finder.hpp
	$(CC) $(CFLAGS) $(INCLUDE)finder.cpp -c

clean:
	rm -rf *.o $(TARGET)