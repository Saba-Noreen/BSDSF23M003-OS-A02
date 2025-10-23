# Compiler and flags
CC = gcc
CFLAGS = -Wall

# Paths
SRC = src/ls.c
OBJ = obj/ls.o
BIN = bin/ls

# Default target
all: $(BIN)

# Rule to link object file and create executable
$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN)

# Rule to compile .c to .o
$(OBJ): $(SRC)
	mkdir -p obj bin
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

# Clean rule
clean:
	rm -f $(OBJ) $(BIN)

