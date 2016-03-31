#ifndef UNIT_TEST
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "readdir.h"
#include "normfile.h"
#include "parser.h"

void doit(char *filename)
{
    char new_filename[FILENAME_MAX];
    name_time_t result;

    parse_file(filename, &result);
    normfile(filename, result.name, result.timestamp);
}

int main(int argc, char** argv) {
    if (argc < 2)
    {
        printf("Parameter directory missing.\n");
        exit(1);
    }
    printf ("Reading entries of %s\n\n", argv[1]);

    read_dir(argv[1], doit);
}
#else
int main(int c, char *v) {
	return 0;
}
#endif
