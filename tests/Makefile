CC = gcc
STD = -std=c99
CFLAGS = $(STD) -I ../src
LIBS = -lcheck

check: check_queue.c ../src/queue.h
	$(CC) $(CFLAGS) check_queue.c -o check_queue $(LIBS)
	./check_queue
	rm check_queue

.PHONY: check
