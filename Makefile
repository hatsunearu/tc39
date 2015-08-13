CC = gcc-5
CFLAGS = -Wall -std=c99

all:
	$(CC) $(CFLAGS) speck-toy.c -o speck-toy

clean:
	rm speck-toy
