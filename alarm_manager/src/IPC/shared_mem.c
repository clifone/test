#include "IPC/shared_mem.h"
#include "IPC/semaphore.h"
#include "alarm_manager/system_condition/system_condition.h"
#include "utils/program_log.h"

void create_shmem()
{
    my_log("create_shmem: start", 1);

    int shmem = shm_open(SHARED_MEM_PATH, O_CREAT | O_RDWR, 0666);
    if (shmem == -1)
    {
        my_log("create_shmem: Shared memory open", 0);
        exit(EXIT_FAILURE);
    }

    sem_t* sem = get_semaphore(DEM_SEM_SHM);
    sem_wait(sem);

    if (ftruncate(shmem, sizeof(system_condition)) == -1)
    {
        my_log("create_shmem: Shared memory truncate", 0);
        exit(EXIT_FAILURE);
    }

    sem_post(sem);
    sem_close(sem);
}

void* get_shmem()
{
    my_log("get_shmem: start", 1);
    
    int shmem = shm_open(SHARED_MEM_PATH, O_CREAT | O_RDWR, 0666);
    if (shmem == -1)
    {
        my_log("get_shmem: Shared memory open", 0);
        exit(EXIT_FAILURE);
    }
    void* ptr = mmap(0, sizeof(system_condition), PROT_READ | PROT_WRITE, MAP_SHARED, shmem, 0);
    close(shmem);
    return ptr;
}

void destroy_shmem()
{
    my_log("destroy_shmem: start", 1);
    shm_unlink(SHARED_MEM_PATH);
}
