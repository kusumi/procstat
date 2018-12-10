#include <unistd.h>
#ifdef USE_GETTIMEOFDAY
#include <sys/time.h>
#endif

#include "./thread_posix.h"

pthread_mutex_t __mutex;

int init_lock(void)
{
	return pthread_mutex_init(&__mutex, 0);
}

int cleanup_lock(void)
{
	return pthread_mutex_destroy(&__mutex);
}

int Mutex::timedwait(time_t s, long n)
{
	struct timespec ts;
#ifdef USE_GETTIMEOFDAY
	struct timeval tv;
	if (gettimeofday(&tv, 0) == -1) {
#else
	if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
#endif
		sleep(s);
		usleep(n / 1000);
		return -1;
	}
#ifdef USE_GETTIMEOFDAY
	ts.tv_sec = tv.tv_sec;
	ts.tv_nsec = tv.tv_usec * 1000;
#endif
	ts.tv_sec += s;
	ts.tv_nsec += n;
	if (ts.tv_nsec >= 1000000000) {
		ts.tv_sec++;
		ts.tv_nsec -= 1000000000;
	}
	return pthread_cond_timedwait(&_c, &_m, &ts);
}
