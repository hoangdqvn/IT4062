CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm

all: ser cli

cli: tcp-client-sample.c
	 ${CC} -o client tcp-client-sample.c

ser: tcp-server-sample.c
	 ${CC} -o server tcp-server-sample.c


clean:
	rm -f *.o *~