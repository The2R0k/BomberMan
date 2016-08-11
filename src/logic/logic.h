/*  Interface for logic-module.
 *  \file logic.h
 */

#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "../include/action_table.h"
#include "../include/stats_table.h"

/** \brief Generate new field and starting statistics.
 *  \param [out] game_field Game field that will be generated.
 *  \param [out] stats_table Statistics table that will be generated.
 */
void GenerateFieldAndStats(struct Field *game_field,
                           struct StatsTable* stats_table);

/** /brief Update the game filed and statistics.
 *  \param [in] action_table Table contains players actions.
 *  \param [out] game_file Game field.
 *  \param [out] stats_table Statistics table.
 *  
 *  Function updates game field and statistics table rely on
 *  players actions that contain in action table.
 */
void Update(const struct ActionTable *action_table,
                struct Field *game_field, struct StatsTable *stats_table);

#endif
