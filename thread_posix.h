#ifndef __THREAD_POSIX_H__
#define __THREAD_POSIX_H__

#include <pthread.h>
#include <ctime>

#define USE_C_LINKAGE

#define get_thread_id() ((unsigned long)pthread_self())
#define global_lock() pthread_mutex_lock(&__mutex)
#define global_unlock() pthread_mutex_unlock(&__mutex)

typedef void *(*pfn)(void*);
extern pthread_mutex_t __mutex;

int init_lock(void);
int cleanup_lock(void);

class Mutex {
public:
	Mutex(void) {
		pthread_mutex_init(&_m, 0);
		pthread_cond_init(&_c, 0);
	}
	~Mutex(void) {
		pthread_cond_destroy(&_c);
		pthread_mutex_destroy(&_m);
	}
	int lock(void) { return pthread_mutex_lock(&_m); }
	int unlock(void) { return pthread_mutex_unlock(&_m); }
	int signal(void) { return pthread_cond_signal(&_c); }
	int wait(void) { return pthread_cond_wait(&_c, &_m); }
	int timedwait(time_t, long);
private:
	pthread_mutex_t _m;
	pthread_cond_t _c;
};

class Thread {
public:
	int create(pfn fn, void *arg) {
		return pthread_create(&_t, 0, fn, arg);
	}
	int join(void) {
		void *res;
		return pthread_join(_t, &res);
	}
private:
	pthread_t _t;
};
#endif
