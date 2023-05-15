INCLUDE_DIRS = -I/usr/include/opencv4 -I.
LIB_DIRS = 
CC=g++

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= -lrt
LIBS= -L/usr/lib -lopencv_core -lopencv_flann -lopencv_video -lrt

HFILES=  colors.h bounding_box.h
CFILES= 
CPPFILES= centroids.cpp centroidsVout.cpp

SRCS= ${HFILES} ${CFILES}
CPPOBJS= ${CPPFILES:.cpp=.o}

all:	centroids centroidsVout

clean:
	-rm -f *.o *.d
	-rm -f centroids centroidsVout

distclean:
	-rm -f *.o *.d

centroids: centroids.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o `pkg-config --libs opencv4` $(LIBS)

centroidsVout: centroidsVout.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o `pkg-config --libs opencv4` $(LIBS)

depend:

.c.o:
	$(CC) $(CFLAGS) -c $<

.cpp.o:
	$(CC) $(CFLAGS) -c $<
