CC=g++
STD=-std=c++11 -Wall -pedantic
CF=$(STD)
BUILD_DIR=build

output: hotel.o -ljsoncpp
	$(CC) hotel.o -ljsoncpp -o output

hotel.o: room.hpp hotel.hpp user.hpp hotel.cpp
	$(CC) -c hotel.cpp -ljsoncpp

clean:
	rm -rf *.o *.out &> /dev/null
