CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJS = hello.o canvas.o scene.o

all: hello

hello: $(OBJS)
	$(CC) $(CFLAGS) -o hello $(OBJS)

hello.o: hello.c canvas.h
	$(CC) $(CFLAGS) -c hello.c

canvas.o: canvas.c canvas.h
	$(CC) $(CFLAGS) -c canvas.c

scene.o: scene.c scene.h
	$(CC) $(CFLAGS) -c scene.c

clean:
	rm -f $(OBJS) hello