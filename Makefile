all: keys server client

keys:
	gcc -Wall -c keys.c
	ar -rv libkeys.a keys.o

server: server.c
	gcc -Wall server.c -o server -lrt -pthread

client: client.c
	gcc -Wall -o client client.c libkeys.a -lrt

clean:
	rm -f client
	rm -f server
	rm -f keys.o
	rm -f libkeys.a
