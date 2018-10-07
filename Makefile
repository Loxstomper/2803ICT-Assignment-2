all: server client

server: server.o common.o server_func.o job_queue.o thread_pool.o
	gcc -o ./dist/server server.o common.o server_func.o

server.o: server.c
	gcc -c server.c

server_func.o: server_func.c
	gcc -c server_func.c

client: client.o common.o
	gcc -o ./dist/client client.o common.o

client.o: client.c
	gcc -c client.c 

common.o: common.c
	gcc -c common.c

job_queue.o: job_queue.c
	gcc -c job_queue.c

thread_pool.o: thread_pool.c 
	gcc -c thread_pool.c

clean:
	rm *.o
	rm *.gch
