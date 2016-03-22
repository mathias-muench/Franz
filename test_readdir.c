#include <stdlib.h>
#include <stdio.h>

#include "readdir.h"

void test_callback(char *filename)
{
    printf("FILE: %s\n", filename);
}

int test_read_dir_should_return_false_on_invalid_path() {
  int result = read_dir("test/not/existing/folder", test_callback);

  return result != 0;
}

int test_read_dir_should_return_false_on_file() {
  int result = read_dir("test_testdir/dir/file1", test_callback);

  return result != 0;
}

int test_read_dir_should_not_return_false_on_directory() {
  int result = read_dir("test_testdir/dir", test_callback);

  return result == 0;
}

int test_read_dir_should_not_return_false_on_nested_directories() {
  int result = read_dir("test_testdir", test_callback);

  return result == 0;
}

int main(int argc, char** argv) {
  int success = 0;

  success = test_read_dir_should_return_false_on_invalid_path();/**/
  printf(
    "\x1b[36m[TEST]\x1b[0m read_dir should return false on invalid path: %s\n",
    (success ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );

  success = test_read_dir_should_return_false_on_file();/**/
  printf(
    "\x1b[36m[TEST]\x1b[0m read_dir should return false on file: %s\n",
    (success ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );

  success = test_read_dir_should_not_return_false_on_directory();/**/
  printf(
    "\x1b[36m[TEST]\x1b[0m read_dir should not return false on directory: %s\n",
    (success ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );

  success = test_read_dir_should_not_return_false_on_nested_directories();/**/
  printf(
    "\x1b[36m[TEST]\x1b[0m read_dir should not return false on nested directories: %s\n",
    (success ? "\x1b[32mOK\x1b[0m" : "\x1b[31mFAIL\x1b[0m")
  );

  return 0;
}

