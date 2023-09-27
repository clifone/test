#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "IPC/semaphore.h"
#include "utils/program_log.h"

static char key_name[20];
static sem_t* semaphore;

sem_t* get_semaphore(int key)
{
    my_log("get_semaphore: start", 1);

    snprintf(key_name, sizeof(key_name), "%s%d","/semfordemon", key);
    semaphore = sem_open(key_name, O_CREAT, 0666, DEM_SEM_BEG_VAL);
    if (semaphore == SEM_FAILED)
    {
        char buf[256];
        my_log(strerror(errno), 0);
        exit(EXIT_FAILURE);
    }
    return semaphore;
}

int destroy_semaphore(int key)
{
    my_log("destroy_semaphore: start", 1);
    
    snprintf(key_name, sizeof(key_name), "%s%d","/semfordemon", key);
    semaphore = sem_open(key_name, O_CREAT);
    if (semaphore == SEM_FAILED)
    {
        my_log("destroy_semaphore: sem_open", 0);
        exit(EXIT_FAILURE);
    }
    if (sem_close(semaphore) == -1)
    {
        my_log("destroy_semaphore: sem_close", 0);
        exit(EXIT_FAILURE);
    }
    if (sem_unlink(key_name) == -1)
    {
        my_log("destroy_semaphore: sem_unlink", 0);
        exit(EXIT_FAILURE);
    }
    return 0;
}
