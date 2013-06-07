CC=gcc
CFLAGS=-Wall


myTest : limb.o myTest.o
	gcc -o myTest limb.o myTest.o

myTest.o : limb.o myTest.c
	gcc -c myTest.c 

limb.o : myCan.o limb.c
	gcc -c limb.c 

myCan.o : canerror.o ntcan.o myCan.c
	gcc -c myCan.c 


clean: rm -f myTest myTest.o limb.o myCan.o
