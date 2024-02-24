TARGET ?= circle
MODE ?= RELEASE
MAIN ?= main

CC = gcc
CFLAGS = -g -Wall -O3 -std=c99 -I./types -D$(MODE) -Isrc/lib
LDFLAGS = -lc

OBJDIR=build/objs/$(TARGET)

SOURCES = $(wildcard src/lib/*/*.c)
OBJECTS = $(addprefix $(OBJDIR)/, $(SOURCES:.c=.o))
DEPS = $(addprefix $(OBJDIR)/, $(SOURCES:.c=.d))

all: $(TARGET)


$(OBJDIR)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.d : %.c
	@mkdir -p $(dir $@)
	$(CC) -MM $(CFLAGS) -c $< -o $@
	sed -i '' '1s;^;$(dir $@);' $@

$(OBJDIR)/$(MAIN).o : src/$(MAIN).c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/.depend: $(DEPS)
	echo "# Automatic Dependencies" > $(OBJDIR)/.depend
	cat $(DEPS) >> $(OBJDIR)/.depend

$(TARGET) : $(OBJECTS) $(OBJDIR)/$(MAIN).o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean test

test:
	make TARGET=runtest MODE=TEST MAIN=test
	./runtest

clean:
	@rm -rf $(TARGET) build/objs/*

-include $(OBJDIR)/.depend