CC = gcc

all: client server connect

client: client.c ./auth/auth.c
	${CC} client.c ./auth/auth.c ./filter/filter.c -o client -lssl -lcrypto

server: server.c
	${CC} server.c ./auth/auth.c ./filter/filter.c -o server -lpthread -lssl -lcrypto

connect: ./connectDB/connect.c
	${CC} ./connectDB/connect.c -o connect -lmysqlclient

clean:
	rm -f *.o server client connect
