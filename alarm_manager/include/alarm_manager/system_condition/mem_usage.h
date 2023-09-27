#ifndef MEM_USAGE_H_
#define MEM_USAGE_H_

#define PROC_STAT_RAM_TOTAL 1
#define PROC_STAT_RAM_AVAIL 3
#define PROC_STAT_SWP_TOTAL 15
#define PROC_STAT_SWP_FREE 16

enum mem_type 
{
    RAM = 0, 
    SWP
};

char** get_mem_entry(int page_number);
unsigned long proc_memory_size(int entry_index);
unsigned long get_mem_usage(enum mem_type type);

#endif // MEM_USAGE_H_
