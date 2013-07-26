#include <string>
#include <cstdarg>

#include "./screen.h"
#include "./thread.h"

namespace {
	int color_attr = 0;
}

short string_to_color(const char *arg) {
	std::string s(arg);
	if (s == "black")
		return COLOR_BLACK;
	else if (s == "red")
		return COLOR_RED;
	else if (s == "green")
		return COLOR_GREEN;
	else if (s == "yellow")
		return COLOR_YELLOW;
	else if (s == "blue")
		return COLOR_BLUE;
	else if (s == "magenta")
		return COLOR_MAGENTA;
	else if (s == "cyan")
		return COLOR_CYAN;
	else if (s == "white")
		return COLOR_WHITE;
	return -1;
}

int init_screen(short fg, short bg) {
	if (!initscr())
		return -1;
	if (keypad(stdscr, TRUE) == ERR)
		return -2;
	noecho();
	cbreak();
	curs_set(0);
	wtimeout(stdscr, 500);
	clear_terminal();

	if (fg == -1 && bg == -1)
		return 0; // nothing to do
	if (fg == -1)
		fg = COLOR_BLACK;
	if (bg == -1)
		bg = COLOR_WHITE;

	if (has_colors() == FALSE)
		return -3;
	if (start_color() == ERR)
		return -4;
	if (init_pair(1, fg, bg) == ERR)
		return -5;
	color_attr = COLOR_PAIR(1);
	return 0;
}

int cleanup_screen(void) {
	curs_set(1);
	if (endwin() == ERR)
		return -1;
	return 0;
}

int read_incoming(void) {
	return wgetch(stdscr);
}

void clear_terminal(void) {
	global_lock();
	wclear(stdscr);
	wrefresh(stdscr);
	global_unlock();
}

int get_terminal_lines(void) {
	global_lock();
	int y = static_cast<int>(LINES);
	global_unlock();
	return y;
}

int get_terminal_cols(void) {
	global_lock();
	int x = static_cast<int>(COLS);
	global_unlock();
	return x;
}

void flash_terminal(void) {
	flash();
}

Screen *alloc_screen(int ylen, int xlen, int ypos, int xpos) {
	global_lock();
	Screen *scr = newwin(ylen, xlen, ypos, xpos);
	if (scr) {
		scrollok(scr, FALSE);
		idlok(scr, FALSE);
		keypad(scr, TRUE);
	}
	global_unlock();
	return scr;
}

void delete_screen(Screen *scr) {
	global_lock();
	delwin(scr);
	global_unlock();
}

void print_screen(Screen *scr, int y, int x, bool standout, const char *fmt,
		...) {
	int attr = standout ? A_STANDOUT : A_NORMAL;
	global_lock();
	wattron(scr, attr);
	va_list ap;
	va_start(ap, fmt);
	mvwprintw(scr, y, x, fmt, ap);
	va_end(ap);
	wattroff(scr, attr);
	global_unlock();
}

void refresh_screen(Screen *scr) {
	global_lock();
	wrefresh(scr);
	global_unlock();
}

void erase_screen(Screen *scr) {
	global_lock();
	werase(scr);
	global_unlock();
}

void resize_screen(Screen *scr, int ylen, int xlen) {
	global_lock();
	wresize(scr, ylen, xlen);
	global_unlock();
}

void move_screen(Screen *scr, int ypos, int xpos) {
	global_lock();
	mvwin(scr, ypos, xpos);
	global_unlock();
}

void box_screen(Screen *scr) {
	global_lock();
	wborder(scr, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE,
		ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
	global_unlock();
}

void bkgd_screen(Screen *scr) {
	global_lock();
	if (color_attr)
		wbkgd(scr, color_attr | ' ');
	global_unlock();
}
