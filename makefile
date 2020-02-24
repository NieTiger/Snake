TARGET=snake

CC=gcc
CFLAGS+=-Wall -Werror
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
HDRS := $(wildcard *.h)
LIBS := -lncurses

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LIBS) $(CFLAGS)

.PHONY: install
install: all
	cp $(TARGET) /usr/local/bin

uninstall:
	rm /usr/local/bin/$(TARGET)

.PHONY: clean
clean:
	rm $(TARGET) $(OBJS)
