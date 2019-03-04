#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <vector>
#include <csignal>
#include <ctime>

extern volatile sig_atomic_t interrupted;

// to use for readonly after getopt
extern std::vector<int> layout;
extern time_t sinterval;
extern long ninterval;
extern bool showlnum;
extern bool foldline;
extern bool blinkline;
extern bool rotatecol;
extern bool usedelay;
#endif
