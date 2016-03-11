#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <utime.h>

#include "normfile.h"

void normfile(const char *old_filename, const char *new_filename, time_t mtime) {
	struct utimbuf times;

	rename(old_filename, new_filename);
	perror("rename");
	times.actime = time(NULL);
	times.modtime = time(NULL) - 3600;
	utime(new_filename, &times);
	perror("utime");
}
