CC	= g++
TARGET	= procstat
SRCS	= main.cc container.cc window.cc panel.cc buffer.cc
CFLAGS	= -Wall
LIBS	=

USE_STDTHREAD	?= 0
ifeq ($(USE_STDTHREAD), 1)
	CFLAGS	+= -std=c++11 -DUSE_STDTHREAD
	SRCS	+= thread_std.cc
	#LIBS	+= -lpthread # may need this
else
	SRCS	+= thread_posix.cc
	LIBS	+= -lpthread
endif

PEDANTIC	?= 0
ifeq ($(PEDANTIC), 1)
	CFLAGS	+= -pedantic
endif

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
else
	CFLAGS	+= -DNO_USE_INOTIFY
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
