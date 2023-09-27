#include "alarm_manager/daemon.h"
#include "utils/program_log.h"

void create_daemon() 
{
    my_log("create_daemon: start", 1);

    destroy_semaphore(DEM_SEM_LOG);
    destroy_semaphore(DEM_SEM_SHM);
    destroy_semaphore(DEM_SEM_PID);

    create_shmem();
    set_default_limits();

    char* name = "alarm_manager";
    char* path = "/tmp";
    daemonize(name, path);
}

void write_pid() 
{
    my_log("write_pid: start", 1);

    sem_t* sem = get_semaphore(DEM_SEM_PID);
    sem_wait(sem);

    fprintf(stdin, "%d", getpid());
    fflush(stdin);

    sem_post(sem);
    sem_close(sem);
}

pid_t read_pid() 
{
    my_log("read_pid: start", 1);

    pid_t pid;
    char str[10];

    sem_t* sem = get_semaphore(DEM_SEM_PID);
    sem_wait(sem);

    fseek(stdin, 0, SEEK_SET);
    fgets(str, sizeof(str), stdin);

    sem_post(sem);
    sem_close(sem);

    pid = strtol(str, NULL, 10);
    return pid;
}

int daemonize(char* name, char* path)
{
    my_log("daemonize: start", 1);

    if (!path) { path = "/"; }
    if (!name) { name = "alarm_manager"; }

    pid_t pid = fork();
    if (pid < 0) { my_log("daemonize: fork 1", 0); exit(EXIT_FAILURE); }
    if (pid > 0) { exit(EXIT_SUCCESS); }
    if (setsid() < 0) { my_log("daemonize: setdid", 0); exit(EXIT_FAILURE); }

    signal(SIGCHLD,SIG_IGN);
    signal(SIGHUP,SIG_IGN);

    pid = fork();
    if (pid < 0) { my_log("daemonize: fork 2", 0); exit(EXIT_FAILURE); }
    if (pid > 0) { exit(EXIT_SUCCESS); }

    umask(0);
    chdir(path);
    setpgrp();

    if (!freopen(STDIN_PATH, "w+", stdin)) 
        my_log("daemon freopen stdin to infile (for pid)", 0);

    if (!freopen(STDOUT_PATH, "a+", stdout)) 
        my_log("daemon freopen stdout to outfile (for logs)", 0);

    if (!freopen(STDERR_PATH, "w+", stderr)) 
        my_log("daemon freopen strerr to errfile (for errors)", 0);

    write_pid();
    
    while(1)
    {
        sleep(DELAY);

        system_condition* cur_limits = get_limits();
        check_failure(cur_limits);
        free(cur_limits);
    }
}


int kill_daemon() 
{
    my_log("kill_daemon: start", 1);
    if (check_running_daemon())
    {
        destroy_shmem();

        pid_t pid = read_pid();
        kill(pid, SIGKILL);

        destroy_semaphore(DEM_SEM_LOG);
        destroy_semaphore(DEM_SEM_SHM);
        destroy_semaphore(DEM_SEM_PID);

        return 1;
    }
    return 0;
}

char check_running_daemon() 
{
    my_log("check_running_daemon: start", 1);
    pid_t pid = read_pid();

    if (pid && !kill(pid, 0))
    {
        my_log("check_running_daemon: daemon exists", 1);
        return 1;
    }

    my_log("check_running_daemon: daemon DO NOT exists", 1);
    return 0;
}
