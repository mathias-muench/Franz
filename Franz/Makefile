CC=gcc
CFLAGS=-I./include -ansi -g

.PHONY: test/parser test/readdir test/normfile


franz: src/parser.o src/franz.o src/normfile.o src/readdir.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test: test/parser test/readdir test/normfile
	@test/parser 2>/dev/null
	#@test/normfile 2>/dev/null
	@test/readdir test/testdir 2>/dev/null

test/parser: src/parser.o test/parser.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test/normfile: src/normfile.o test/normfile.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test/readdir: src/readdir.o test/readdir.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
