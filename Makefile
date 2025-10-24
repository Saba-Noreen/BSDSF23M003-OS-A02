# Compiler and flags
CC = gcc
CFLAGS = -Wall

# Paths

OBJ = obj/ls.o
BIN = bin/ls

# Default target
all: $(BIN)

# Rule to link object file and create executable
$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN)

# Rule to compile .c to .o
obj/ls.o: src/ls.c
	mkdir -p obj bin
	$(CC) $(CFLAGS) -c src/ls.c -o obj/ls.o 

# Clean rule
clean:
	rm -f obj*/.o bin/ls
