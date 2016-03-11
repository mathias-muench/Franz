#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <utime.h>

#include "normfile.h"

void doit(const char *old_filename, const char *new_filename, const struct utimbuf *times) {
	rename(old_filename, new_filename);
	perror("rename");
	utime(new_filename, times);
	perror("utime");
}
