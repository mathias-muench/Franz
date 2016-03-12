#ifndef INC_PARSER_H_
#define INC_PARSER_H_

#include <stdio.h>
#include <sys/types.h>

typedef struct {
  char* name;
  time_t timestamp;
} name_time_t;

typedef struct {
  char* dirname;
  char* filename;
  int flight_number_of_the_day;
  char* suffix;
} path_tokens_t;

int parse_file(char* filename, name_time_t* result);

path_tokens_t* split_path(char* path, path_tokens_t* tokens);

#endif /* INC_PARSER_H_ */
