#ifndef NORMFILE_H
#define NORMFILE_H
#include <utime.h>

void doit(const char *filename, const struct utimbuf *times);
#endif
