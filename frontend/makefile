CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0`

SRC = src/homepage.c
OBJ = homepage.o
EXEC = homepage

# Phương thức tạo executable
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

# Phương thức tạo object file từ source
$(OBJ): $(SRC)
	$(CC) -c $(SRC) $(CFLAGS)

# Lệnh clean để xóa các file object và executable cũ
clean:
	rm -f $(OBJ) $(EXEC)
