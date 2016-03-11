#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <limits.h>

#include "readdir.h"

void read_dir(char *dir, void (* callback)(char *))
{
    chdir(dir);
    read_current_dir(callback);
}

void read_current_dir(void (* callback)(char *))
{
    DIR *d;
    struct dirent *dir;
    struct stat file_stat;
    int result = 0;

    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_name[0] != '.')
            {
                result = lstat(dir->d_name, &file_stat);
                if (result)
                {
                    printf("Error getting stats of file: %s\n", dir->d_name);
                    perror("lstat");
                    continue;
                }
                if (S_ISREG(file_stat.st_mode))
                {
                    char resolved_path[FILENAME_MAX];
                    realpath(dir->d_name, resolved_path);
                    callback(resolved_path);
                }
                else if (S_ISDIR(file_stat.st_mode))
                {
                    result = chdir(dir->d_name);
                    if(!result)
                    {
                        read_current_dir(callback);
                        result = chdir("..");
                        if (result)
                        {
                            printf("Error changing to \"..\"\n");
                            perror("chdir");
                            exit(1);
                        }
                    }
                    else
                    {
                        perror("chdir");
                        printf("Failed to enter DIR: %s\n", dir->d_name);
                    }
                }
            }
        }
        closedir(d);
    }
}
