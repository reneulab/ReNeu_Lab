CC=gcc
CFLAGS=-c -Wall
LEAP_LIBRARY := libLeap.so
LDFLAGS=-ldl
CAN_OBJ=canerror.o ntcan.o
OBJ=POSIX_main.o limb.o myCan.o $(CAN_OBJ) 
EXECUTABLE=POSIX_main


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ) 
	$(CXX) -pthread -ggdb -Wall -I. -I/usr/include -L/usr/lib -o POSIX_main $(OBJ) -lpthread -lLeap

POSIX_main.o: $(LDFLAGS) limb.o POSIX_main.cpp 

limb.o: $(LDFLAGS) myCan.o limb.c 

myCan.o: $(LDFLAGS) $(CAN_OBJ) myCan.c

.PHONY:clean
clean:
	rm -f $(EXECUTABLE) myCan.o POSIX_main.o limb.o 
