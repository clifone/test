#ifndef SYSTEM_CONDITION_H_
#define SYSTEM_CONDITION_H_

#include <stdlib.h>
#include <string.h>

#include "input_processing/input_processing.h"

#define DEFAULT_CPU_LIMIT       0.75
#define DEFAULT_RAM_LIMIT       (unsigned long)(0.8 * get_ram_max())
#define DEFAULT_DIR_PATH(buf)   getcwd(buf, PATH_SIZE_MAX) != NULL ? buf : NULL
#define DEFAULT_DIR_LIMIT(buf)  (unsigned long)(1.2 * get_dir_usage(DEFAULT_DIR_PATH(buf)))
#define DEFAULT_SWP_LIMIT       (unsigned long)(0.8 * get_swp_max())

#undef PATH_SIZE_MAX
#define PATH_SIZE_MAX 256

typedef struct system_condition
{
    double cpu_usage;
    unsigned long ram_usage, dir_size, swap_usage;
    char dir_path[PATH_SIZE_MAX];
} system_condition;


system_condition* get_system_condition(void);
char* condition_to_str(system_condition* system_condition);

system_condition* get_limits(void);
double get_cpu_limit(void);
unsigned long get_ram_limit(void);
unsigned long get_dir_limit(void);
unsigned long get_swp_limit(void);
char* get_dir_path(void);

double get_cpu_max(void);
unsigned long get_ram_max(void);
unsigned long get_dir_max(void);
unsigned long get_swp_max(void);

void set_limits(system_condition* limits, char mask);
void set_default_limits(void);
void set_cpu_limit(double cpu_limit);
void set_ram_limit(unsigned long ram_limit);
void set_dir_limit(unsigned long dir_limit);
void set_swp_limit(unsigned long swp_limit);
void set_dir_path(char* dir_path);

#endif // SYSTEM_CONDITION_H_
