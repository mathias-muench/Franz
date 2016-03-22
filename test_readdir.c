#include <stdlib.h>
#include <stdio.h>

#include "readdir.h"

#include "cu.h"

void test_callback(char *filename)
{
    printf("FILE: %s\n", filename);
}

int read_dir_should_return_false_on_invalid_path() {
  int result = read_dir("test/not/existing/folder", test_callback);

  return result != 0;
}

int read_dir_should_return_false_on_file() {
  int result = read_dir("test_testdir/dir/file1", test_callback);

  return result != 0;
}

int read_dir_should_not_return_false_on_directory() {
  int result = read_dir("test_testdir/dir", test_callback);

  return result == 0;
}

int read_dir_should_not_return_false_on_nested_directories() {
  int result = read_dir("test_testdir", test_callback);

  return result == 0;
}

int main(int argc, char** argv) {
  run_test(read_dir_should_return_false_on_invalid_path);
  run_test(read_dir_should_return_false_on_file);
  run_test(read_dir_should_not_return_false_on_directory);
  run_test(read_dir_should_not_return_false_on_nested_directories);
  run_test(read_dir_should_not_return_false_on_nested_directories);
  exit(!test_result());
}

