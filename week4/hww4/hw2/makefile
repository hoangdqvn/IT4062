CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm

all: ser cli

cli: tcp-client.c
	 ${CC} -o client tcp-client.c

ser: tcp-server.c
	 ${CC} -o server tcp-server.c


clean:
	rm -f *.o *~