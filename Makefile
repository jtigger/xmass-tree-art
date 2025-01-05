CC = clang

# -Wall: enable all warnings
# -Wextra: enable extra warnings
# -std=c99: use C99 standard
# -g: generate debug information
CFLAGS = -Wall -Wextra -std=c99 -g 

OBJS = hello.o canvas.o scene.o

all: echo_date hello

echo_date:
	@echo $(shell date)

hello: $(OBJS)
	@echo "hello:"
	$(CC) $(CFLAGS) -o hello $(OBJS)

hello.o: hello.c canvas.h
	@echo "hello.o:"
	$(CC) $(CFLAGS) -c hello.c

canvas.o: canvas.c canvas.h
	@echo "canvas.o:"
	$(CC) $(CFLAGS) -c canvas.c

scene.o: scene.c scene.h
	@echo "scene.o:"
	$(CC) $(CFLAGS) -c scene.c

clean:
	@echo "clean:"
	rm -f $(OBJS) hello
