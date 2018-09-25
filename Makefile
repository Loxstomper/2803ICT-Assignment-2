all: server client

server: server.o common.o
	gcc -o ./dist/server server.o common.o

server.o: server.c
	gcc -c server.c

client: client.o common.o
	gcc -o ./dist/client client.o common.o

client.o: client.c
	gcc -c client.c 

common.o: common.c
	gcc -c common.c

clean:
	rm *.o
	rm *.gch
