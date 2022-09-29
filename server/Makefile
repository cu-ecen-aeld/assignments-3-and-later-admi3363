CC=gcc
#CC=aarch64-none-linux-gnu-gcc

CFLAGS= -g -Wall

HFILES= 
CFILES= aesdsocket.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	aesdsocket

clean:
	-rm -f *.o *.d
	-rm -f aesdsocket

game: game.o
	$(CC) $(CFLAGS) -o $@ $@.o -lpthread