CC = gcc
CFLAGS = -g -Wall -O0 -std=c99 -I./types
LDFLAGS =

OBJDIR=build/objs/

SOURCES = $(wildcard src/lib/*/*.c)
OBJECTS = $(addprefix $(OBJDIR), $(SOURCES:.c=.o))
DEPS = $(addprefix $(OBJDIR), $(SOURCES:.c=.d))

TARGET = circle

all: $(TARGET)


$(OBJDIR)%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)main.o : src/main.c
	@mkdir -p $(dir $@)
	$(CC) $(CXXFLAGS) -c $< -o $@
$(TARGET) : $(OBJECTS) $(OBJDIR)main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	@rm -rf $(TARGET) $(OBJDIR)*
