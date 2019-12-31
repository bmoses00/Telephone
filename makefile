ifeq ($(DEBUG), TRUE)
	CC = gcc -g -o
else
	CC = gcc -o
endif

all:
	$(CC) control control.c
	$(CC) write write.c

clean:
	rm control
	rm write
