#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int test_parse_file_should_return_null_on_invalid_path() {
  name_time_t* input = malloc(sizeof(name_time_t));
  int result = parse_file("file_does_not_exist", input);

  return result == 0;
}

int test_parse_file_should_return_null_on_empty_file() {
  name_time_t* input = malloc(sizeof(name_time_t));
  int result = parse_file("test/igc/empty", input);

  return result == 0;
}

int test_parse_file_should_return_null_on_invalid_file() {
  name_time_t* input = malloc(sizeof(name_time_t));
  int result = parse_file("test/igc/invalid", input);

  return result == 0;
}

int test_parse_file_should_not_return_null_on_valid_path() {
  name_time_t* input = malloc(sizeof(name_time_t));
  int result = parse_file("test/igc/58cd1vj1.igc", input);

  return result != 0;
}

int test_parse_file_should_return_correct_filename() {
  name_time_t* input = malloc(sizeof(name_time_t));
  int result = parse_file("test/igc/58cd1vj1.igc", input);
  /*printf("actual: %s\n", input->name);/**/
  char* expected = "test/igc/2012-08-15-FLA-1VJ-00.igc";
  /*printf("actual: %s, expected: %s", input->name, expected);/**/
  
  return strcmp(input->name, expected) == 0;
}

int test_parse_file_should_return_correct_time() {
  name_time_t* input = malloc(sizeof(name_time_t));
  int result = parse_file("test/igc/58cd1vj1.igc", input);
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

int test_split_path_should_return_correct_tokens() {
  path_tokens_t tokens = {NULL, NULL, NULL};
  split_path("test/igc/58cd1vjC.igc", &tokens);

  return strcmp(tokens.dirname, "test/igc") == 0
	  && strcmp(tokens.filename, "58cd1vjC") == 0
          && tokens.flight_number_of_the_day  == 12
	  && strcmp(tokens.suffix, "igc") == 0;
}

int main(char** argv, int argc) {
  int success = 0;

  success = test_parse_file_should_return_null_on_invalid_path();/**/
  printf(
    "\x1b[36m[TEST]\x1b[0m parse_file should return null on invalid path : %s\n",
    (success ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );
  
  success = test_parse_file_should_not_return_null_on_valid_path();/**/
  printf(
    "\x1b[36m[TEST]\x1b[0m parse_file should not return null on valid path : %s\n",
    (success ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );
  
  success = test_parse_file_should_return_null_on_empty_file();/**/
  printf(
    "\x1b[36m[TEST]\x1b[0m parse_file should return null on empty file: %s\n",
    (success ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );
  
  success = test_parse_file_should_return_null_on_invalid_file();/**/
  printf(
    "\x1b[36m[TEST]\x1b[0m parse_file should return null on invalid file: %s\n",
    (success ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );
  
  success = test_parse_file_should_return_correct_filename();/**/
  printf(
    "\x1b[36m[TEST]\x1b[0m parse_file should return correct filename: %s\n",
    (success ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );

  success = test_parse_file_should_return_correct_time();/**/
  printf(
    "\x1b[36m[TEST]\x1b[0m parse_file should return correct time: %s\n",
    (success ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );

  success = test_split_path_should_return_correct_tokens();/**/
  printf(
    "\x1b[36m[TEST]\x1b[0m split_path should return correct tokens: %s\n",
    (success ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );

  return 0;
}
