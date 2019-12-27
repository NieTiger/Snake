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

.PHONY: clean
clean:
	rm $(TARGET) $(OBJS)
