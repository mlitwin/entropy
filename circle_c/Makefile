TARGET ?= circle
MODE ?= RELEASE
MAIN ?= main

CC = gcc
CFLAGS = -g -Wall -O0 -std=c99 -I./types -D$(MODE) -Isrc/lib
LDFLAGS =

OBJDIR=build/objs/$(TARGET)

SOURCES = $(wildcard src/lib/*/*.c)
OBJECTS = $(addprefix $(OBJDIR)/, $(SOURCES:.c=.o))
DEPS = $(addprefix $(OBJDIR)/, $(SOURCES:.c=.d))



all: $(TARGET)


$(OBJDIR)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/$(MAIN).o : src/$(MAIN).c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
$(TARGET) : $(OBJECTS) $(OBJDIR)/$(MAIN).o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean test

test:
	make TARGET=runtest MODE=TEST MAIN=test
	./runtest

clean:
	@rm -rf $(TARGET) $(OBJDIR)/*
