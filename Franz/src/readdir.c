#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>

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
            result = lstat(current_entry, &file_stat);
            if (result)
            {
                printf("Error getting stats of file: %s\n", current_entry);
                perror("lstat");
                continue;
            }
            if (S_ISREG(file_stat.st_mode))
            {
                char resolved_path[FILENAME_MAX];
                realpath(current_entry, resolved_path);
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
    free(directory_entries);
}
