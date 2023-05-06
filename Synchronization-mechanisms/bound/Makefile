# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Werror

# Target
TARGET = bound

# Rules
$(TARGET): bound.c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -f $(TARGET) d

run: 
	./$(TARGET)
