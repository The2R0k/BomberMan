/**
  /file
  File contains interface to interact between client and graphics
*/

#ifndef _CLIENT_GUI_H_
#define _CLIENT_GUI_H_

#include <stdint.h>

int8_t IsAddressValid(char *ip_address);

void PrintGreetings(void);

void PrintMenu(void);

void StartNewGame(void);

int8_t Connect(char *ip_address);

void TryConnect(void);

void ShowStartingMenu(void);

#endif /* _CLIENT_GUI_H_ */
