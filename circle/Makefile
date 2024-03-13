TARGET ?= circle
MODE ?= RELEASE
MAIN ?= main

CC = gcc
CFLAGS = -g -Wall -O0 -std=c99 -I./types -D$(MODE) -Isrc/lib
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
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(OBJDIR)/$(MAIN).o $(LDFLAGS)
	codesign -s - -f --entitlements $(MAIN).entitlements $(TARGET)

$(TARGET) : $(MAIN).entitlements

$(MAIN).entitlements:
	/usr/libexec/PlistBuddy -c "Add :com.apple.security.get-task-allow bool true" $(MAIN).entitlements

.PHONY: clean test process

test:
	make TARGET=runtest MODE=TEST MAIN=test
	./runtest

process:
	make TARGET=runprocess MODE=PROCESS MAIN=process_main
	./runprocess world < test.txt

clean:
	@rm -rf $(TARGET) build/objs/*

-include $(OBJDIR)/.depend