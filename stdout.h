#ifndef __STDOUT_H__
#define __STDOUT_H__

typedef void		Screen;

#define KBD_CTRL(x)	(x & 0x1F)

#define KBD_ERR		0xDEAD
#define KBD_UP		(KBD_ERR + 1)
#define KBD_DOWN	(KBD_ERR + 2)
#define KBD_LEFT	(KBD_ERR + 3)
#define KBD_RIGHT	(KBD_ERR + 4)
#define KBD_RESIZE	(KBD_ERR + 5)
#endif
