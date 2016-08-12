#include <stdio.h>

#include "../logic.h"

struct Field *field = NULL;
struct StatsTable *stats_table;

struct ActionTable action_table;

int main(void)
{
  action_table.player_info[0].suicide = 0;
  action_table.player_info[0].bomb_pos.x = 0;
  action_table.player_info[0].bomb_pos.y = 0;
  action_table.player_info[0].move_pos.x = 1;
  action_table.player_info[0].move_pos.y = 1;

  SetUp(&field, &stats_table);
  Update(&action_table);

  action_table.player_info[0].move_pos.x = 1;
  action_table.player_info[0].move_pos.y = 1;

  action_table.player_info[0].bomb_pos.x = 1;
  action_table.player_info[0].bomb_pos.y = 1;
  Update(&action_table);

  return 0;
}
