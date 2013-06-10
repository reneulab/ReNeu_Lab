CC=gcc
CFLAGS=-Wall
LDFLAGS=-ldl
OBJ=myTest.o limb.o
EXECUTABLE=myTest
CAN_OBJ=canerror.o ntcan.o

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ) 
	gcc -o myTest $(OBJ)

myTest.o: limb.o myTest.c myTest.h

limb.o: myCan.o limb.c limb.h

myCan.o: $(CAN_OBJ) myCan.c myCan.h

.PHONY:clean
clean:
	rm -f $(EXECUTABLE) myCan.o myTest.o limb.o 
