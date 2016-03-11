#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <libgen.h>
#include <time.h>

int parse_file(char* path, name_time_t* result) {
  /*printf("opening file %s\n", path);*/

  char fn[FILENAME_MAX];
  char dn[FILENAME_MAX];

  FILE* fp = fopen(path, "r");
  
  if (!fp) {
    fprintf(stderr, "failed to open file!\n");
    return 0;
  }

  char* line = NULL;
  size_t len = 0;
  ssize_t read;
 
  read = getline(&line, &len, fp);

  if (read == -1) {
    fprintf(stderr, "file is empty!\n");
    return 0;
  }

  if (line[0] != 'A' && len != 7) {
    fprintf(stderr, "file seems not to conform IGC standard!\n");
    return 0;
  }

  strcpy(fn, path);
  char *filename = basename(fn);
  strcpy(dn, path);
  char *dir_name = dirname(dn);

  char manufacturer[4] = {0};
  strncpy(manufacturer, line+1, 3);
  
  char serial_number[4] = {0};
  strncpy(serial_number, line+4, 3);
  
  char flight_count[] = "00"; /* 2 digits, zero padding!*/

  char suffix[4] = {0};

  strncpy(suffix, filename+(strlen(filename)-3), 3);/**/

  char date_str[255];

  while ((read = getline(&line, &len, fp)) != -1) {
    if (strncmp("HFDTE", line, 5) == 0) {
      struct tm tm_struct;
      memset(&tm_struct, 0, sizeof(struct tm));
      setenv("TZ", "");
      strptime(line+5, "%y%m%d", &tm_struct);
      tm_struct.tm_isdst = -1;
      tm_struct.tm_hour = 12;
      result->timestamp = mktime(&tm_struct);
      strftime(date_str, sizeof(date_str), "%Y-%m-%d", &tm_struct);
      break;
    }

    if (line[0] != 'H') {
      fprintf(stderr, "we are done reading the header.\n");
      break;
    }
  }

  fclose(fp);

  if (line)
    free(line);

  char new_path[255];
  sprintf(new_path, "%s/%s-%s-%s-%s.%s", dir_name, date_str, manufacturer, serial_number, flight_count, suffix);
  strcpy(result->name, new_path);

  return 1;
}

