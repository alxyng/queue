CC = gcc
STD = -std=c99
CFLAGS = $(STD) -pthread -I ../src

simple.out: simple.c ../src/queue.h
	$(CC) $(CFLAGS) simple.c -o simple.out
