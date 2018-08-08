CC=gcc
CFLAGS=-I.

make: functions.o main.o
	$(CC) -o main functions.o main.o -I.
	rm functions.o main.o