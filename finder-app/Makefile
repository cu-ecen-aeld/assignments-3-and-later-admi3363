#CC=$(CROSS_COMPILE)gcc
#CC=aarch64-none-linux-gnu-gcc

CFLAGS= -g -Wall
CFILES= writer.c
OUTPUT=writer

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all: writer
	$(CC) $(CFLAGS) writer.c -o writer


clean:
	-rm -f *.o *.d
	-rm -f $(OUTPUT)