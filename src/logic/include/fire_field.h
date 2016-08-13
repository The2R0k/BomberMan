#ifndef _FIRE_FIELD_H_
#define _FIRE_FIELD_H_

#include <stdint.h>

struct FireCell {
  int8_t time;
  int8_t player_num;
} __attribute__((packed));

struct FireField {
  struct FireCell cell[FIELD_SIZE][FIELD_SIZE];
};

#endif /* _FIRE_FIELD_H_ */
