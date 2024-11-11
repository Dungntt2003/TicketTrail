CC = gcc

all: client server user_auth filter connect

client: client.c
	${CC} client.c -o client

server: server.c
	${CC} server.c -o server -lpthread

user_auth: main.c auth.c
	${CC} main.c auth.c -o user_auth -lssl -lcrypto

filter: filter.c 
	${CC} filter.c -o filter

connect: connect.c
	${CC} connect.c -o connect -lmysqlclient

clean:
	rm -f *.o server client user_auth
