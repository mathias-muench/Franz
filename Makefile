CC=gcc
CFLAGS=-ansi -g -Wwrite-strings

.PHONY: test_parser test_readdir test_normfile


franz: parser.o franz.o normfile.o readdir.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test: test_parser test_readdir test_normfile
	./test_parser
	#./test_normfile
	./test_readdir

test_parser: parser.o test_parser.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_normfile: normfile.o test_normfile.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_readdir: readdir.o test_readdir.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
