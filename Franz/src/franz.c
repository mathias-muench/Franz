#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "readdir.h"
#include "normfile.h"

void doit(char *filename)
{
    char new_filename[FILENAME_MAX];

    strcpy(new_filename, filename);
    strcat(new_filename, "+");
    normfile(filename, new_filename, time(NULL) - 3600);
    printf("FILE: %s\n", filename);
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
