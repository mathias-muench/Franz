#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <utime.h>
#include <sys/stat.h>

#include "normfile.h"

void normfile (const char *old_filename, const char *new_filename, time_t mtime)
{
    struct utimbuf times;
    struct stat file_stat;

    if (strcmp(old_filename, new_filename) != 0) {
        if (stat(new_filename, &file_stat) && errno == ENOENT) {
            if (rename (old_filename, new_filename)) {
                perror ("rename");
                abort();
            }
        } else {
            fprintf(stderr, "rename %s to %s - file exists\n", old_filename, new_filename);
        }
    }

    times.actime = time (NULL);
    times.modtime = mtime;
    if (utime (new_filename, &times)) {
        perror ("utime");
        abort();
    }
}

#ifdef UNIT_TEST
#include <stdio.h>
#include <time.h>

#include "normfile.h"

int main (int argc, char *argv[])
{
    normfile ("test/foo", "test/bar", time (NULL) - 3600);
    exit(0);
}
#endif

/* vi:ai:ts=4:sw=4:et
*/
