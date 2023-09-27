#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <syslog.h>

#include "alarm_manager/failure.h"
#include "utils/program_log.h"

#define HYSTERESIS_RELATIVE 0.05

void hysteresis_f(char* flag, failure_type type, failure* f, double max_val);
void hysteresis_lu(char* flag, failure_type type, failure* f, unsigned long max_val);

#define hysteresis(FLAG, F_TYPE, F, MAX) \
        _Generic((FLAG, F_TYPE, F, MAX), \
        unsigned long: hysteresis_lu, \
        double: hysteresis_f \
        )(FLAG, F_TYPE, F, MAX)

void popup_fork(failure* f)
{
    #ifdef POPUP
    switch(fork())
    {
        case -1:
            my_log("Unable to fork() return!", 0);
            exit(EXIT_FAILURE);
        case 0:
            show_popup(f, 0, NULL);
            exit(EXIT_SUCCESS);
        default:
            break;
    }
    #endif
}

void hysteresis_f(char* flag, failure_type type, failure* f, double max_val)
{
    my_log("hysteresis_f: start", 1);

    double current = f->condition->cpu_usage,
           limit = f->limits->cpu_usage;
    if (current > (limit + max_val * HYSTERESIS_RELATIVE) && (*flag == 0) ||
       (current < (limit - max_val * HYSTERESIS_RELATIVE) && (*flag == 1)))
    {

        if (*flag == 0)
            popup_fork(f);
        *flag = !(*flag);
        f->type = type;
        write_log(f);
    }
    return;
}

void hysteresis_lu(char* flag, failure_type type, failure* f, unsigned long max_val)
{
    my_log("hysteresis_lu: start", 1);

    unsigned long current, limit;
    switch(type)
    {
        case RAM:
            current = f->condition->ram_usage;
            limit = f->limits->ram_usage;
            break;
        case SWP:
            current = f->condition->swap_usage;
            limit = f->limits->swap_usage;
            break;
        case DIREC:
            current = f->condition->dir_size;
            limit = f->limits->dir_size;
            break;
    }
    if (current > (limit + (unsigned long)(max_val * HYSTERESIS_RELATIVE)) && (*flag == 0) ||
       (current < (limit > (unsigned long)(max_val * HYSTERESIS_RELATIVE) ?
                   limit - (unsigned long)(max_val * HYSTERESIS_RELATIVE) : 0) && (*flag == 1)))
    {
        if (*flag == 0)
            popup_fork(f);
        *flag = !(*flag);
        f->type = type;
        write_log(f);
    }
}

void check_failure(system_condition* limits)
{
    my_log("check_failure: start", 1);

    failure* f = (failure*)malloc(sizeof(failure));
    if (f == NULL)
    {
        my_log("check_failure: start", 1);
        exit(EXIT_FAILURE);
    }

    system_condition* current = get_system_condition();
    if (current == NULL)
    {
        my_log("check_failure: Unable to get current system condition!", 0);
        free(f);
        fflush(stderr);
        exit(EXIT_FAILURE);
    }
    char enable_popup = 0;
    f->condition = current;
    f->limits = limits;
    f->time = time(NULL);

    static char cpu_failure = 0;
    static char ram_failure = 0;
    static char swap_failure = 0;
    static char dir_failure = 0;

    hysteresis(&cpu_failure, CPU, f, get_cpu_max());
    hysteresis(&ram_failure, RAM, f, get_ram_max());
    hysteresis(&swap_failure, SWP, f, get_swp_max());
    hysteresis(&dir_failure, DIREC, f, (unsigned long)(limits->dir_size * HYSTERESIS_RELATIVE));

    free(f);
    free(current);
    return;
}
