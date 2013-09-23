CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-ldl
OBJ=POSIX_main.o limb.o myCan.o $(CAN_OBJ) NiFpga.o
EXECUTABLE=POSIX_main
CAN_OBJ=canerror.o ntcan.o

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ) 
	$(CC) -pthread -ldl -ggdb -o POSIX_main $(OBJ)

POSIX_main.o: $(LDFLAGS) limb.o POSIX_main.c 

limb.o: myCan.o limb.c 

myCan.o: $(CAN_OBJ) myCan.c 

.PHONY:clean
clean:
	rm -f $(EXECUTABLE) myCan.o POSIX_main.o limb.o 
