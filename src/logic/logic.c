#include "logic.h"

struct BombInfo
{
  uint8_t d_time; /*  Detonate time. */
  struct Position pos;
};

/*============================*/
/*                            */
/*  Global variables.         */
/*                            */
static struct Field g_field;
static struct StatsTable g_table;
static struct BombInfo g_bomb_info[MAX_PLAYER_AMOUNT];
static struct Position g_player_pos[MAX_PLAYER_AMOUNT];
static uint8_t g_res_time[MAX_PLAYER_AMOUNT];  /* Respawn time. */

/*============================*/
/*                            */
/*  Interface definitions.    */
/*                            */
void Update(const struct ActionTable *action_table,
            struct Field **game_field, struct StatsTable **stats_table)
{
  RemoveSuicides(action_table, stats_table);
}

#define FIELD_SIZE (19)

void SetUp(struct Field **game_field,
           struct StatsTanle **stats_table)
{
  uint16_t i, j;
  enum Cell ** field;
  *game_field = 0;

  /* Code below instantiate 2-dimentional array
   * First dimention is an array of pointers
   * Second dimention is an linear blob of enum Cell */
  g_field.location = calloc(sizeof(void *) * FIELD_SIZE);
  if (g_field.location == 0) {
    perror("Field alloc(1) error");
    return;
  }

  g_field.location[0] = calloc(sizeof(enum Cell) * FIELD_SIZE * FIELD_SIZE);
  if (g_field.location == 0) {
    perror("Field alloc(2) error");
    return;
  }

  for (i = 1; i < FIELD_SIZE; ++i) {
    g_field.location[i] = g_field.location[0] + FIELD_SIZE * i;
  }

  field = g_field.location;
  for (i = 1; i < FIELD_SIZE; i += 2) {
    for (j = 1; j < FIELD_SIZE; j += 2) {
      field[i][j] = WALL;
    }
  }

  for (i = 0; i < FIELD_SIZE; i += 2) {
    for (j = 0; j < FIELD_SIZE; j += 2) {
      field[i][j] = BOX;
    }
  }

  /* Filling corners
   * FIXME : move to standalone function */
  field[0][0] = EMPTY;
  field[0][1] = EMPTY;
  field[1][0] = EMPTY;

  field[FIELD_SIZE - 1][0] = EMPTY;
  field[FIELD_SIZE - 1][1] = EMPTY;
  field[FIELD_SIZE - 2][0] = EMPTY;

  field[FIELD_SIZE - 1][FIELD_SIZE - 1] = EMPTY;
  field[FIELD_SIZE - 1][FIELD_SIZE - 2] = EMPTY;
  field[FIELD_SIZE - 2][FIELD_SIZE - 1] = EMPTY;

  field[0][FIELD_SIZE - 1] = EMPTY;
  field[0][FIELD_SIZE - 2] = EMPTY;
  field[1][FIELD_SIZE - 1] = EMPTY;

}