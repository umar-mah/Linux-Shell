CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SRC = temp_shell.c utils.c
OBJ = $(SRC:.c=.o)
TARGET = simple_shell

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

temp_shell.o: temp_shell.c utils.h
	$(CC) $(CFLAGS) -c temp_shell.c -o temp_shell.o

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c -o utils.o

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)

rebuild: clean all

