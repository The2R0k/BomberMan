#include <stdio.h>

#include "../logic.h"

struct Field *field = NULL;
struct StatsTable *stats_table;

struct ActionTable action_table;

void Initialize(void) {
  int i;

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    action_table.player_info[i].suicide = 1;
  }
  action_table.player_info[0].suicide = 0;
}

void MovePlayer(int player_num, struct Position pos)
{
  action_table.player_info[player_num].move_pos = pos;
}

void MakeBomb(int player_num, struct Position pos)
{
  action_table.player_info[player_num].bomb_pos = pos;
}

void MakeMoveAndBomb(int player_num, struct Position move_pos,
                     struct Position bomb_pos)
{
  action_table.player_info[player_num].move_pos = move_pos;
  action_table.player_info[player_num].bomb_pos = bomb_pos;
}

void MakeNull(int player_num)
{
  action_table.player_info[player_num].move_pos.x = 0;
  action_table.player_info[player_num].move_pos.y = 0;
  action_table.player_info[player_num].bomb_pos.x = 0;
  action_table.player_info[player_num].bomb_pos.y = 0;
}

void Step(int step)
{
  int i;

  for (i = 0; i < step; ++i) {
    Update(&action_table);
  }
}

int main(void)
{
  struct Position pos,
                  pos2;

  Initialize();
  SetUp(&field, &stats_table);
  action_table.player_info[0].suicide = 0;
  MakeNull(0);
  pos.x = 1;
  pos.y = 2;
  MovePlayer(0, pos);
  Step(1);

  MakeBomb(0, pos);
  Step(1);
  pos2 = pos;
  pos2.y = 1;
  MakeMoveAndBomb(0, pos2, pos);
  Step(2);
  MakeNull(0);
  Step(2);
  Step(2);
  PrintStatsTable();
  return 0;
}
