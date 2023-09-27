#ifndef INPUT_PROCESSING_H_
#define INPUT_PROCESSING_H_

#include "alarm_manager/system_condition/system_condition.h"

// Bit masks for individual variable output
#define RAM_MASK        0b10000000
#define CPU_MASK        0b01000000
#define SWAP_MASK       0b00100000
#define DIR_PATH_MASK   0b00010000
#define DIR_SIZE_MASK   0b00001000

/**************************************
 * Macro for first long name argument *
 **************************************/

// Flags
#define SET_LONG        "set"
#define PRINT_LONG      "print"
#define PRINT_ALL_LONG  "print_all"
#define LOG_LONG        "log"
#define DEL_LOG_LONG    "del_log"
#define KILL_LONG       "kill"
#define HELP_LONG       "help"

// Options
#define RAM_LONG        "ram"
#define CPU_LONG        "cpu"
#define SWAP_LONG       "swap"
#define DIR_PATH_LONG   "dir_path"
#define DIR_SIZE_LONG   "dir_size"

/***************************************
 * Macro for first short name argument *
 ***************************************/

// Flags
#define SET_SHORT       's'
#define PRINT_SHORT     'p'
#define PRINT_ALL_SHORT 'a'
#define LOG_SHORT       'l'
#define DEL_LOG_SHORT   'd'
#define KILL_SHORT      'k'
#define HELP_SHORT      'h'

// Options
#define RAM_SHORT       'r'
#define CPU_SHORT       'c'
#define SWAP_SHORT      'w'
#define DIR_PATH_SHORT  'o'
#define DIR_SIZE_SHORT  'b'


// Path length for directory path
#define PATH_SIZE_MAX 256

// Defined to detect errors while setting values
typedef enum set_value_type
{
    SV_NONE = 0,
    SV_WAIT,
    SV_RAM,
    SV_SWAP,
    SV_CPU,
    SV_DIR_PATH,
    SV_DIR_SIZE,
} set_value_type;

int input_processing(int argc, char* argv[]);

int get_kbytes(char* arg, unsigned long* value, unsigned long max);
int get_percent(char* arg, double* set_value);
int get_dir(char* arg, struct system_condition* sc);

void help(void);

#endif // INPUT_PROCESSING_H_
