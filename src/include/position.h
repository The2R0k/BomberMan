/*  Position structure.
 *  \file position.h
 */

#ifndef _POSITION_H_
#define _POSITION_H_

#include <stdint.h>

struct Position {
  int8_t x;
  int8_t y;
}__attribute__((__packed__ ));

#endif /* _POSITION_H_ */
