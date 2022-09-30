#CC=gcc
#CC=aarch64-none-linux-gnu-gcc

CFLAGS= -g -Wall
CFILES= aesdsocket.c
OUTPUT=aesdsocket

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all: 
	$(CC) aesdsocket.c -o aesdsocket

clean:
	-rm -f *.o *.d
	-rm -f $(OUTPUT)

default: 
	$(CC) aesdsocket.c -o aesdsocket