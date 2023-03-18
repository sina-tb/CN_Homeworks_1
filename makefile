CC=g++
STD=-std=c++11 -Wall -pedantic
CF=$(STD)
BUILD_DIR=build

# output: hotel.o -ljsoncpp 
# 	$(CC) hotel.o -ljsoncpp -o output 

code: user hotel

hotel: room.hpp hotel.hpp user.hpp hotel.cpp Error.cpp 
	$(CC) hotel.cpp -ljsoncpp -o h.out

# server: server.cpp
# 	$(CC) server.cpp -ljsoncpp -o s.out

user: user.cpp
	$(CC) user.cpp -ljsoncpp -o c.out

clean:
	rm -rf *.o *.out &> /dev/null
