#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <sys/sem.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#define DEM_SEM_LOG 10
#define DEM_SEM_SHM 20
#define DEM_SEM_PID 30
#define DEM_SEM_BEG_VAL 1

sem_t* get_semaphore(int key);
int destroy_semaphore(int key);

#endif // SEMAPHORE_H_
