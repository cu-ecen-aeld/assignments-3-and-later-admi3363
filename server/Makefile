#CC=$(CROSS_COMPILE)gcc
CC=aarch64-none-linux-gnu-gcc

CFLAGS= -g -Wall -Werror
CFILES= aesdsocket.c
OUTPUT=aesdsocket

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all: aesdsocket
	$(CC) $(CFLAGS) aesdsocket.c -o aesdsocket

clean:
	-rm -f *.o *.d
	-rm -f $(OUTPUT)

default: 
	$(CC) $(CFLAGS) aesdsocket.c -o aesdsocket