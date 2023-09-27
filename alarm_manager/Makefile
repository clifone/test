files_c=\
	src/alarm_manager/failure/log.c \
	src/alarm_manager/system_condition/cpu_usage.c \
	src/alarm_manager/system_condition/dir_usage.c \
	src/alarm_manager/system_condition/mem_usage.c \
	src/alarm_manager/system_condition/system_condition.c \
	src/alarm_manager/system_condition/usage_utils.c \
	src/alarm_manager/daemon.c \
	src/alarm_manager/failure/failure.c \
	src/input_processing/input_lib.c \
	src/input_processing/input_processing.c \
	src/IPC/semaphore.c \
	src/IPC/shared_mem.c \
	src/utils/program_log.c \
	src/utils/tokenize.c

flags_no_gtk=\
	-Iinclude -lm -lrt -Wall -Wextra

flags_gtk=\
	-Iinclude -lm -lrt -Wall -Wextra \
	`pkg-config --cflags gtk+-3.0` \
	`pkg-config --libs gtk+-3.0` \
	-DPOPUP

no_gtk: main.c $(files_c)
	gcc $^ -o alarm_manager $(flags_no_gtk)

gtk: main.c src/alarm_manager/failure/popup.c $(files_c)
	gcc $^ -o alarm_manager $(flags_gtk)

no_gtk_log: main.c $(files_c)
	gcc $^ -o alarm_manager $(flags_no_gtk) -DMYLOG

gtk_log: main.c src/alarm_manager/failure/popup.c $(files_c)
	gcc $^ -o alarm_manager $(flags_gtk) -DMYLOG

rerun: clean alarm_manager

clean:
	rm -f alarm_manager
