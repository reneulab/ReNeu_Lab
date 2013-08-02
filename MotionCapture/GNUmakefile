prefix := $(shell rtai-config --prefix)

ifeq ($(prefix),)
$(error Please add <rtai-install>/bin to your PATH variable)
endif

# Library Directories
LIBDIRS  = -L/usr/realtime/lib 
LIBDIRS += -L/usr/lib 
LIBDIRS += -L/usr/local/lib 
#LIBDIRS += -Llibowlsock.so
#LIBDIRS += -L./lib
#LIBDIRS += -L/usr/X11R6/lib #for GLUT

# Static Libraries
STATICLIBS = /home/hal/phasespace_inc/libowlsock.so

# Include Directories
INCLUDEDIRS = -I/home/hal/phasespace_inc
#INCLUDEDIRS += -I/home/reneu-t1600/comedi_programs/include/eigen-eigen-5097c01bcdc4/Eigen
#INCLUDEDIRS = -I/home/reneu-t1600/phasespace/include/libowlsock
#INCLUDEDIRS += -I/usr/include/
#INCLUDEDIRS += -I/usr/local/include/
#INCLUDEDIRS += -I/usr/realtime/include/ # rtai headers - check if you need the 'realtime' part
#INCLUDEDIRS += -I/usr/src/comedilib/c++/include/
#INCLUDEDIRS = -I/usr/src/rtai/addons/cpp/
#INCLUDEDIRS += -I./include/
#INCLUDEDIRS += -I./src/Externals/eigen3/
#INCLUDEDIRS += -I/usr/X11R6/include  #for GLUT
#INCLUDEDIRS += -I/home/rtai/boost/boost_1_47_0/

CC = $(shell rtai-config --cxx)
LXRT_CFLAGS = $(shell rtai-config --lxrt-cflags)
LXRT_CFLAGS += -D_GNU_SOURCE
LXRT_LDFLAGS = $(shell rtai-config --lxrt-ldflags)

all:: capture_data 	

capture_data: capture_data.cc NiFpga.o
	$(CC) $(LXRT_CFLAGS) $(INCLUDEDIRS) -o $@ $< $(LXRT_LDFLAGS) NiFpga.o -ldl -lkcomedilxrt -lm $(LIBDIRS) $(STATICLIBS) 

clean::
	rm -f *.o capture_data modules.order Module.symvers

.PHONY: clean
