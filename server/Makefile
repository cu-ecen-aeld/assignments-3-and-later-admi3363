#CC=gcc
#CC=aarch64-none-linux-gnu-gcc

CFLAGS= -g
CFILES= aesdsocket.c
OUTPUT=aesdsocket

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all: 
	$(CC) $(CFLAGS) aesdsocket.c -o aesdsocket

clean:
	-rm -f *.o *.d
	-rm -f $(OUTPUT)

default: 
	$(CC) $(CFLAGS) aesdsocket.c -o aesdsocket