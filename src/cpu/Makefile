

CC = gcc
CFLAGS = -std=gnu99 -g
LDFLAGS = -lpthread -lncurses

.PHONY: all clean

OBJS = memory.o cpucore.o display.o 

EXECS = mirriros

all:$(EXECS)

mirriros:mirriros.c $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXECS)
