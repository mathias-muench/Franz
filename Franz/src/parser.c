#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <libgen.h>
#include <time.h>

int parse_file(char* path, name_time_t* result) {
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

  path_tokens_t tokens = {.filename=NULL, .dirname=NULL, .suffix=NULL};
  split_path(path, &tokens);

  char manufacturer[4] = {0};
  strncpy(manufacturer, line+1, 3);
  
  char serial_number[4] = {0};
  strncpy(serial_number, line+4, 3);
  
  char flight_count[] = "00"; /* 2 digits, zero padding!*/

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
  
  char* flight_counter = "00";

  fclose(fp);

  if (line)
    free(line);

  char new_path[FILENAME_MAX] = {0};

  sprintf(
    new_path,
    "%s/%s-%s-%s-%s.%s",
    tokens.dirname,
    date_str,
    manufacturer,
    serial_number,
    flight_counter,
    tokens.suffix
  );

  result->name = new_path;
  
  return 1;
}

int fnotd_char_to_int(char fnotd) {
  if (fnotd < 65) {
    return (int) (fnotd - 48);
  } else {
    return (int) (fnotd - 55);
  }
}

path_tokens_t* split_path(char* path, path_tokens_t* tokens) {
  char buffer[FILENAME_MAX];

  strcpy(buffer, path);
  tokens->filename = basename(buffer);
  strcpy(buffer, path);
  tokens->dirname = dirname(buffer);
  char* pos_dot = strrchr(tokens->filename, '.');
  tokens->suffix = pos_dot+1;
  *pos_dot = 0;/**/
  pos_dot--;
  tokens->flight_number_of_the_day = fnotd_char_to_int(*pos_dot);
  
  return tokens;
}
