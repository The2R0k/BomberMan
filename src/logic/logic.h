/*  Interface for logic-module.
 *  \file logic.h
 */

#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "../include/action_table.h"
#include "../include/stats_table.h"

void Handle(const struct ActionTable *action_table, struct StatsTable *stats_t,
            struct Field *game_field);

void RemoveSuicides();

void MakeStep();

void PlantBombs();

void Boom();

#endif
