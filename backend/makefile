CC = gcc

LIBS_CLIENT = -lssl -lcrypto
LIBS_SERVER = -lpthread -lssl -lcrypto
LIBS_CONNECT = -lmysqlclient

SRC_DIR = .
AUTH_DIR = ./auth
FILTER_DIR = ./filter
DB_DIR = ./db
AIRLINE_DIR = ./airline
USER_DIR = ./user
TEST_CONNECT_DIR = ./testConnect

SERVER_SRC = $(SRC_DIR)/server.c $(AUTH_DIR)/auth.c $(FILTER_DIR)/filter.c $(USER_DIR)/user.c $(DB_DIR)/db.c
CONNECT_SRC = $(TEST_CONNECT_DIR)/connect.c $(DB_DIR)/db.c $(AIRLINE_DIR)/airline.c $(USER_DIR)/user.c

BINS = server connect

all: $(BINS)

server: $(SERVER_SRC)
	$(CC) $(SERVER_SRC) -o $@ $(LIBS_SERVER) $(LIBS_CONNECT)

connect: $(CONNECT_SRC)
	$(CC) $(CONNECT_SRC) -o $@ $(LIBS_CONNECT)

clean:
	rm -f $(BINS) *.o