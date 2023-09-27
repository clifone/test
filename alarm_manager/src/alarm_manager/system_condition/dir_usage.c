#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/dir.h>

#include "alarm_manager/system_condition/dir_usage.h"
#include "alarm_manager/system_condition/usage_utils.h"
#include "utils/program_log.h"

unsigned long get_dir_bytes(char* name)
{
    my_log("get_dir_bytes: start", 1);

    unsigned long dir_size = 0;
    struct dirent* dirent;
    DIR* dir = opendir(name);

    if (dir == NULL)
        return 0;

    while ((dirent = readdir(dir)))
    {
        if (dirent->d_name[0] != '.' && dirent->d_name[strlen(dirent->d_name)-1] != '~')
        {
            char buf[PATH_MAX];
            strcpy(buf, name);
            strcat(buf, "/");
            strcat(buf, dirent->d_name);
            if (not_folder(buf))
            {
                struct stat st;
                stat(buf, &st);
                dir_size += st.st_blocks * S_BLKSIZE;
            }
            else dir_size += get_dir_bytes(buf);
        }
    }
    closedir(dir);
    return (unsigned long)dir_size;
}

unsigned long get_dir_usage(char* name)
{
    my_log("get_dir_usage: start", 1);
    return get_dir_bytes(name) / 1024;
}
