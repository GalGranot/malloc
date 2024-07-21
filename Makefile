# Compiler and flags
CC = gcc
CFLAGS = -w -Wextra -pedantic -std=c99

# Executable name
TARGET = main

# Source files
SRCS = main.c wet3.c

# Object files
OBJS = $(SRCS:.c=.o)

# Header files
HDRS = wet3.h

# Default rule
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compile source files to object files
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJS)

# Phony targets
.PHONY: all clean
