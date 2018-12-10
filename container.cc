#include <map>
#include <stdexcept>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "./global.h"
#include "./container.h"
#include "./thread.h"
#include "./screen.h"
#include "./watch.h"
#include "./log.h"

namespace {
Mutex _mutex;
Thread _thread;
typedef std::map<int, Window*> _map_t;
_map_t _map;
}

Container::Container(int argc, char *argv[])
{
	_build_window();
	for (int i = 0; i < argc; i++) {
		char *p = argv[i];
		struct stat st;
		if (stat(p, &st) == -1 || !S_ISREG(st.st_mode))
			continue;
		if (i < static_cast<int>(_v.size())) {
			_v[i]->attach_buffer(p);
			int wd = add_watch(p);
			if (wd != -1)
				_map.insert(_map_t::value_type(wd, _v[i]));
		}
	}
	_it = _v.begin();
	(*_it)->focus(true);
}

Container::~Container(void)
{
	for (_map_t::iterator it = _map.begin(); it != _map.end(); it++)
		delete_watch(it->first);
	while (!_v.empty()) {
		Window *p = _v.back();
		_v.pop_back();
		delete p;
	}
}

void Container::_goto_next_window(void)
{
	std::vector<Window*>::iterator it = _it;
	(*_it)->focus(false);
	do {
		_it++;
		if (_it == _v.end())
			_it = _v.begin();
	} while ((*_it)->is_dead() && _it != it);
	(*_it)->focus(true);
}

void Container::_goto_prev_window(void)
{
	std::vector<Window*>::iterator it = _it;
	(*_it)->focus(false);
	do {
		if (_it == _v.begin())
			_it = _v.end();
		_it--;
	} while ((*_it)->is_dead() && _it != it);
	(*_it)->focus(true);
}

void Container::_build_window(void)
{
	if (!rotatecol)
		_build_window_xy();
	else
		_build_window_yx();
}

void Container::_build_window_xy(void)
{
	int seq = 0;
	int xx = get_terminal_cols();
	int yy = get_terminal_lines();
	int x = static_cast<int>(layout.size());
	int xq = xx / x;
	int xr = xx % x;

	for (int i = 0; i < x; i++) {
		int xpos = xq * i;
		int xlen = xq;
		if (i == x - 1)
			xlen += xr;
		int y = layout[i];
		if (y == -1)
			y = 1; // ignore invalid
		int yq = yy / y;
		int yr = yy % y;

		for (int j = 0; j < y; j++) {
			int ypos = yq * j;
			int ylen = yq;
			if (j == y - 1)
				ylen += yr;
			_alloc_window(seq++, ylen, xlen, ypos, xpos);
		}
	}
}

void Container::_build_window_yx(void)
{
	int seq = 0;
	int yy = get_terminal_lines();
	int xx = get_terminal_cols();
	int y = static_cast<int>(layout.size());
	int yq = yy / y;
	int yr = yy % y;

	for (int i = 0; i < y; i++) {
		int ypos = yq * i;
		int ylen = yq;
		if (i == y - 1)
			ylen += yr;
		int x = layout[i];
		if (x == -1)
			x = 1; // ignore invalid
		int xq = xx / x;
		int xr = xx % x;

		for (int j = 0; j < x; j++) {
			int xpos = xq * j;
			int xlen = xq;
			if (j == x - 1)
				xlen += xr;
			_alloc_window(seq++, ylen, xlen, ypos, xpos);
		}
	}
}

void Container::_alloc_window(int seq, int ylen, int xlen, int ypos, int xpos)
{
	try {
		Window *p = _v.at(seq);
		p->resize(ylen, xlen, ypos, xpos);
		p->signal();
	} catch (const std::out_of_range &e) {
		Window *p = new Window(ylen, xlen, ypos, xpos);
		_v.push_back(p);
	}
}

void Container::parse_event(int x)
{
	Window *p = *_it;
	switch (x) {
	case KBD_ERR:
		break;
	case KBD_RESIZE:
	case KBD_CTRL('l'):
		clear_terminal();
		_build_window();
		break;
	case 'h':
	case KBD_LEFT:
		_goto_prev_window();
		break;
	case 'l':
	case KBD_RIGHT:
		_goto_next_window();
		break;
	case '0':
		p->goto_head();
		goto _signal;
	case '$':
		p->goto_tail();
		goto _signal;
	case 'k':
	case KBD_UP:
		p->goto_current(-1);
		goto _signal;
	case 'j':
	case KBD_DOWN:
		p->goto_current(1);
		goto _signal;
	case KBD_CTRL('B'):
		p->goto_current(-get_terminal_lines());
		goto _signal;
	case KBD_CTRL('U'):
		p->goto_current(-get_terminal_lines() / 2);
		goto _signal;
	case KBD_CTRL('F'):
		p->goto_current(get_terminal_lines());
		goto _signal;
	case KBD_CTRL('D'):
		p->goto_current(get_terminal_lines() / 2);
		goto _signal;
	default:
		goto _signal;
	}
	return;
_signal:
	p->signal();
}

#ifdef USE_C_LINKAGE
extern "C" {
#endif
static void *watch_thread_handler(void *arg)
{
	log("thread=%lu", get_thread_id());
	while (!interrupted) {
		watch_res r;
		if (read_watch(r) > 0) {
			for (watch_res::iterator it = r.begin(); it != r.end();
				it++) {
				Window *p = _map[it->first];
				switch (it->second) {
				case WATCH_MODIFY:
					p->update_buffer();
					break;
				}
			}
			flash_terminal();
		}
		_mutex.timedwait(1, 0);
	}
	return 0;
}
#ifdef USE_C_LINKAGE
}
#endif

int Container::thread_create(void)
{
	int ret = _thread.create(watch_thread_handler, 0);
	if (ret) {
		log("create thread failed %d", ret);
		return ret;
	}

	for (size_t i = 0; i < _v.size(); i++) {
		ret = _v[i]->create();
		if (ret) {
			log("window=%p create thread failed %d",
				static_cast<void*>(_v[i]), ret);
			return ret;
		}
	}
	return 0;
}

void Container::thread_join(void)
{
	int ret;
	for (size_t i = 0; i < _v.size(); i++)
		_v[i]->signal();
	for (size_t i = 0; i < _v.size(); i++) {
		ret = _v[i]->join();
		if (ret)
			log("window=%p join thread failed %d",
				static_cast<void*>(_v[i]), ret);
	}

	_mutex.signal();
	ret = _thread.join();
	if (ret)
		log("join thread failed %d", ret);
}
