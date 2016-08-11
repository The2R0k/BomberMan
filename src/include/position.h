/*  Position structure.
 *  \file position.h
 */

#ifndef _POSITION_H_
#define _POSITION_H_

#include <stdint.h>

struct Position {
  uint8_t x;
  uint8_t y;
}__attribute__((__packed__ ));

#endif /* _POSITION_H_ */
