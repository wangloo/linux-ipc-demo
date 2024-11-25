_all: serv client
serv: serv.c
	gcc serv.c -o serv
client: client.c
	gcc client.c -o client

run: client serv
	./serv
