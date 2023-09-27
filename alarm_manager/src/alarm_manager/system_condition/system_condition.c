#include <linux/limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#define _GNU_SOURCE
#include <unistd.h>

#include "alarm_manager/system_condition/system_condition.h"
#include "alarm_manager/system_condition/cpu_usage.h"
#include "alarm_manager/system_condition/mem_usage.h"
#include "alarm_manager/system_condition/dir_usage.h"
#include "alarm_manager/system_condition/usage_utils.h"
#include "IPC/shared_mem.h"
#include "IPC/semaphore.h"
#include "utils/program_log.h"

system_condition* get_system_condition()
{
    my_log("get_system_condition: start", 1);

    system_condition* condition = (system_condition*)malloc(sizeof(system_condition));

    if (condition == NULL)
    {
        my_log("get_system_condition: malloc", 0);
        exit(EXIT_FAILURE);
    }

    condition->cpu_usage = get_cpu_usage();
    condition->swap_usage = get_mem_usage(SWP);
    condition->ram_usage = get_mem_usage(RAM);
    strcpy(condition->dir_path, get_dir_path());
    condition->dir_size = get_dir_usage(condition->dir_path);

    return condition;
}

char* condition_to_str(system_condition* condition)
{
    my_log("condition_to_str: start", 1);

    size_t size = snprintf(NULL, 0, "CPU:%f%%; RAM:%.2fgB; SWP:%.2fgB; DIR:%.2fgB; PATH:%s",
        condition->cpu_usage * 100, 
        kb_to_gb(condition->ram_usage), 
        kb_to_gb(condition->swap_usage), 
        kb_to_gb(condition->dir_size), 
        condition->dir_path);

    char* str = (char*)malloc(size);

    snprintf(str, size + 1, "CPU:%f%%; RAM:%.2fgB; SWP:%.2fgB; DIR:%.2fgB; PATH:%s",
        condition->cpu_usage * 100, 
        kb_to_gb(condition->ram_usage), 
        kb_to_gb(condition->swap_usage), 
        kb_to_gb(condition->dir_size), 
        condition->dir_path);
    return str;
}

// GET LIMITS

system_condition* get_limits()
{
    my_log("get_limits: start", 1);

    system_condition* limits = (system_condition*)malloc(sizeof(system_condition));
    if (limits == NULL)
    {
        my_log("get_limits: malloc", 0);
        exit(EXIT_FAILURE);
    }

    sem_t* sem = get_semaphore(DEM_SEM_SHM);
    sem_wait(sem);

    limits->ram_usage = get_ram_limit();
    limits->cpu_usage = get_cpu_limit();
    limits->swap_usage = get_swp_limit();
    strcpy(limits->dir_path, get_dir_path());
    limits->dir_size = get_dir_limit();

    sem_post(sem);
    sem_close(sem);

    return limits;
}

double get_cpu_limit()
{
    my_log("get_cpu_limit: start", 1);
    return ((system_condition*) get_shmem())->cpu_usage;
}

unsigned long get_ram_limit()
{
    my_log("get_ram_limit: start", 1);
    return ((system_condition*) get_shmem())->ram_usage;
}

unsigned long get_dir_limit()
{
    my_log("get_dir_limit: start", 1);
    return ((system_condition*) get_shmem())->dir_size;
}

unsigned long get_swp_limit()
{
    my_log("get_swp_limit: start", 1);
    return ((system_condition*) get_shmem())->swap_usage;
}

char* get_dir_path()
{
    my_log("get_dir_path: start", 1);
    return ((system_condition*) get_shmem())->dir_path;
}

// SET LIMITS

void set_default_limits()
{
    my_log("set_default_limits: start", 1);
    char buf[PATH_SIZE_MAX];

    set_ram_limit(DEFAULT_RAM_LIMIT);
    set_cpu_limit(DEFAULT_CPU_LIMIT);
    set_swp_limit(DEFAULT_SWP_LIMIT);
    set_dir_path(DEFAULT_DIR_PATH(buf));
    set_dir_limit(DEFAULT_DIR_LIMIT(buf));
}

void set_limits(system_condition* limits, char mask) 
{
    my_log("set_limits: start", 1);

    sem_t* sem = get_semaphore(DEM_SEM_SHM);
    sem_wait(sem);

    if (mask & RAM_MASK)
        set_ram_limit(limits->ram_usage);

    if (mask & CPU_MASK)
        set_cpu_limit(limits->cpu_usage);

    if (mask & SWAP_MASK)
        set_swp_limit(limits->swap_usage);

    if (mask & DIR_PATH_MASK)
        set_dir_path(limits->dir_path);

    if (mask & DIR_SIZE_MASK)
        set_dir_limit(limits->dir_size);

    sem_post(sem);
    sem_close(sem);
}

void set_cpu_limit(double cpu_limit)
{
    my_log("set_cpu_limit: start", 1);
    ((system_condition*) get_shmem())->cpu_usage = cpu_limit;
}

void set_ram_limit(unsigned long ram_limit)
{
    my_log("set_ram_limit: start", 1);
    ((system_condition*) get_shmem())->ram_usage = ram_limit;
}

void set_dir_limit(unsigned long dir_limit)
{
    my_log("set_dir_limit: start", 1);
    ((system_condition*) get_shmem())->dir_size = dir_limit;
}

void set_swp_limit(unsigned long swp_limit)
{
    my_log("set_swp_limit: start", 1);
    ((system_condition*) get_shmem())->swap_usage = swp_limit;
}

void set_dir_path(char* dir_path)
{
    my_log("set_dir_path: start", 1);
    strcpy(((system_condition*) get_shmem())->dir_path, dir_path);
}

// MAX VALUES

double get_cpu_max()
{
    my_log("get_cpu_max: start", 1);
    return 1.0;
}

unsigned long get_ram_max()
{
    my_log("get_ram_max: start", 1);
    static unsigned long ram_max = 0;
    if (ram_max == 0)
        ram_max = proc_memory_size(PROC_STAT_RAM_TOTAL);
    return ram_max;
}

unsigned long get_swp_max()
{
    my_log("get_swp_max: start", 1);
    static unsigned long swap_max = 0;
    if (swap_max == 0)
        swap_max = proc_memory_size(PROC_STAT_SWP_TOTAL);
    return swap_max;
}
