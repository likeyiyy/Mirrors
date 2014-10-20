

CC = gcc
CFLAGS = -std=gnu99 -g
LDFLAGS = -lpthread

.PHONY: all clean

OBJS = memory.o cpucore.o keyboard.o screen.o

EXECS = memory_test test_key

all:$(EXECS)

test_key:test_key.c $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

memory_test:memory_test.c $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXECS)
