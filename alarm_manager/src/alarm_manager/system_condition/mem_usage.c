#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/dir.h>
#include <time.h>

#include "alarm_manager/system_condition/mem_usage.h"
#include "utils/tokenize.h"
#include "utils/program_log.h"

char** get_mem_entry(int page_number)
{
    my_log("get_mem_entry: start", 1);

    FILE* meminfo = NULL;
    char* mem_entry = NULL;
    size_t sz;

    while (meminfo == NULL)
    {
        my_log("get_mem_entry: try to fopen", 1);
        meminfo = fopen("/proc/meminfo", "rb");
        usleep(1000);
    }

    for (int i = 0; i < page_number; i++)
        getline(&mem_entry, &sz, meminfo); 
    char** tokens = tokenize(mem_entry, 2);
    fclose(meminfo);
    free(mem_entry);

    return tokens;
}

unsigned long proc_memory_size(int entry_index)
{
    my_log("proc_memory_size: start", 1);

    char** mem_entry = get_mem_entry(entry_index);
    unsigned long size = atol(mem_entry[1]);

    for (int i = 0; i < 2; i++)
        free(mem_entry[i]);
    free(mem_entry);

    return size;
}

unsigned long get_mem_usage(enum mem_type type)
{
    my_log("get_mem_usage: start", 1);

    unsigned long dedicated_mem, available_mem;

    switch(type)
    {
        case RAM:
            dedicated_mem = proc_memory_size(PROC_STAT_RAM_TOTAL);
            available_mem = proc_memory_size(PROC_STAT_RAM_AVAIL);
            break;
        case SWP:
            dedicated_mem = proc_memory_size(PROC_STAT_SWP_TOTAL);
            available_mem = proc_memory_size(PROC_STAT_SWP_FREE);
            break;
        default:
            my_log("Invalid type of mem chosen!", 0);
            exit(EXIT_FAILURE);
    }
    return dedicated_mem - available_mem;
}
