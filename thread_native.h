#ifndef __THREAD_NATIVE_H__
#define __THREAD_NATIVE_H__

#include <thread>
#include <mutex>
#include <condition_variable>
#include <ctime>

#define get_thread_id() std::hash<std::thread::id>()(std::this_thread::get_id())
#define global_lock() __mutex.lock()
#define global_unlock() __mutex.unlock()

typedef void *(*pfn)(void*);
extern std::mutex __mutex;

int init_lock(void);
int cleanup_lock(void);

class Mutex {
public:
	int lock(void) { _m.lock(); return 0; }
	int unlock(void) { _m.unlock(); return 0; }
	int signal(void) { _c.notify_one(); return 0; }
	int wait(void);
	int timedwait(time_t, long);
private:
	std::mutex _m;
	std::condition_variable _c;
};

class Thread {
public:
	int create(pfn fn, void *arg) {
		_t = std::thread(fn, arg);
		return 0;
	}
	int join(void) {
		_t.join();
		return 0;
	}
private:
	std::thread _t;
};
#endif
