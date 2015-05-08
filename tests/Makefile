CC = gcc
STD = -std=c99
CFLAGS = $(STD) -pthread -I ../src
LIBS = -lcheck

check: check_queue.c ../src/queue.h
	$(CC) $(CFLAGS) check_queue.c -o check_queue.out $(LIBS)
	./check_queue.out
	rm check_queue.out

.PHONY: check
