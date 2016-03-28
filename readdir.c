#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>

#include "readdir.h"

int read_dir(const char *dir, void (* callback)(char *))
{
  int result = 0;

  char buf[FILENAME_MAX];
  getcwd(buf, FILENAME_MAX);

  if (!chdir(dir)) {
    result = read_current_dir(callback);
    chdir(buf);

  } else {
    fprintf(stderr, "Failed to enter DIR: %s\n", dir);
    perror("chdir");
    result = 1;
  }

  return result;
}

int read_current_dir(void (* callback)(char *))
{
  DIR *d;
  struct dirent *dir;
  struct stat file_stat;
  int result = 0;
  int dir_entries_count = 0;
  char *directory_entries;

  d = opendir(".");
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      dir_entries_count++;
    }
    directory_entries = malloc(sizeof(char) * dir_entries_count * FILENAME_MAX);
    memset(directory_entries, 0, sizeof(char) * dir_entries_count * FILENAME_MAX);

    rewinddir(d);
    int pos;
    for (pos = 0; ((dir = readdir(d)) != NULL); pos++)
    {
      strcpy(directory_entries + FILENAME_MAX*pos*sizeof(char), dir->d_name);
    }
  }
  closedir(d);

  int i;
  for(i=0; i < dir_entries_count; i++)
  {
    char *current_entry = directory_entries + FILENAME_MAX * i * sizeof(char);
    if (current_entry[0] != '.')
    {
#ifdef __MINGW32__
      result = stat(current_entry, &file_stat);
#else
      result = lstat(current_entry, &file_stat);
#endif
      if (result)
      {
        fprintf(stderr, "Error getting stats of file: %s\n", current_entry);
        perror("stat");
        continue;
      }
      if (S_ISREG(file_stat.st_mode))
      {
        char resolved_path[FILENAME_MAX];
	getcwd(resolved_path, FILENAME_MAX);
	strcat(resolved_path, "/");
	strcat(resolved_path, current_entry);
        callback(resolved_path);
      }
      else if (S_ISDIR(file_stat.st_mode))
      {
        result = chdir(current_entry);
        if(!result)
        {
          read_current_dir(callback);
          result = chdir("..");
          if (result)
          {
            fprintf(stderr, "Error changing to \"..\"\n");
            perror("chdir");
            return 1;
          }
        }
        else
        {
          fprintf(stderr, "Failed to enter DIR: %s\n", current_entry);
          perror("chdir");
          return 1;
        }
      }
      else
      {
        fprintf(stderr, "Failed to enter DIR: %s\n", current_entry);
        perror("chdir");
      }
    }
  }

  free(directory_entries);

  return 0;
}

#ifdef UNIT_TEST
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "readdir.h"

#include "cu.h"

static int callback_called;

void test_callback(char *filename)
{
    printf("FILE: %s\n", filename);
    if (strcmp(filename, "/home/osboxes/Franz/test_testdir/dir/file1") == 0) {
        callback_called = 1;
    }
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

int read_dir_should_call_callback() {
  callback_called = 0;
  read_dir("test_testdir/dir", test_callback);

  return callback_called;
}

int main(int argc, char** argv) {
  run_test(read_dir_should_return_false_on_invalid_path);
  run_test(read_dir_should_return_false_on_file);
  run_test(read_dir_should_not_return_false_on_directory);
  run_test(read_dir_should_not_return_false_on_nested_directories);
  run_test(read_dir_should_call_callback);
  exit(!test_result());
}

#endif
