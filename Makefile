all: keys server client

keys:
	gcc -Wall -fPIC -c keys.c

server: server.c
	gcc -Wall server.c -o server -lrt -pthread

client: client.c
	gcc -Wall client.c -o client -lrt

clean:
	rm -f client
	rm -f server
