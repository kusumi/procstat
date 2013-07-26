#ifndef __CURSES_H__
#define __CURSES_H__

#include <ncurses.h>

#define Screen		WINDOW

#define KBD_CTRL(x)	(x & 0x1F)

#define KBD_ERR		ERR
#define KBD_UP		KEY_UP
#define KBD_DOWN	KEY_DOWN
#define KBD_LEFT	KEY_LEFT
#define KBD_RIGHT	KEY_RIGHT
#define KBD_RESIZE	KEY_RESIZE
#endif
