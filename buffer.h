#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <string>
#include <vector>
#include <fstream>

#include "./thread.h"

class Buffer {
public:
	Buffer(std::string);
	~Buffer(void) { _ifs.close(); }
	std::string get_path(void) { return _path; }
	int get_max_line(void);
	bool is_dead(void);
	void update(void);
	int readline(int&, std::string&, bool&);
	void clear(void);
	void block_till_ready(void);
	void signal_blocked(void);
private:
	Mutex _mutex;
	std::vector<std::string> _chunk;
	std::ifstream _ifs;
	std::string _path;
	int _curline;
	int _maxline;
	bool _busy;
};
#endif
