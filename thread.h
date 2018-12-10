#ifndef __THREAD_H__
#define __THREAD_H__

#ifdef USE_NATIVETHREAD
#include "./thread_native.h"
#else
#include "./thread_posix.h"
#endif
#endif
