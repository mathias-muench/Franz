CC=gcc
CFLAGS=-I./include -ansi

franz: src/parser.o
	$(CC) -o franz src/parser.o

test/normfile: src/normfile.o test/normfile.o 
	$(CC) -o test/normfile src/normfile.o test/normfile.o
