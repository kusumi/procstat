#ifndef __WATCH_H__
#define __WATCH_H__

#define WATCH_MODIFY 1

#include <map>
typedef std::map<int, int> watch_res;

#ifndef NO_USE_INOTIFY
int init_watch(bool);
int cleanup_watch(void);
int add_watch(const char*);
int delete_watch(int);
int read_watch(watch_res&);
#else
namespace {
inline int init_watch(bool) { return 0; }
inline int cleanup_watch(void) { return 0; }
inline int add_watch(const char*) { return -1; }
inline int delete_watch(int) { return -1; }
inline int read_watch(watch_res&) { return -1; }
}
#endif
#endif
