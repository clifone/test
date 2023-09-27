#ifndef CPU_USAGE_H_
#define CPU_USAGE_H_

typedef struct cpu_condition
{
    double working_time, total_time;
} cpu_condition;

char** get_cpu_entry(void);
cpu_condition* get_cpu_condition(void);
double get_cpu_usage(void);

#endif // CPU_USAGE_H_
