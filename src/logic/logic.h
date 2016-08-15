/**
  \file
  \brief File contains interface for logic-module.
*/

#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "../include/action_table.h"
#include "../include/stats_table.h"

/** 
  \brief Generate new field and starting statistics.
  \param [out] game_field Game field that will be generated.
  \param [out] stats_table Statistics table that will be generated.
*/
void SetUp(struct Field **game_field,
           struct StatsTable **stats_table);

/**
  \brief Update the game field and statistics table.
  \param [in] action_table Table that contains players actions.
 
  Function updates game field and statistics table rely on
  players actions that contain in action table.
*/
void Update(struct ActionTable *action_table);

#endif /* _LOGIC_H_ */
