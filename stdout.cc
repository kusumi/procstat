#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cassert>
#include <unistd.h>
#include <sys/ioctl.h>

#include "./screen.h"
#include "./thread.h"

namespace {
int LINES = -1;
int COLS = -1;
std::vector<char> vec;

void update_terminal_size(void)
{
	global_lock();
	struct winsize ws;
	if (ioctl(0, TIOCGWINSZ, &ws) == -1) {
		perror("ioctl");
		exit(1);
	}
	LINES = static_cast<int>(ws.ws_row);
	COLS = static_cast<int>(ws.ws_col);
	global_unlock();
}
}

short string_to_color(const char *arg)
{
	return -1;
}

int init_screen(short fg, short bg)
{
	update_terminal_size();
	return 0;
}

int cleanup_screen(void)
{
	for (size_t i = 0; i < vec.size(); i++)
		assert(vec[i] == 0);
	return 0;
}

int read_incoming(void)
{
	sleep(1);
	return KBD_ERR;
}

void clear_terminal(void)
{
}

int get_terminal_lines(void)
{
	assert(LINES != -1);
	return LINES;
}

int get_terminal_cols(void)
{
	assert(COLS != -1);
	return COLS;
}

void flash_terminal(void)
{
}

Screen *alloc_screen(int ylen, int xlen, int ypos, int xpos)
{
	global_lock();
	vec.push_back(1);
	Screen *ret = reinterpret_cast<Screen*>(vec.size());
	fprintf(stdout, "Allocate %p\n", ret);
	global_unlock();
	return ret;
}

void delete_screen(Screen *scr)
{
	global_lock();
	fprintf(stdout, "Delete %p\n", scr);
	size_t i = reinterpret_cast<size_t>(scr);
	vec[i - 1] = 0;
	global_unlock();
}

void print_screen(Screen *scr, int y, int x, bool standout, const char *fmt,
		...)
{
	global_lock();
	fprintf(stdout, "%p \"", scr);
	va_list ap;
	va_start(ap, fmt);
	fprintf(stdout, fmt, ap);
	va_end(ap);
	fprintf(stdout, "\"\n");
	global_unlock();
}

void refresh_screen(Screen *scr)
{
	global_lock();
	fflush(stdout);
	global_unlock();
}

void erase_screen(Screen *scr)
{
}

void resize_screen(Screen *scr, int ylen, int xlen)
{
	update_terminal_size();
}

void move_screen(Screen *scr, int ypos, int xpos)
{
}

void box_screen(Screen *scr)
{
}

void bkgd_screen(Screen *scr)
{
}
