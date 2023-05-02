INCLUDE_DIRS = -I/usr/include/opencv4
LIB_DIRS = 
CC=g++

CDEFS=
CFLAGS= -O3 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= -L/usr/lib -lopencv_core -lopencv_flann -lopencv_video -lrt

HFILES=
CFILES= createVideo.cpp vidstamp.cpp showstamp.cpp scorestamps.cpp colorhist.cpp

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.cpp=.o}

all:	createVideo vidstamp showstamp scorestamps colorhist
clean:
	-rm -f *.o *.d
	-rm -f createVideo vidstamp showstamp scorestamps colorhist

createVideo: createVideo.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o `pkg-config --libs opencv4` $(LIBS)

vidstamp: vidstamp.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o `pkg-config --libs opencv4` $(LIBS)

showstamp: showstamp.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o `pkg-config --libs opencv4` $(LIBS)

scorestamps: scorestamps.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o `pkg-config --libs opencv4` $(LIBS)

colorhist: colorhist.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o `pkg-config --libs opencv4` $(LIBS)

depend:

.cpp.o: $(SRCS)
	$(CC) $(CFLAGS) -c $<
