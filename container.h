#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include <vector>

#include "./window.h"

class Container {
public:
	Container(int, char*[]);
	~Container(void);
	void parse_event(int);
	int thread_create(void);
	void thread_join(void);
private:
	void _goto_next_window(void);
	void _goto_prev_window(void);
	void _build_window(void);
	void _build_window_xy(void);
	void _build_window_yx(void);
	void _alloc_window(int, int, int, int, int);
	std::vector<Window*> _v;
	std::vector<Window*>::iterator _it;
};
#endif
