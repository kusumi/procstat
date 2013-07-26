procstat
========

## Usage

    [root@localhost procstat]# ./procstat -h
    Usage: ./procstat [options] /proc/...
    Options:
	    -c <arg> - Set column layout. e.g. -c 123 makes 3 columns with 1,2,3 windows for each
	    -t <arg> - Set refresh interval in second. Default is 1. e.g. -t 5 makes it refresh every 5 seconds
	    -m - Take refresh interval as milli second. e.g. -t 500 -m makes it refresh evety 500 milli seconds
	    -n - Show line number
	    -f - Fold line to the next when longer than the window width
	    -r - Rotate column layout
	    -h - This option
	    --fg <arg> - Set foreground color to [black|red|green|yellow|blue|magenta|cyan|white]
	    --bg <arg> - Set background color to [black|red|green|yellow|blue|magenta|cyan|white]
	    --noblink - Disable blink
	    --usedelay - Add random delay time before each window starts

    Commands:
	    0 - Set current position to the first line of the buffer
	    $ - Set current position to the last line of the buffer
	    k|UP - Scroll upward
	    j|DOWN - Scroll downward
	    h|LEFT - Select next window
	    l|RIGHT - Select previous window
	    CTRL-b - Scroll one page upward
	    CTRL-u - Scroll half page upward
	    CTRL-f - Scroll one page downward
	    CTRL-d - Scroll half page downward
	    CTRL-l - Repaint whole screen
