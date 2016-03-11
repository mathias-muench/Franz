#ifndef READDIR_H
#define READDIR_H

void read_dir(char *dir, void (* callback)(char *) );
void read_current_dir(void (* callback)(char *) );

#endif
