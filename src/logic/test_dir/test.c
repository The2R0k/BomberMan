#include <stdio.h>

#include "../logic.h"

struct Field *field = NULL;
struct StatsTable *stats_table;

struct ActionTable action_table;

void PrintTable(void)
{
  int i, j;
  char c;

  for (i = 0; i < FIELD_SIZE; ++i) {
    for (j = 0; j < FIELD_SIZE; ++j) {
      switch (field->location[i][j]) {
        case EMPTY:
          c = '0';
          break;
        case PLAYER_1:
        case PLAYER_2:
        case PLAYER_3:
        case PLAYER_4:
          c = '0' + EMPTY;
          break;
        case BOX:
          c = 'x';
          break;
        case BOMB:
          c = 'B';
          break;
        case WALL:
          c = 'W';
          break;
        case FIRE:
          c = '*';
          break;
        default:
          c = 'a' + field->location[i][j];
      } 
      printf("%c", c);
    }
    printf("\n");
  }
}

int main(void)
{
  action_table.player_info[0].suicide = 0;
  action_table.player_info[0].bomb_pos.x = 0;
  action_table.player_info[0].bomb_pos.y = 0;
  action_table.player_info[0].move_pos.x = 1;
  action_table.player_info[0].move_pos.y = 2;

  SetUp(&field, &stats_table);
  PrintTable();

  return 0;
}
