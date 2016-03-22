#include <stdio.h>
#include <time.h>

#include "normfile.h"

int main(int argc, char *argv[]) {
	normfile("test/foo", "test/bar", time(NULL) - 3600);
}
