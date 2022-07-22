CC=gcc
INCLUDE=../finder/
FLAGS=-I$(INCLUDE) -pthread -Wall -g -std=gnu99
TARGET=exec

all: $(TARGET)

$(TARGET): main.c $(INCLUDE)file_finder.c file_finder.o
	$(CC) $(FLAGS) -o $(TARGET) main.c file_finder.o

file_finder.o: $(INCLUDE)file_finder.c $(INCLUDE)file_finder.h
	$(CC) -c $(FLAGS) $(INCLUDE)file_finder.c

run: exec
	./exec

clean:
	rm -rf *.o $(TARGET)