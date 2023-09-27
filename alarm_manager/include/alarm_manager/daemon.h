#ifndef DAEMON_H_
#define DAEMON_H_

#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>

#include "IPC/shared_mem.h"
#include "IPC/semaphore.h"
#include "alarm_manager/system_condition/system_condition.h"
#include "alarm_manager/failure.h"

#define DELAY 1
#define STDIN_PATH "/tmp/alarm_manager_infile"
#define STDOUT_PATH "/tmp/alarm_manager_outfile"
#define STDERR_PATH "/tmp/alarm_manager_errfile"

void write_pid(void);
pid_t read_pid(void);

int daemonize(char* name, char* path);
int kill_daemon(void);

char check_running_daemon(void);
void create_daemon(void);

#endif // DAEMON_H_
