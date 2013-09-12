#Fenge Gao Lab9 Makefile

LDFLAGS=-static -Wl,-rpath=/opt/glibc/glibc-2.2.2/lib 
INCLDIR=-I/opt/glibc/glibc-2.2.2/include 
LIBDIR=-L/opt/glibc/glibc-2.2.2/lib
LIBS=-lpthread
all:server client

server:server.c
	${CC} ${LDFLAGS} -o $@ $@.c ${LIBDIR} ${INCLDIR} ${LIBS} 
client:client.c
	gcc -o client -Wall -g client.c
clean:
	rm -rf server client
