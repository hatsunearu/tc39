CC = clang
CFLAGS = -Wall

all:
	$(CC) $(CFLAGS) speck-toy.c -o speck-toy

clean:
	rm speck-toy
