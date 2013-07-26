#ifndef __PANEL_H__
#define __PANEL_H__

#include <string>

#include "./screen.h"

class Panel {
public:
	Panel(int, int, int, int);
	virtual ~Panel(void) { delete_screen(_scr); }
	int get_ylen(void) { return _ylen; }
	int get_xlen(void) { return _xlen; }
	int get_ypos(void) { return _ypos; }
	int get_xpos(void) { return _xpos; }
	virtual void set_title(std::string) {}
	virtual void set_focus(bool) {}
	virtual void resize(int, int, int, int);
	void refresh(void) { refresh_screen(_scr); }
	void erase(void) { erase_screen(_scr); }
	void print(int, int, bool, std::string&);
protected:
	void _resize(int, int, int, int);
	Screen *_scr;
	int _ylen, _xlen, _ypos, _xpos;
};

class Frame: public Panel {
public:
	Frame(int, int, int, int);
	void set_title(std::string);
	void set_focus(bool);
	void resize(int, int, int, int);
private:
	void _print_title(void);
	std::string _title;
	bool _focus;
};
#endif
