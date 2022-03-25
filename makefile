src = $(wildcard *.c)
obj = $(src:.c=.o)

CFLAGS = -std=c99 -Wall -pedantic -march=native
LDFLAGS = -lm -lncurses

snake: $(obj)
	$(CC) $(CFLAGS) -g -o $@ $^ $(LDFLAGS)

.PHONEY:
	clean doc

clean:
	rm -f *.o
