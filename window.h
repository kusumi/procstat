#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <string>

#include "./thread.h"
#include "./panel.h"
#include "./buffer.h"

class Window {
public:
	Window(int, int, int, int);
	~Window(void);
	bool is_dead();
	void resize(int, int, int, int);
	void attach_buffer(std::string);
	void update_buffer(void);
	void focus(bool);
	void goto_head(void);
	void goto_tail(void);
	void goto_current(int);
	void repaint(void);
	void signal(void);
	void timedwait(time_t, long);
	int create(void);
	int join(void) { return _thread.join(); }
private:
	Mutex _mutex;
	Thread _thread;
	Panel *_frame, *_panel;
	Buffer *_buffer;
	int _offset;
};
#endif
