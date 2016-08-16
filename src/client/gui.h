/**
  /file
  File contains interface to interact between client and graphics
*/

#ifndef _CLIENT_GUI_H_
#define _CLIENT_GUI_H_

void Run(void);

enum ThreadState {
	STABLE = 0,
	NEED_REFRESH = 1,
	NEED_EXIT = 2
} __attribute__((packed));

#endif /* _CLIENT_GUI_H_ */
