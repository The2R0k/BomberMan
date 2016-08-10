/*  Stats table structure. 
 *  \file stats_table.h
 */

#ifndef _STATS_TABLE_H_
#define _STATS_TABLE_H_

#include "common.h"

struct StatTable {
  struct Dklb player_stats[MAX_PLAYER_AMOUNT];
};

#endif /* _STATS_TABLE_H_ */
