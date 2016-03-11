#ifndef NORMFILE_H
#define NORMFILE_H
#include <utime.h>

void doit(const char *old_filename, const char *new_filename, const struct utimbuf *times);
#endif
