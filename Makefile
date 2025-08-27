# Simple Makefile for C Projects

# Compiler and flags
CC = clang
CFLAGS = -Wall -g
TARGET = main
SOURCES = main.c

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean up
clean:
	rm -f $(TARGET)

# Rebuild
rebuild: clean all

.PHONY: all run clean rebuild