#include "alarm_manager/failure.h"
#include "IPC/semaphore.h"
#include "alarm_manager/daemon.h"
#include "utils/program_log.h"

static FILE* logfile;
static sem_t* sem_for_log;

#define DATE_STRING_LENGTH 20

void write_log(failure* data)
{
    my_log("write_log: start", 1);

    sem_for_log = get_semaphore(DEM_SEM_LOG);

    double value, limit;
    struct tm* now = localtime(&(data->time));
    switch(data->type)
    {
        case 0:
            value = data->condition->cpu_usage;
            limit = data->limits->cpu_usage;
            break;
        case 1:
            value = data->condition->ram_usage;
            limit = data->limits->ram_usage;
            break;
        case 2:
            value = data->condition->swap_usage;
            limit = data->limits->swap_usage;
            break;
        case 3:
            value = data->condition->dir_size;
            limit = data->limits->dir_size;
            break;
        default:
            break;
    }
    char datestr[DATE_STRING_LENGTH];
    strftime(datestr, sizeof(datestr), "%Y.%m.%d %H:%M:%S", now);

    sem_wait(sem_for_log);
    printf("%s (#%d) Value %.2f Limit %.2f\n", datestr, data->type, value, limit);
    fflush(stdout);

    sem_post(sem_for_log);
    sem_close(sem_for_log);
    return;
}

void clear_log()
{
    my_log("clear_log: start", 1);

    sem_for_log = get_semaphore(DEM_SEM_LOG);

    sem_wait(sem_for_log);
    if((logfile = fopen(STDOUT_PATH, "w+")) == NULL)
    {
        sem_post(sem_for_log);
        sem_close(sem_for_log);
        my_log("clear_log: fopen", 0);
        return;
    }
    fclose(logfile);

    sem_post(sem_for_log);
    sem_close(sem_for_log);
    return;
}

void print_log()
{
    my_log("print_log: start", 1);
    
    sem_for_log = get_semaphore(DEM_SEM_LOG);

    int max = 100 * sizeof(char);
    char* string = (char*)malloc(max);
    if (string == NULL)
    {
        my_log("print_log: malloc", 0);
        exit(EXIT_FAILURE);
    }
    char temp;
    int i = 0;
    sem_wait(sem_for_log);
    if((logfile = fopen(STDOUT_PATH, "rt")) == NULL)
    {
        sem_post(sem_for_log);
        sem_close(sem_for_log);
        my_log("print_log: fopen", 0);
        return;
    }
    
    while(!feof(logfile))
    {
        fread(&temp, sizeof(temp), 1, logfile);
        string[i] = temp;
        if(i + 1 == max)
        {
            string = (char*)realloc(string, max * 2);
            if (string == NULL)
            {
                my_log("print_log: realloc", 0);
                exit(EXIT_FAILURE);
            }
            max *= 2 ;
        }
        i++;
    }

    string[i] = 0;
    printf("\n%s", string);
    fclose(logfile);

    sem_post(sem_for_log);
    sem_close(sem_for_log);
    
    free(string);
    return;
}
