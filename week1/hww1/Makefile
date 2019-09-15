CFLAGS = -c -Wall
CC = gcc

all: hww1

hww1: hww1.o
	${CC} hww1.o -o hww1

hww1.o: hww1.c
	${CC} ${CFLAGS} hww1.c

clean:
	rm -f *.o *~