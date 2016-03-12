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
    /*printf("failed to open file!\n");*/
    return 0;
  }

  char* line = NULL;
  size_t len = 0;
  ssize_t read;
 
  read = getline(&line, &len, fp);

  if (read == -1) {
    /*printf("file is empty!\n");*/
    return 0;
  }

  if (line[0] != 'A' && len != 7) {
    /*printf("file seems not to conform IGC standard!\n");*/
    return 0;
  }

  strcpy(fn, path);
  char *filename = basename(fn);
  strcpy(dn, path);
  char *dir_name = dirname(dn);

  /*printf("found header: %s", line);*/
 
  char manufacturer[4] = {0};
  strncpy(manufacturer, line+1, 3);
  
  /*printf("found manufacturer '%s'\n", manufacturer);*/

  char serial_number[4] = {0};
  strncpy(serial_number, line+4, 3);
  
  /*printf("found serial number '%s'\n", serial_number);*/

  char flight_count[] = "00"; /* 2 digits, zero padding!*/

  char suffix[4] = {0};

  strncpy(suffix, filename+(strlen(filename)-3), 3);/**/

  /*printf("found suffix '%s'\n", suffix);*/

  unsigned char c = filename[(strlen(filename)-5)];
  /*printf("found flightcount '%c'\n", c);*/
  sprintf(flight_count, "%02d", c - '0');

  char year_str[3] = {0};
  char month_str[3] = {0};
  char day_str[3] = {0};
  char date_str[255];

  while ((read = getline(&line, &len, fp)) != -1) {
    /*printf("found header: %s", line);*/
    if (strncmp("HFDTE", line, 5) == 0) {
      struct tm tm_struct;
      memset(&tm_struct, 0, sizeof(struct tm));
      strptime(line+5, "%d%m%y", &tm_struct);
      tm_struct.tm_hour = 11;
      setenv("TZ", "");
      result->timestamp = mktime(&tm_struct);
      
      printf("year %d month %d day %d\n", tm_struct.tm_year, tm_struct.tm_mon, tm_struct.tm_mday);
      strftime(date_str, sizeof(date_str), "%Y-%m-%d", &tm_struct);
      puts(date_str);
      printf("timestamp %d\n", result->timestamp);/**/
    }

    if (line[0] != 'H') {
      /*printf("we are done reading the header.\n");*/
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

