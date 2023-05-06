# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Werror

# Target
TARGET = matrix

# Rules
$(TARGET): matrix.c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -f $(TARGET) d

run: 
	./$(TARGET)
