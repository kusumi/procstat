#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <csignal>
#include <cstring>
#include <getopt.h>

#include "./global.h"
#include "./thread.h"
#include "./log.h"
#include "./watch.h"
#include "./screen.h"
#include "./container.h"

extern char *optarg;
extern int optind;

volatile sig_atomic_t interrupted = 0;
std::vector<int> layout;
time_t sinterval = 0;
long ninterval = 0;
short fgcolor = -1; // default color
short bgcolor = -1; // default color
bool showlnum = false;
bool foldline = false;
bool blinkline = true;
bool rotatecol = false;
bool usedelay = false;

namespace {
std::string _layout;
bool _usemsec = false;
const char *_what = 0;

void sigint_handler(int n)
{
	interrupted = 1;
}

void atexit_handler(void)
{
	cleanup_screen();
	cleanup_watch();
	cleanup_log();
	cleanup_lock();
	if (_what)
		std::cerr << _what << std::endl;
}

void usage(const char *arg)
{
	std::cerr << "Usage: " << arg << " [options] /proc/..." << std::endl
		<< "Options:" << std::endl
		<< "\t-c <arg> - Set column layout."
		<< " e.g. \"-c 123\" to make 3 columns with 1,2,3 windows for each"
		<< std::endl
		<< "\t-t <arg> - Set refresh interval in second. Default is 1."
		<< " e.g. \"-t 5\" to refresh screen every 5 seconds"
		<< std::endl
		<< "\t-m - Take refresh interval as milli second."
		<< " e.g. \"-t 500 -m\" to refresh screen every 500 milli seconds"
		<< std::endl
		<< "\t-n - Show line number" << std::endl
		<< "\t-f - Fold lines when longer than window width"
		<< std::endl
		<< "\t-r - Rotate column layout" << std::endl
		<< "\t-h - This option" << std::endl
		<< "\t--fg <arg> - Set foreground color. Available colors are "
		<< "\"black\", \"blue\", \"cyan\", \"green\", \"magenta\", \"red\", \"white\", \"yellow\"."
		<< std::endl
		<< "\t--bg <arg> - Set background color. Available colors are "
		<< "\"black\", \"blue\", \"cyan\", \"green\", \"magenta\", \"red\", \"white\", \"yellow\"."
		<< std::endl
		<< "\t--noblink - Disable blink" << std::endl
		<< "\t--usedelay - Add random delay time before each window starts"
		<< std::endl
		<< std::endl
		<< "Commands:" << std::endl
		<< "\t0 - Set current position to the first line of the buffer"
		<< std::endl
		<< "\t$ - Set current position to the last line of the buffer"
		<< std::endl
		<< "\tk|UP - Scroll upward" << std::endl
		<< "\tj|DOWN - Scroll downward" << std::endl
		<< "\th|LEFT - Select next window" << std::endl
		<< "\tl|RIGHT - Select previous window" << std::endl
		<< "\tCTRL-b - Scroll one page upward" << std::endl
		<< "\tCTRL-u - Scroll half page upward" << std::endl
		<< "\tCTRL-f - Scroll one page downward" << std::endl
		<< "\tCTRL-d - Scroll half page downward" << std::endl
		<< "\tCTRL-l - Repaint whole screen" << std::endl;
}
}

int main(int argc, char *argv[])
{
	int i, c;
	struct option lo[] = {
		{"fg", 1, 0, 'F'},
		{"bg", 1, 0, 'B'},
		{"noblink", 0, 0, 'N'},
		{"usedelay", 0, 0, 'T'},
		{0, 0, 0, 0},
	};

	while ((c = getopt_long(argc, argv, "c:t:mnfrhu", lo, &i)) != -1) {
		switch (c) {
		case 'c':
			_layout = optarg;
			break;
		case 't':
			sinterval = atoi(optarg);
			break;
		case 'F':
			fgcolor = string_to_color(optarg);
			break;
		case 'B':
			bgcolor = string_to_color(optarg);
			break;
		case 'm':
			_usemsec = true;
			break;
		case 'n':
			showlnum = true;
			break;
		case 'f':
			foldline = true;
			break;
		case 'N':
			blinkline = false;
			break;
		case 'r':
			rotatecol = true;
			break;
		case 'T':
			usedelay = true;
			break;
		case 'h':
		case 'u':
			usage(argv[0]);
			exit(1);
		}
	}

	if (_usemsec) {
		unsigned int x = sinterval;
		sinterval = x / 1000;
		ninterval = (x % 1000) * 1000 * 1000;
	}

	if (!sinterval && !ninterval) {
		sinterval = 1;
		ninterval = 0;
	}

	if (_layout.empty()) {
		_layout = std::string(argc - optind, '1');
		if (_layout.empty())
			_layout = "1";
	}
	for (size_t i = 0; i < _layout.size(); i++) {
		int x = toupper(_layout[i]);
		if (x >= '0' && x <= '9')
			layout.push_back(x - '0');
		else if (x >= 'A' && x <= 'F')
			layout.push_back(x - 'A' + 10);
		else
			layout.push_back(-1);
	}

	init_lock();
	init_log(argv[0]);
	init_watch(true);

	int ret = init_screen(fgcolor, bgcolor);
	if (ret) {
		log("failed to init screen %d", ret);
		exit(1);
	}

	if (atexit(atexit_handler)) {
		perror("atexit");
		exit(1);
	}

	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = sigint_handler;
	if (sigemptyset(&sa.sa_mask) == -1) {
		perror("sigemptyset");
		exit(1);
	}
	if (sigaction(SIGINT, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	try {
		Container co(argc - optind, &argv[optind]);
		if (co.thread_create())
			exit(1);
		while (!interrupted)
			co.parse_event(read_incoming());
		co.thread_join();
	} catch (const std::exception &e) {
		_what = e.what();
		exit(1);
	}

	return 0;
}
