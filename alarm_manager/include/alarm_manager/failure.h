#ifndef FAILURE_H_
#define FAILURE_H_

#include <time.h>
#include "system_condition/system_condition.h"

typedef enum failure_type 
{
    CPU = 0, 
    RAM, 
    SWP, 
    DIREC
} failure_type;

typedef struct failure
{
    time_t time;
    failure_type type;
    system_condition* condition;
    system_condition* limits;
} failure;

void write_log(failure* failure); // If failure occurs, daemon must write to log to journal&stdout 
int show_popup(failure* failure, int argc, char** argv); // and show popup window to user
void clear_log(void);
void print_log(void);
void check_failure(system_condition* limits);

#endif // FAILURE_H_
