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

int test_parser_should_return_correct_filename() {
  name_time_t* input = malloc(sizeof(name_time_t));
  int result = parse_file("test/igc/58cd1vj1.igc", input);
  char* expected = "test/igc/2012-08-15-FLA-1VJ-00.igc";
  puts(input->name);
  return strcmp(input->name, expected) == 0;
}

int test_parser_should_return_correct_time() {
  name_time_t* input = malloc(sizeof(name_time_t));
  int result = parse_file("test/igc/58cd1vj1.igc", input);
  time_t expected = 1345024800;
  return input->timestamp == expected;
}

  

int main(char** argv, int argc) {
  int result = 0;

  result = test_parser_should_return_null_on_invalid_path();/**/
  printf(
    "\x1b[36m[TEST]\x1b[0m parser should return null on invalid path : %s\n",
    (result ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );
  
  result = test_parser_should_not_return_null_on_valid_path();/**/
  printf(
    "\x1b[36m[TEST]\x1b[0m parser should not return null on valid path : %s\n",
    (result ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );
  
  result = test_parser_should_return_null_on_empty_file();/**/
  printf(
    "\x1b[36m[TEST]\x1b[0m parser should return null on empty file: %s\n",
    (result ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );
  
  result = test_parser_should_return_null_on_invalid_file();/**/
  printf(
    "\x1b[36m[TEST]\x1b[0m parser should return null on invalid file: %s\n",
    (result ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );
  
  result = test_parser_should_return_correct_filename();/**/
  printf(
    "\x1b[36m[TEST]\x1b[0m parser should return correct filename: %s\n",
    (result ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );

  result = test_parser_should_return_correct_time();/**/
  printf(
    "\x1b[36m[TEST]\x1b[0m parser should return correct time: %s\n",
    (result ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );
}
