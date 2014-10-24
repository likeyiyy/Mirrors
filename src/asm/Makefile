

CC = gcc
CFLAGS = -std=gnu99 -g
LDFLAGS = -lpthread -lncurses

.PHONY: all clean

OBJS = hash.o 

EXECS = masm objdump

all:$(EXECS)

masm:masm.c $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

objdump:objdump.c $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXECS)
