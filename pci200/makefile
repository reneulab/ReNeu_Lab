CC=gcc
CFLAGS=-Wall
LDFLAGS=-ldl
OBJ=myTest.o limb.o
EXECUTABLE=myTest
CAN_OBJ=canerror.o ntcan.o
VPATH=canLib:limb

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ) 
	gcc -o myTest $(OBJ)

myTest.o: limb.o myTest.c 

limb.o: myCan.o limb.c

myCan.o: $(CAN_OBJ) myCan.c

.PHONY:clean
clean:
	rm -f $(EXECUTABLE) myCan.o myTest.o limb.o 
