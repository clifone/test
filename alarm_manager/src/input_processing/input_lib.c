#include <stdio.h>
#include <sys/stat.h>
#include "input_processing/input_processing.h"
#include "alarm_manager/system_condition/system_condition.h"
#include "utils/program_log.h"

// Kbytes number to set in ram, swap and dir_size
int get_kbytes(char* arg, unsigned long* value, unsigned long max)
{
    my_log("get_kbytes: start", 1);

    int res = -1;
    double value_lf = 0;
    char c = 0;
    char postfix = 0;

    int items_number = sscanf(arg, "%lf%c%c", &value_lf, &postfix, &c);
    if (items_number == 1)
    {
        *value = (long)value_lf;
        if (*value <= max && *value >= 0)
            res = 0;
    }
    else if (items_number == 2)
    {
        switch (postfix)
        {
            case 'K':
            case 'k':
                *value = (long)value_lf;
                if (*value <= max && *value >= 0)
                    res = 0;
                break;
            case 'M':
            case 'm':
                *value = (long)(value_lf * 1024);
                if (*value <= max && *value >= 0)
                    res = 0;
                break;
            case 'G':
            case 'g':
                *value = (long)(value_lf * 1024 * 1024);
                if (*value <= max && *value >= 0)
                    res = 0;
                break;
        }
    }

    return res;
}

// % or propotion to set in cpu
int get_percent(char* arg, double* value)
{
    my_log("get_percent: start", 1);

    int res = -1;
    char c = 0;
    char postfix = 0;

    int items_number = sscanf(arg, "%lf%c%c", value, &postfix, &c);
    if (items_number == 1 && *value >= 0 && *value <= 1)
    {
        res = 0;
    }
    else if (items_number == 2 && *value >= 0 && *value <= 100 && postfix == '%')
    {
        res = 0;
        *value /= 100;
    }

    return res;
}

// Check if dir exists
int get_dir(char* arg, system_condition* sc)
{
    my_log("get_dir: start", 1);

    // res is -1 => set default directory
    // res is 0 => set passed directory
    int res = -1;
    struct stat sb;
    if (stat(arg, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        res = 0;
        strcpy(sc->dir_path, arg);
    }
    else
    {
        char buf[PATH_SIZE_MAX];
        strcpy(sc->dir_path, DEFAULT_DIR_PATH(buf));
    }
    return res;
}

void help()
{
    printf("NAME:\n");
    printf("\tAlarm manager\n\n");


    printf("DESCRIPTION\n");
    printf("\tCheck system's parameters: ");
    printf("RAM's size, CPU's workload, SWAP's size, directory's size and path.\n\n");


    printf("SYNOPSIS:\n");
    printf("\t./alarm_manager [flags] [options]\n\n");


    printf("FLAGS:\n");
    printf("\t-%c, --%s [...]\n", SET_SHORT, SET_LONG);
    printf("\t\tSet limits in running daemon.\n\n");

    printf("\t-%c, --%s [...]\n", PRINT_SHORT, PRINT_LONG);
    printf("\t\tPrint system conditions listed in running daemon.\n\n");

    printf("\t-%c, --%s\n", PRINT_ALL_SHORT, PRINT_ALL_LONG);
    printf("\t\tPrint system all conditions in running daemon.\n\n");

    printf("\t-%c, --%s\n", LOG_SHORT, LOG_LONG);
    printf("\t\tPrint failure log-file.\n\n");

    printf("\t-%c, --%s\n", DEL_LOG_SHORT, DEL_LOG_LONG);
    printf("\t\tClear failure log-file.\n\n");

    printf("\t-%c, --%s\n", KILL_SHORT, KILL_LONG);
    printf("\t\tKill daemon.\n\n");

    printf("\t-%c, --%s\n", HELP_SHORT, HELP_LONG);
    printf("\t\tPrint help.\n\n");


    printf("OPTIONS:\n");
    printf("\t-%c, --%s [value]\n", RAM_SHORT, RAM_LONG);
    printf("\t\tK- (M-, G-)bytes number. If floating-point value then round to long. Range: [0; ram_max]\n\n");

    printf("\t-%c, --%s [value]\n", CPU_SHORT, CPU_LONG);
    printf("\t\tCpu percent or propotion. Range: [0; 1] or [0%%, 100%%]\n\n");

    printf("\t-%c, --%s [value]\n", SWAP_SHORT, SWAP_LONG);
    printf("\t\tK- (M-, G-)bytes number. If floating-point value then round to long. Range: [0; swap_max]\n\n");

    printf("\t-%c, --%s [value]\n", DIR_PATH_SHORT, DIR_PATH_LONG);
    printf("\t\tDirectory path\n\n");

    printf("\t-%c, --%s [value]\n", DIR_SIZE_SHORT, DIR_SIZE_LONG);
    printf("\t\tK- (M-, G-)bytes number. If floating-point value then round to long. Range: [0; dir_size_max]\n\n");

    printf("\t* Options have values specified after only with flag -s, --set\n\n");


    printf("EXAMPLES\n");
    printf("\tStart daemon by default:\n");
    printf("\t\t./alarm_manager\n\n");

    printf("\tSet RAM's size and CPU workload (equivalent):\n");
    printf("\t\t./alarm_manager --set --ram=4.5G --cpu=80%%\n\n");
    printf("\t\t./alarm_manager -s -r 4.5g -c 0.8\n\n");

    printf("\tPrint directory's path and SWAP's size:\n");
    printf("\t\t./alarm_manager --print --dir_path --swap\n\n");

    printf("\tShow log:\n");
    printf("\t\t./alarm_manager --log\n\n");

    printf("\tClear log:\n");
    printf("\t\t./alarm_manager --del_log\n\n");

    printf("\tKill daemon:\n");
    printf("\t\t./alarm_manager --kill\n\n");
}
