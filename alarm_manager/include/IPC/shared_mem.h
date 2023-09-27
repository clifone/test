#ifndef SHARED_MEM_H_
#define SHARED_MEM_H_

#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define SHARED_MEM_PATH "/alarm_manager"

void create_shmem(void);
void* get_shmem(void);
void destroy_shmem(void);

#endif // SHARED_MEM_H_
