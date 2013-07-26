CC	= g++
TARGET	= procstat
SRCS	= procstat.cc thread.cc container.cc window.cc panel.cc buffer.cc
CFLAGS	= -Wall
LIBS	= -lpthread

DEBUG	?= 0
ifeq ($(DEBUG), 1)
	SRCS	+= log.cc
	CFLAGS	+= -g -DDEBUG
else
	CFLAGS	+= -O2
endif

USE_STDOUT	?= 0
ifeq ($(USE_STDOUT), 1)
	SRCS	+= stdout.cc
	CFLAGS	+= -DUSE_STDOUT
else
	SRCS	+= curses.cc
	LIBS	+= -lncurses
endif

USE_INOTIFY	?= 1
ifeq ($(USE_INOTIFY), 1)
	SRCS	+= watch.cc
	CFLAGS	+= -DUSE_INOTIFY
endif

USE_GETTIMEOFDAY	?= 0
ifeq ($(USE_GETTIMEOFDAY), 1)
	CFLAGS	+= -DUSE_GETTIMEOFDAY
else
	LIBS	+= -lrt
endif
OBJS	= $(SRCS:.cc=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)
.cc.o:
	$(CC) $(CFLAGS) -c $<
clean:
	@if [ -f ./$(TARGET) ]; then \
		rm ./$(TARGET); \
	fi
	@rm *.o
