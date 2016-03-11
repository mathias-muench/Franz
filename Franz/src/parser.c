#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <libgen.h>

int parse_file(char* path, name_time_t* result) {
  printf("opening file %s\n", path);

  FILE* fp = fopen(path, "r");
  
  if (!fp) {
    printf("failed to open file!\n");
    return 0;
  }

  char* line = NULL;
  size_t len = 0;
  ssize_t read;
 
  read = getline(&line, &len, fp);

  if (read == -1) {
    printf("file is empty!\n");
    return 0;
  }

  if (line[0] != 'A' && len != 7) {
    printf("file seems not to conform IGC standard!\n");
    return 0;
  }

  char path1[1024];
  char path2[1024];

  strcpy(path1, path);
  strcpy(path2, path);

  char* filename = basename(path1);
  char* dir = dirname(path2);

  printf("found header: %s", line);
 
  char manufacturer[4] = {0};
  strncpy(manufacturer, line+1, 3);
  
  printf("found manufacturer '%s'\n", manufacturer);

  char serial_number[4] = {0};
  strncpy(serial_number, line+4, 3);
  
  printf("found serial number '%s'\n", serial_number);

  char flight_count[] = "00"; /* 2 digits, zero padding!*/

  char suffix[4] = {0};

  strncpy(suffix, filename+(strlen(filename)-3), 3);/**/

  printf("found suffix '%s'\n", suffix);

  char year_str[3] = {0};
  char month_str[3] = {0};
  char day_str[3] = {0};

  while ((read = getline(&line, &len, fp)) != -1) {
    /*printf("found header: %s", line);*/
    if (strncmp("HFDTE", line, 5)) {
      strncpy(year_str, line, 2);
      strncpy(month_str, line, 2);
      strncpy(day_str, line, 2);
    }

    if (strncmp("HFFTY", line, 5)) {
    }

    if (line[0] != 'H') {
      printf("we are done reading the header.\n");
      break;
    }
  }

  fclose(fp);

  if (line)
    free(line);

  result->name = path;
  result->timestamp = NULL; 

  return 1;
}

