#include <new>

#include "./panel.h"

// Panel
Panel::Panel(int ylen, int xlen, int ypos, int xpos):
	_scr(0),
	_ylen(ylen),
	_xlen(xlen),
	_ypos(ypos),
	_xpos(xpos)
{
	_scr = alloc_screen(_ylen, _xlen, _ypos, _xpos);
	if (!_scr)
		throw std::bad_alloc();
	bkgd_screen(_scr);
}

void Panel::resize(int ylen, int xlen, int ypos, int xpos)
{
	_resize(ylen, xlen, ypos, xpos);
	refresh();
}

void Panel::_resize(int ylen, int xlen, int ypos, int xpos)
{
	_ylen = ylen;
	_xlen = xlen;
	_ypos = ypos;
	_xpos = xpos;
	resize_screen(_scr, _ylen, _xlen);
	move_screen(_scr, _ypos, _xpos);
}

void Panel::print(int y, int x, bool standout, std::string &s)
{
	print_screen(_scr, y, x, standout, s.c_str());
}

// Frame
Frame::Frame(int ylen, int xlen, int ypos, int xpos):
	Panel(ylen, xlen, ypos, xpos),
	_title(""),
	_focus(false)
{
	box_screen(_scr);
}

void Frame::set_title(std::string s)
{
	_title = s;
	_print_title();
}

void Frame::set_focus(bool t)
{
	_focus = t;
	_print_title();
}

void Frame::resize(int ylen, int xlen, int ypos, int xpos)
{
	_resize(ylen, xlen, ypos, xpos);
	box_screen(_scr);
	_print_title();
}

void Frame::_print_title(void)
{
	print(0, 1, _focus, _title);
	refresh();
}
