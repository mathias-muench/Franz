CC=gcc
CFLAGS=-ansi -g -Wwrite-strings

%_utest: %.c
	$(CC) -DUNIT_TEST $(CFLAGS) $(LDFLAGS) -o $@ $^

utest: $(addsuffix _utest,$(basename $(wildcard *.c)))
	./$^

franz: parser.o franz.o normfile.o readdir.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
