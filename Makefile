CC      = gcc
CFLAGS  = -g -Wall
RM      = rm -f


default: all

all: uselessh

uselessh: uselessh.c
	$(CC) $(CFLAGS) -o uselessh uselessh.c
clean:
	$(RM) uselessh