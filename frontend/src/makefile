
TARGET = client


CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -lssl -lcrypto -lpthread -lm -lcurl -lmicrohttpd `pkg-config --libs gtk+-3.0`
LDFLAGS = 


SRC_DIRS = ./register ./auth ./server_com ./login ./homepage ./ticket_detail ./list ./component ./global ./book_seat ./payment ./booklist ./email


SRCS = $(wildcard $(addsuffix /*.c, $(SRC_DIRS)))
OBJS = $(SRCS:.c=.o)


all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(CFLAGS) $(LDFLAGS)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJS) $(TARGET)


# gcc client.c ./register/register.c ./auth/auth.c ./server_com/server_com.c ./login/login.c ./homepage/homepage.c ticket_detail/ticket_detail.c list/list.c ./component/component.c ./global/global.c ./book_seat/book_seat.c ./payment/payment.c ./booklist/booklist.c ./email/email.c -o client `pkg-config --cflags --libs gtk+-3.0` -lssl -lcrypto -lpthread -lm -lcurl -lmicrohttpd