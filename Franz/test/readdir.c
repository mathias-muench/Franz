#include <stdlib.h>
#include <stdio.h>

#include "readdir.h"

void test(char *filename)
{
    printf("FILE: %s\n", filename);
}

int main(int argc, char** argv) {
    if (argc < 2)
    {
        printf("Parameter directory missing.\n");
        exit(1);
    }
    printf ("Reading entries of %s\n\n", argv[1]);

    read_dir(argv[1], test);
}
