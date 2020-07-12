CC      := gcc
CFLAGS  := -Wall -Wextra -O2 -march=native -m64 -ffreestanding -nostdlib -nostartfiles -g
LDFLAGS := -e my_entry_pt
FILES   := clib.o syscalls.o caller.o callee.o errno.o

.PHONY: all rebuild clean

all: static-program dynamic-program

rebuild: clean all

clean:
	rm -f static-program dynamic-program *.o

%.o: %.c
	$(CC) $(CFLAGS) $^ -c -o $@

%.o: %.s
	$(CC) $(CFLAGS) $^ -c -o $@

dynamic-program: $(FILES)
	$(CC) -dynamic $(CFLAGS) $^ $(LDFLAGS) -o $@

static-program: $(FILES)
	$(CC) -static $(CFLAGS) $^ $(LDFLAGS) -o $@
