CFLAGS=-O2 -pipe -g
#DEBUG=-g -DDEBUG

OBJ=console.o

.c.o:
	gcc -c -I.. ${CFLAGS} ${DEBUG} $<

all: ${OBJ}

new:
	touch console.c
clean:
	rm ${OBJ}
