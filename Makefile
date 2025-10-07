CC = gcc
CFLAGS = -Wall
SRC = src/ls-v1.0.0.c
OBJ = obj/ls-v1.0.0.o
BIN = bin/ls

all: $(BIN)
$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

obj/ls-v1.0.0.o: $(SRC)
	@mkdir -p obj bin
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

clean:
	rm -f $(OBJ) $(BIN)

