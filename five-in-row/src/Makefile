#CFLAGS=-O2 -pipe
#DEBUG=-g -DDEBUG
DEBUG=-g

UI=ui

SUBDIRS=${UI}
SRC=5mok.c stack.c
OBJ=5mok.o stack.o

.c.o:
	gcc -c -I. ${CFLAGS} ${DEBUG} $<

all: 5mok 

5mok: ${OBJ} 
	cd ui && make
	gcc -o 5mok ${OBJ} ui/console.o

new: 
	cd ui && make new
	touch ${SRC}
clean:
	cd ui && make clean
	rm 5mok ${OBJ}
