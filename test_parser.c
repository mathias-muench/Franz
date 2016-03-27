#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "parser.h"

#include "cu.h"

int parse_file_should_return_null_on_invalid_path() {
  name_time_t* input = malloc(sizeof(name_time_t));
  int result = parse_file("file_does_not_exist", input);

  return result == 0;
}

int parse_file_should_return_null_on_empty_file() {
  name_time_t* input = malloc(sizeof(name_time_t));
  int result = parse_file("test_igc/empty", input);

  return result == 0;
}

int parse_file_should_return_null_on_invalid_file() {
  name_time_t* input = malloc(sizeof(name_time_t));
  int result = parse_file("test_igc/invalid", input);

  return result == 0;
}

int parse_file_should_not_return_null_on_valid_path() {
  name_time_t input[sizeof(name_time_t)];
  int result = parse_file("test_igc/58cd1vj1.igc", input);

  return result != 0;
}

int parse_file_should_return_correct_filename() {
  name_time_t* input = malloc(sizeof(name_time_t));
  int result = parse_file("test_igc/58cd1vj1.igc", input);
  /*printf("actual: %s\n", input->name);/**/
  char* expected = "test_igc/2012-08-15-FLA-1VJ-00.igc";
  /*printf("actual: %s, expected: %s", input->name, expected);/**/
  
  return strcmp(input->name, expected) == 0;
}

int parse_file_should_return_correct_time() {
  name_time_t* input = malloc(sizeof(name_time_t));
  int result = parse_file("test_igc/58cd1vj1.igc", input);
  struct tm tm_struct = {
    .tm_year = 112,
    .tm_mon = 7,
    .tm_mday = 15,
    .tm_hour = 12,/**/
    .tm_isdst = -1
  };
  time_t expected = mktime(&tm_struct);

  return input->timestamp == expected;
}

int split_path_should_return_correct_tokens() {
  path_tokens_t tokens = {NULL, NULL, 0, NULL};
  split_path("test_igc/58cd1vjC.igc", &tokens);

  return strcmp(tokens.dirname, "test_igc") == 0
	  && strcmp(tokens.filename, "58cd1vjC") == 0
          && tokens.flight_number_of_the_day  == 12
	  && strcmp(tokens.suffix, "igc") == 0;
}

int main(char** argv, int argc) {
  run_test(parse_file_should_return_null_on_invalid_path);
  run_test(parse_file_should_not_return_null_on_valid_path);
  run_test(parse_file_should_return_null_on_empty_file);
  run_test(parse_file_should_return_null_on_invalid_file);
  run_test(parse_file_should_return_correct_filename);
  run_test(parse_file_should_return_correct_time);
  run_test(split_path_should_return_correct_tokens);
  exit(!test_result());
}
