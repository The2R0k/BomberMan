/**
  \file
  \brief Header file with struct Position defintion.
*/

#ifndef _POSITION_H_
#define _POSITION_H_

#include <stdint.h>

/**
  \brief Structure that represent position in game field. 
*/
struct Position {
  uint8_t x;
  uint8_t y;
} __attribute__((packed));

#endif /* _POSITION_H_ */
