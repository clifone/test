#include <stdio.h>
#include <getopt.h>
#include <limits.h>

#include "alarm_manager/system_condition/system_condition.h"
#include "input_processing/input_processing.h"
#include "alarm_manager/daemon.h"
#include "alarm_manager/failure.h"
#include "alarm_manager/system_condition/usage_utils.h"
#include "utils/program_log.h"

int input_processing(int argc, char* argv[])
{
    my_log("input_processing: start", 1);
    int err = 0;
    char mask = 0;
    int longind = -1;  // Index of current argv in longopts
    int res = 0;  // Returned value from getopt_long
    int is_daemon_running = 0;

    // First argument settings
    const char shortopts_first[] = {SET_SHORT, PRINT_SHORT, PRINT_ALL_SHORT, LOG_SHORT, DEL_LOG_SHORT, KILL_SHORT, HELP_SHORT};
    const struct option longopts_fisrt[] = {
        {SET_LONG, no_argument, NULL, SET_SHORT},
        {PRINT_LONG, no_argument, NULL, PRINT_SHORT},
        {PRINT_ALL_LONG, no_argument, NULL, PRINT_ALL_SHORT},
        {LOG_LONG, no_argument, NULL, LOG_SHORT},
        {DEL_LOG_LONG, no_argument, NULL, DEL_LOG_SHORT},
        {KILL_LONG, no_argument, NULL, KILL_SHORT},
        {HELP_LONG, no_argument, NULL, HELP_SHORT},
        {NULL, 0, NULL, 0}
    };

    // Print argument settings
    const char shortopts_print[] = {RAM_SHORT, CPU_SHORT, SWAP_SHORT, DIR_PATH_SHORT, DIR_SIZE_SHORT};
    const struct option longopts_print[] = {
        {RAM_LONG, no_argument, NULL, RAM_SHORT},
        {CPU_LONG, no_argument, NULL, CPU_SHORT},
        {SWAP_LONG, no_argument, NULL, SWAP_SHORT},
        {DIR_PATH_LONG, no_argument, NULL, DIR_PATH_SHORT},
        {DIR_SIZE_LONG, no_argument, NULL, DIR_SIZE_SHORT},
        {NULL, 0, NULL, 0}
    };

    // Set argument settings
    const char shortopts_set[] = {
        RAM_SHORT, ':',
        CPU_SHORT, ':',
        SWAP_SHORT, ':',
        DIR_PATH_SHORT, ':',
        DIR_SIZE_SHORT, ':'
    };
    const struct option longopts_set[] = {
        {RAM_LONG, required_argument, NULL, RAM_SHORT},
        {CPU_LONG, required_argument, NULL, CPU_SHORT},
        {SWAP_LONG, required_argument, NULL, SWAP_SHORT},
        {DIR_PATH_LONG, required_argument, NULL, DIR_PATH_SHORT},
        {DIR_SIZE_LONG, required_argument, NULL, DIR_SIZE_SHORT},
        {NULL, 0, NULL, 0}
    };

    // daemon run: 1 / daemon stop: 0
    if (!freopen(STDIN_PATH, "a+", stdin))
    {
        my_log("input_processing: freopen stdin", 0);
        exit(EXIT_FAILURE);
    }
    is_daemon_running = check_running_daemon();

    if (argc == 1)
    {
        if (!is_daemon_running)
        {
            printf("creating daemon\n");
            create_daemon();
        }
        else
        {
            printf("daemon already run\n");
        }
    }
    else
    {
        system_condition* limits = (system_condition*)malloc(sizeof(system_condition));
        if (limits == NULL)
        {
            my_log("input_processing: malloc", 0);
            exit(EXIT_FAILURE);
        }
        
        // First argument processing
        res = getopt_long(argc, argv, shortopts_first, longopts_fisrt, &longind);
        switch (res)
        {
            // --print
            case PRINT_SHORT:

                if (!is_daemon_running)
                {
                    my_log("input_processing: can not print before creating daemon", 0);
                    printf("wrong input: can not print before creating daemon\n");
                    err = 1;
                }

                while (!err && (res = getopt_long(argc, argv, shortopts_print, longopts_print, &longind)) != -1)
                {
                    switch (res)
                    {
                        case RAM_SHORT:
                            mask |= RAM_MASK;
                            break;
                        case CPU_SHORT:
                            mask |= CPU_MASK;
                            break;
                        case SWAP_SHORT:
                            mask |= SWAP_MASK;
                            break;
                        case DIR_PATH_SHORT:
                            mask |= DIR_PATH_MASK;
                            break;
                        case DIR_SIZE_SHORT:
                            mask |= DIR_SIZE_MASK;
                            break;
                        default:
                            err = 1;
                            my_log("input_processing: invalid argument in print", 0);
                            printf("wrong input: invalid argument in print\n");
                            break;
                    }
                }

                if (!err)
                {
                    printf("print values:\n");
                    system_condition* sc = get_system_condition();
                    system_condition* system_limits = get_limits();
                    if (mask & RAM_MASK) printf("\tram %lu Kb / %lu Kb (limit: %lu Kb)\n",
                                                sc->ram_usage,
                                                get_ram_max(),
                                                system_limits->ram_usage);
                    if (mask & CPU_MASK) printf("\tcpu %lf / %lf (limit: %lf)\n",
                                                sc->cpu_usage,
                                                get_cpu_max(),
                                                system_limits->cpu_usage);
                    if (mask & SWAP_MASK) printf("\tswap %lu Kb / %lu Kb (limit: %lu Kb)\n",
                                                sc->swap_usage,
                                                get_swp_max(),
                                                system_limits->swap_usage);
                    if (mask & DIR_PATH_MASK) printf("\tdir_path %s\n", system_limits->dir_path);
                    if (mask & DIR_SIZE_MASK) printf("\tdir_size %lu Kb / %lu Kb\n", sc->dir_size, system_limits->dir_size);
                    free(sc);
                    free(system_limits);
                }
                break;
            
            // --print_all
            case PRINT_ALL_SHORT:

                if (!is_daemon_running)
                {
                    my_log("input_processing: can not print before creating daemon", 0);
                    printf("wrong input: can not print before creating daemon\n");
                    err = 1;
                }
                else
                {
                    printf("print all values:\n");
                    system_condition* sc = get_system_condition();
                    system_condition* system_limits = get_limits();
                    printf("\tram %lu Kb / %lu Kb (limit: %lu Kb)\n", sc->ram_usage, get_ram_max(), system_limits->ram_usage);
                    printf("\tcpu %lf / %lf (limit: %lf)\n", sc->cpu_usage, get_cpu_max(), system_limits->cpu_usage);
                    printf("\tswap %lu Kb / %lu Kb (limit: %lu Kb)\n", sc->swap_usage, get_swp_max(), system_limits->swap_usage);
                    printf("\tdir_path %s\n", system_limits->dir_path);
                    printf("\tdir_size %lu Kb (limit: %lu Kb)\n", sc->dir_size, system_limits->dir_size);
                    free(sc);
                    free(system_limits);
                }
                break;

            // --set
            case SET_SHORT:

                if (!is_daemon_running)
                {
                    my_log("input_processing: can not set before creating daemon", 0);
                    printf("input_processing: can not set before creating daemon\n");
                    err = 1;
                }

                printf("check set values:\n");

                while (!err && (res = getopt_long(argc, argv, shortopts_set, longopts_set, &longind)) != -1)
                {
                    switch (res)
                    {
                        case RAM_SHORT:
                            mask |= RAM_MASK;
                            int ram_check = get_kbytes(optarg, &limits->ram_usage, get_ram_max());
                            if (ram_check == -1)
                            {
                                my_log("input_processing: ram check failed", 0);
                                err = 1;
                            }
                            printf("\tram check: %s, set: %ld\n", ram_check ? "fail" : "ok", limits->ram_usage);
                            break;
                        case CPU_SHORT:
                            mask |= CPU_MASK;
                            int cpu_check = get_percent(optarg, &limits->cpu_usage);
                            if (cpu_check == -1)
                            {
                                my_log("input_processing: cpu check failed", 0);
                                err = 1;
                            }
                            printf("\tcpu check: %s, set: %lf\n", cpu_check ? "fail" : "ok", limits->cpu_usage);
                            break;
                        case SWAP_SHORT:
                            mask |= SWAP_MASK;
                            int swap_check = get_kbytes(optarg, &limits->swap_usage, get_swp_max());
                            if (swap_check == -1)
                            {
                                my_log("input_processing: swap check failed", 0);
                                err = 1;
                            }
                            printf("\tswap check: %s, set: %ld\n", swap_check ? "fail" : "ok", limits->swap_usage);
                            break;
                        case DIR_PATH_SHORT:
                            mask |= DIR_PATH_MASK;
                            int dir_path_check = get_dir(optarg, limits);
                            printf("\tset %s dir: %s\n", dir_path_check ? "default" : "passed", limits->dir_path);
                            break;
                        case DIR_SIZE_SHORT:
                            mask |= DIR_SIZE_MASK;
                            int dir_size_check = get_kbytes(optarg, &limits->dir_size, UINT_MAX);
                            if (dir_size_check == -1)
                            {
                                my_log("input_processing: dir_size check failed", 0);
                                err = 1;
                            }
                            printf("\tdir_size check: %s, set: %ld\n", dir_size_check ? "fail" : "ok", limits->dir_size);
                            break;
                        default:
                            err = 1;
                            my_log("input_processing: invalid argument in set", 0);
                            printf("wrong input: invalid argument in set\n");
                            break;
                    }
                }

                if (!err)
                {
                    set_limits(limits, mask);
                    printf("set ok\n");
                }
                break;

            // --log
            case LOG_SHORT:

                if (getopt_long(argc, argv, shortopts_first, longopts_fisrt, NULL) != -1)
                {
                    err = 1;
                    syslog("input_processing: invalid arguments in log", 0);
                    printf("wrong input: invalid arguments in log\n");
                }
                else
                {
                    printf("log:\n");
                    print_log();
                }
                break;

            // --del_log
            case DEL_LOG_SHORT:

                if (getopt_long(argc, argv, shortopts_first, longopts_fisrt, NULL) != -1)
                {
                    err = 1;
                    syslog("input_processing: invalid arguments in del_log", 0);
                    printf("wrong input: invalid arguments in del_log\n");
                }
                else
                {
                    clear_log();
                    printf("log clear\n");
                }
                break;

            // --kill
            case KILL_SHORT:

                if (getopt_long(argc, argv, shortopts_first, longopts_fisrt, NULL) != -1)
                {
                    err = 1;
                    syslog("input_processing: invalid arguments in kill", 0);
                    printf("wrong input: invalid arguments in kill\n");
                }
                else
                {
                    if (!freopen(STDIN_PATH, "a+", stdin))
                    {
                        syslog("input_processing: freopen in kill", 0);
                        exit(EXIT_FAILURE);
                    }
                    printf("kill daemon\n");
                    kill_daemon();
                }
                break;

            // --help
            case HELP_SHORT:
                if (getopt_long(argc, argv, shortopts_first, longopts_fisrt, NULL) != -1)
                {
                    err = 1;
                    syslog("input_processing: invalid arguments in help", 0);
                    printf("wrong input: invalid arguments in help\n");
                }
                else
                {
                    help();
                }
                break;

            // error
            default:
                err = 1;
                syslog("input_processing: invalid argument", 0);
                printf("wrong input: invalid argument\n");
                break;
        }

        free(limits);
    }

    return err;
}
