#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "alarm_manager/system_condition/cpu_usage.h"
#include "utils/tokenize.h"
#include "utils/program_log.h"

#define PROC_STAT_CPU_WORKING 3
#define PROC_STAT_CPU_TOTAL 7

char** get_cpu_entry()
{
    my_log("get_cpu_entry: start", 1);

    char* cpu_entry = NULL;
    FILE *cpuinfo = NULL;
    size_t sz;

    while (cpuinfo == NULL)
    {
        my_log("get_cpu_entry: try to fopen", 1);
        cpuinfo = fopen("/proc/stat", "rb");
        usleep(1000);
    }

    getline(&cpu_entry, &sz, cpuinfo);

    char** tokens = tokenize(cpu_entry, 11);

    fclose(cpuinfo);
    free(cpu_entry);
    return tokens;
}

cpu_condition* get_cpu_condition()
{
    my_log("get_cpu_condition: start", 1);

    char** entry = get_cpu_entry();
    cpu_condition* condition = (cpu_condition*)malloc(sizeof(cpu_condition));

    if (condition == NULL)
    {
        my_log("get_cpu_condition: malloc", 0);
        exit(EXIT_FAILURE);
    }

    condition->total_time = 0;
    condition->working_time = 0;
    for (int i = 1; i <= PROC_STAT_CPU_WORKING; i++)
        condition->working_time += atof(entry[i]);
    for (int i = 1; i <= PROC_STAT_CPU_TOTAL; i++)
        condition->total_time += atof(entry[i]);
    for(int i = 0; i < 11; i++)
        free(entry[i]);
    free(entry);
    return condition;
}

double get_cpu_usage()
{
    my_log("get_cpu_usage: start", 1);

    cpu_condition* t1 = get_cpu_condition();
    usleep(100000);
    cpu_condition* t2 = get_cpu_condition();

    double res1 = t1->working_time - t2->working_time;
    double res2 = t1->total_time - t2->total_time;
    
    free(t1);
    free(t2);
    return res1 / res2;
}
