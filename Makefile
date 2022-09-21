PREFIX?=/usr/X11R6
CFLAGS?=-Os -pedantic -Wall

all:
	$(CC) $(CFLAGS) -I$(PREFIX)/include vile.c -L$(PREFIX)/lib -lX11 -o vile

clean:
	rm -f vile

