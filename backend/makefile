CC = gcc

all: client server connect

client: client.c auth.c
	${CC} client.c auth.c filter.c -o client -lssl -lcrypto

server: server.c
	${CC} server.c auth.c filter.c -o server -lpthread -lssl -lcrypto

connect: connect.c
	${CC} connect.c -o connect -lmysqlclient

clean:
	rm -f *.o server client connect
