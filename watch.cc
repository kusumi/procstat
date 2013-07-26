#include <vector>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <sys/inotify.h>

#include "./watch.h"

namespace {
	int fd = -1;
	std::vector<char> buf;

	void expand_buffer(int n) {
		buf.resize(buf.size() +
			n * (sizeof(struct inotify_event) + 256));
	}
}

int init_watch(bool nonblock) {
	fd = inotify_init();
	if (fd == -1)
		return -1;
	if (nonblock) {
		int ret = fcntl(fd, F_GETFL, 0);
		if (ret == -1 || fcntl(fd, F_SETFL, ret | O_NONBLOCK) == -1) {
			close(fd);
			return -1;
		}
	}
	return 0;
}

int cleanup_watch(void) {
	return close(fd);
}

int add_watch(const char *path) {
	int ret = inotify_add_watch(fd, path, IN_MODIFY);
	if (ret != -1)
		expand_buffer(1);
	return ret;
}

int delete_watch(int wd) {
	int ret = inotify_rm_watch(fd, wd);
	if (ret != -1)
		expand_buffer(-1);
	return ret;
}

int read_watch(watch_res &r) {
	char *p = &buf[0];
	int ret = read(fd, p, buf.size());
	if (ret == -1) {
		if (errno == EAGAIN)
			return 0;
		else
			return -1;
	}
	int i = 0;
	while (i < ret) {
		struct inotify_event *e =
			reinterpret_cast<struct inotify_event*>(p + i);
		if (e->mask & IN_MODIFY)
			r.insert(watch_res::value_type(e->wd, WATCH_MODIFY));
		i += sizeof(struct inotify_event) + e->len;
	}
	return ret;
}
