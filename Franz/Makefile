CC=gcc
CFLAGS=-I./include -ansi -g

franz: src/parser.o src/franz.o src/normfile.o src/readdir.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test: test/parser
	test/parser

test/parser: src/parser.o test/parser.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test/normfile: src/normfile.o test/normfile.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test/readdir: src/readdir.o test/readdir.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
