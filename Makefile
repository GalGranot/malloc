CC = gcc
CFLAGS = -w -Wextra -pedantic -std=c99
TARGET = main
SRCS = main.c customAllocator.c
OBJS = $(SRCS:.c=.o)
HDRS = customAllocator.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
