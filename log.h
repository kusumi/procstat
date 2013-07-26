#ifndef __LOG_H__
#define __LOG_H__

#ifdef DEBUG
#include <syslog.h>

#include "./thread.h"

#define log(fmt, arg...)					\
do {								\
	global_lock();						\
	syslog(LOG_INFO, "<%s> " fmt, __FUNCTION__, ##arg);	\
	global_unlock();					\
} while (0)

void init_log(const char*);
void cleanup_log(void);
#else
#define log(fmt, arg...)
#define init_log(s)
#define cleanup_log()
#endif
#endif
