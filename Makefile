%_utest: %.c
	$(CC) -DUNIT_TEST $(CFLAGS) $(LDFLAGS) -o $@ $^
	./$@

CC=gcc
CFLAGS=-ansi -g -Wwrite-strings

franz: franz.o parser.o normfile.o readdir.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

utest: $(addsuffix _utest,$(basename $(wildcard *.c)))
