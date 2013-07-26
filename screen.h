#ifndef __SCREEN_H__
#define __SCREEN_H__

#ifdef USE_STDOUT
#include "./stdout.h"
#else
#include "./curses.h"
#endif

short string_to_color(const char*);
int init_screen(short, short);
int cleanup_screen(void);

int read_incoming(void);
void clear_terminal(void);
int get_terminal_lines(void);
int get_terminal_cols(void);
void flash_terminal(void);

Screen *alloc_screen(int, int, int, int);
void delete_screen(Screen*);
void print_screen(Screen*, int, int, bool, const char*, ...);
void refresh_screen(Screen*);
void erase_screen(Screen*);
void resize_screen(Screen*, int, int);
void move_screen(Screen*, int, int);
void box_screen(Screen*);
void bkgd_screen(Screen*);
#endif
