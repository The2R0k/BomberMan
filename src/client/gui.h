/**
  /file
  File contains interface to interact between client and graphics
*/

#ifndef _CLIENT_GUI_H_
#define _CLIENT_GUI_H_

#include "network.h"

static int8_t IsAddressValid(char *ip_address);

static void PrintGreetings(void);

static void PrintMenu(void);

static void StartNewGame(void);

static int8_t Connect(char *ip_address);

static void TryConnect(void);

static void ShowStartingMenu(void);

#endif /* _CLIENT_GUI_H_ */
