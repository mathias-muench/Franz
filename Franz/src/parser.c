#include "parser.h"
#include <stdio.h>

int parse_file(char* filename, name_time_t* result) {
  printf("opening file %s\n", filename);

  FILE* fp = fopen(filename, "r");
  
  if (!fp) {
    printf("failed to open file!\n");
    return 0;
  }

  return 1;
}

