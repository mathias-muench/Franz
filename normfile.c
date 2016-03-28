#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <utime.h>

#include "normfile.h"

void normfile (const char *old_filename, const char *new_filename, time_t mtime)
{
	struct utimbuf times;

	rename (old_filename, new_filename);
	if (errno) {
		perror ("rename");
		abort();
	}
	times.actime = time (NULL);
	times.modtime = mtime;
	utime (new_filename, &times);
	if (errno) {
		perror ("utime");
		abort();
	}
}

#ifdef UNIT_TEST
#include <stdio.h>
#include <time.h>

#include "normfile.h"

int main (int argc, char *argv[])
{
#if 0
	normfile ("test/foo", "test/bar", time (NULL) - 3600);
#endif
	exit(0);
}
#endif

/* vi:ai:ts=4:sw=4
*/
