#include <syslog.h>
#include "utils/program_log.h"

void my_log(char* buf, int fail)
{
    #ifdef MYLOG
    openlog("alarm_manager", LOG_PID, LOG_DAEMON);
    syslog(0, "### [%s] %s", fail == 0 ? "fail" : " ok ", buf);
    closelog();
    #endif
}
