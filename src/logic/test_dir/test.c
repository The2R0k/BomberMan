#include <stdio.h>

#include "../logic.h"

static struct Field *field = NULL;
static struct StatsTable *stats_table;

static struct ActionTable action_table;

static void Initialize(void) {
  int i;

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    action_table.player_info[i].suicide = 1;
  }
}

static void MovePlayer(int player_num, struct Position pos) {
  action_table.player_info[player_num].move_pos = pos;
}

static void MakeBomb(int player_num) {
  action_table.player_info[player_num].bomb = 1;
}

static void MakeMoveAndBomb(int player_num, struct Position move_pos,
                     struct Position bomb_pos) {
  action_table.player_info[player_num].move_pos = move_pos;
  action_table.player_info[player_num].bomb = 1;
}

static void MakeNull(int player_num) {
  action_table.player_info[player_num].move_pos.x = 0;
  action_table.player_info[player_num].move_pos.y = 0;
  action_table.player_info[player_num].bomb = 0;
}

static void Step(int step) {
  int i;

  for (i = 0; i < step; ++i) {
    Update(&action_table);
  }
}

static void MakePlayer2(void) {
  action_table.player_info[1].suicide = 0;
}

int main(void) {
  struct Position pos,
                  pos2;

  Initialize();
  SetUp(&field, &stats_table);
  action_table.player_info[0].suicide = 0;
  action_table.player_info[1].suicide = 0;
  MakeNull(0);
  MakeNull(1);
  
  /* Moves. */
  pos.x = 2;
  pos.y = 1;
  MovePlayer(0, pos);
  pos.x = 1;
  pos.y = 1;
  MovePlayer(1, pos);
  Step(1);

  /* 1 Makes bomb. */
  pos.x = 2;
  pos.y = 1;
  MakeBomb(0);

  /* 2 Makes bomb. */
  pos.x = 1;
  pos.y = 1;
  MakeBomb(1);
  Step(1);
 
  /* 2 moves and bomb 2 booms. */
  pos.x = 1;
  pos.y = 2;
  MovePlayer(1, pos);
  Step(2);
  
  PrintStatsTable();
  
  /*
  MakeNull(0);
  MakeNull(1);


  Step(3);
  
  PrintStatsTable();
  MakeNull(1);
*/

 /* 
  MakeBomb(0, pos);
  Step(1);

  pos.x = 1;
  pos.y = 1;
  MovePlayer(0, pos);
  Step(1);

  pos.x = 1;
  pos.y = 2;
  MovePlayer(0, pos);
  Step(1);
  PrintStatsTable();
  PrintRespawnTime();

  pos.x = 1;
  pos.y = 1;
  MovePlayer(0, pos);
  Step(1);
  PrintStatsTable();
  PrintRespawnTime();

  Step(2);
*/
  return 0;
}
