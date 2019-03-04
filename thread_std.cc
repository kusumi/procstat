#include <chrono>

#include "./thread_std.h"

std::mutex __mutex;

int init_lock(void)
{
	return 0;
}

int cleanup_lock(void)
{
	return 0;
}

int Mutex::wait(void)
{
	std::unique_lock<std::mutex> lk(_m, std::adopt_lock);
	_c.wait(lk);
	return 0;
}

int Mutex::timedwait(time_t s, long n)
{
	std::unique_lock<std::mutex> lk(_m, std::adopt_lock);
	_c.wait_for(lk, std::chrono::seconds(s) + std::chrono::nanoseconds(n));
	return 0;
}
