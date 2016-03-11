#include <stdio.h>
#include <time.h>

#include "normfile.h"

int main(int argc, char *argv[]) {
	struct utimbuf times;
	times.actime = time(NULL);
	times.modtime = time(NULL) - 3600;
	doit("test/foo", "test/bar", &times);
}
