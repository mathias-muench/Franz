#ifndef READDIR_H
#define READDIR_H

int read_dir(char *dir, void (* callback)(char *) );
int read_current_dir(void (* callback)(char *) );

#endif
