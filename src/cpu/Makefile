

CC = gcc
CFLAGS = -std=gnu99 -g -O0
LDFLAGS = -lpthread -lncurses

.PHONY: all clean

OBJS = memory.o cpucore.o display.o 

EXECS = mirrors

all:$(EXECS)

mirrors:mirrors.c $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXECS)
