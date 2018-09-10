all: server client

server: server.o
	gcc -o ./dist/server server.o

server.o: server.c
	gcc -c server.c

client: client.o
	gcc -o ./dist/client client.o

client.o: client.c
	gcc -c client.c

clean:
	rm *.o
	rm *.gch
