#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <utime.h>

#include "normfile.h"

void doit(const char *filename, const struct utimbuf *times) {
	utime(filename, times);
	if (errno) {
		perror("doit");
		abort();
	}
}
