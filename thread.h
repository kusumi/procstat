#ifndef __THREAD_H__
#define __THREAD_H__

#ifdef USE_STDTHREAD
#include "./thread_std.h"
#else
#include "./thread_posix.h"
#endif
#endif
