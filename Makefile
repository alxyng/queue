main: main.c queue.h
	gcc -std=c99 main.c -o main

clean:
	rm main
