CC      := gcc
CFLAGS  := -Wall -Wextra -O2 -march=native -m64 -ffreestanding -nostdlib -nostartfiles -g
LDFLAGS := -e my_entry_pt
FILES   := custlib.o csysclib.o caller.o callee.o

.PHONY: all rebuild clean

all: static-prog dynamic-prog

rebuild: clean all

clean:
	rm -f static-prog dynamic-prog *.o

%.o: %.c
	$(CC) $(CFLAGS) $^ -c -o $@

%.o: %.s
	$(CC) $(CFLAGS) $^ -c -o $@

dynamic-prog: $(FILES)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

static-prog: $(FILES)
	$(CC) -static $(CFLAGS) $^ $(LDFLAGS) -o $@
