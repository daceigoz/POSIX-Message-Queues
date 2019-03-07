all: keys server client

keys:
	gcc -g -Wall -c keys.c
	ar -rv libkeys.a keys.o

server: server.c
	gcc -g -Wall server.c -o server -lrt -pthread

client: client.c
	gcc -g -Wall -o client client.c libkeys.a -lrt

clean:
	rm -f client
	rm -f server
	rm -f keys.o
	rm -f libkeys.a
