CFLAGS = -c -Wall
CC = gcc

all: resolver

resolver: resolver.o
	${CC} resolver.o -o resolver

resolver.o: resolver.c
	${CC} ${CFLAGS} resolver.c

clean:
	rm -f *.o *~