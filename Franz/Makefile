CC=gcc
CFLAGS=-I./include -ansi -g

franz: src/parser.o
	$(CC) -o franz src/parser.o

test: test/parser
	test/parser

test/parser: src/parser.o test/parser.o
	$(CC) -o test/parser src/parser.o test/parser.o

test/normfile: src/normfile.o test/normfile.o 
	$(CC) -o test/normfile src/normfile.o test/normfile.o
