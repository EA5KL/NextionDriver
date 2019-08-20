
CC      = gcc
CXX     = g++
CFLAGS  = -Wall -O2 -D_GNU_SOURCE -lcurl

SOURCE = \
		NextionDriver.c basicFunctions.c processCommands.c processButtons.c helpers.c

all:		clean NextionDriver

NextionDriver:
		$(CC) $(SOURCE) $(CFLAGS) -o NextionDriver

clean:
		$(RM) NextionDriver *.o *.d *.bak *~
