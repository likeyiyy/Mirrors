

CC = gcc
CFLAGS = -std=gnu99 -g
LDFLAGS = 

.PHONY: all clean

OBJS = memory.o

EXECS = memory_test

all:$(EXECS)

memory_test:memory_test.c $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXECS)
