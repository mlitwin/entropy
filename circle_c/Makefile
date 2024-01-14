CC = gcc
CFLAGS = -g -Wall -O0 -std=c99 -I./types
LDFLAGS =

SOURCES = main.c types/hashmap.c types/matrix.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = circle
$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	@rm -f $(TARGET) $(OBJECTS) core