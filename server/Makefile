CC=$(CROSS_COMPILE)gcc
#CC=aarch64-none-linux-gnu-gcc

CFLAGS= -O0 -g -Wall -Werror
CFILES= aesdsocket.c
OUTPUT=aesdsocket

all: aesdsocket

default: aesdsocket

clean:
	-rm -f *.o *.d
	-rm -f $(OUTPUT)

aesdsocket:
	$(CC) $(CFLAGS) $(CFILES) -o $(OUTPUT)