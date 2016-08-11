#include "logic.h"

struct BombInfo {
  uint8_t         d_time; /*  Detonate time. */
  struct Position pos;
};

/*============================*/
/*                            */
/*  Global variables.         */
/*                            */
static struct Field       g_field;
static struct StatsTable  g_table;
static struct BombInfo    g_bomb_info[4];
static uint8_t            g_res_time[4]; /*  Respawn time. */

/*============================*/
/*                            */
/*  Interface definitions.    */
/*                            */
void Update(const struct ActionTable *action_table,
            struct Field **game_field, struct StatsTable **stats_table)
{
  RemoveSuicides(action_table, stats_table);

}
