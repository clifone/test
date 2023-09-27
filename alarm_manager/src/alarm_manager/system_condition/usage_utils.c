#include <sys/stat.h>
#include <math.h>

#include "alarm_manager/system_condition/usage_utils.h"
#include "utils/program_log.h"

int not_folder(char* path)
{
    my_log("not_folder: start", 1);

    struct stat path_stat;
    stat(path, &path_stat);
    return (S_ISREG(path_stat.st_mode));
}

double kb_to_gb(unsigned long kb)
{
    my_log("kb_to_gb: start", 1);

    return (double)kb / pow(1024, 2);
}
