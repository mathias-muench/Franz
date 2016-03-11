#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

int test_parser_should_return_null_on_invalid_path() {
  name_time_t* input = malloc(sizeof(name_time_t));
  int result = parse_file("file_does_not_exist", input);

  return result == 0;
}

int test_parser_should_return_null_on_empty_file() {
  name_time_t* input = malloc(sizeof(name_time_t));
  int result = parse_file("test/igc/empty", input);

  return result == 0;
}

int test_parser_should_return_null_on_invalid_file() {
  name_time_t* input = malloc(sizeof(name_time_t));
  int result = parse_file("test/igc/invalid", input);

  return result == 0;
}

int test_parser_should_not_return_null_on_valid_path() {
  name_time_t* input = malloc(sizeof(name_time_t));
  int result = parse_file("test/igc/58cd1vj1.igc", input);

  return result != 0;
}

int main(char** argv, int argc) {
  int result;

  result = test_parser_should_return_null_on_invalid_path();
  printf(
    "parser should return null on invalid path : %s\n",
    (result ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );
  
  result = test_parser_should_not_return_null_on_valid_path();
  printf(
    "parser should not return null on valid path : %s\n",
    (result ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );
  
  result = test_parser_should_return_null_on_empty_file();
  printf(
    "parser should return null on empty file: %s\n",
    (result ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );
  
  result = test_parser_should_return_null_on_invalid_file();
  printf(
    "parser should return null on invalid file: %s\n",
    (result ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );
}
