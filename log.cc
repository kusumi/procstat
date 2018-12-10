#include "./log.h"

void init_log(const char *s)
{
	global_lock();
	openlog(s, LOG_PID, LOG_USER);
	global_unlock();
}

void cleanup_log(void)
{
	global_lock();
	closelog();
	global_unlock();
}
