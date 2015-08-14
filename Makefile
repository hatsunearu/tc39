CC = gcc-5
CFLAGS = -Wall -std=c99

all:
	$(CC) $(CFLAGS) tc39-main.c -o tc39

clean:
	rm tc39
