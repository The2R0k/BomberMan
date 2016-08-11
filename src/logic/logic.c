#include "logic.h"

struct BombInfo {
  uint8_t         d_time;  /* Detonate time. */
  struct Position pos;
};

/*============================*/
/*                            */
/* Global variables.          */
/*                            */
static struct Field       g_field;
static struct StatsTable  g_table;
static struct BombInfo    g_bomb_info[MAX_PLAYER_AMOUNT];
static uint8_t            g_res_time[MAX_PLAYER_AMOUNT];  /* Respawn time. */
static struct Position    g_player_pos[MAX_PLAYER_AMOUNT];

/*============================*/
/*                            */
/* Helpfull functions.        */
/*                            */

/* Interface. */
static void RemoveSuicides(const struct ActionTable *action_table,
                           struct StatsTable **stats_table);

static int CanPlant(int player_num, struct Postion bomb_pos);

static void PlantBomb(struct Position pos);

static void PlantBombs(struct ActionTable *action_table);

static int CanMove(int player_num, struct Position next_pos);

static void MovePlayer(int player_num, struct Position next_pos);

static void MovePlayers(const struct ActionTable *action_table);

static void Boom(void);

static void DecreaseRespawn(void);

/* Definitons. */
static void RemoveSuicides(const struct ActionTable *action_table,
                           struct StatsTable **stats_table)
{
  int i;

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    if (action_table->player_info[i].suicide) {
      stats_table->player_stats[i].score  = 0;
      stats_table->player_stats[i].length = 0;
      stats_table->player_stats[i].death  = 0;
      stats_table->player_stats[i].bomb   = 0;
    }
  }
}

static int CanPlant(int player_num, struct Postion bomb_pos)
{
  return !(g_bomb_info[player_num].pos.x == bomb_pos.x &&
           g_bomb_info[player_num].pos.y == bomb_pos.y );
}

static void PlantBomb(struct Position pos)
{
  g_field[pos.y][pos.x] = BOMB;
}

static void PlantBombs(struct ActionTable *action_table)
{
  int i;

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    if (action_table->player_info[i].bomb_pos.x == 0) {
      continue
    }
    if (CanPlant(i, action_table->player_int[i].bomb_pos)) {
      PlantBomb(action_table->player_info[i].bomb_pos);
    }
  }
}

static void MovePlayer(int player_num, struct Position next_pos)
{
  
}

static void MovePlayers(const struct ActionTable *action_table)
{
  int i;

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    if (action_table->player_info[i].move_pos.x == 0) {
      continue;
    }

    if (CanMove(i, action_table->player_info[i].move_pos)) {
      MovePlayer(i, action_table->player_info[i].move_pos);
    }
  }
}

static void Boom(void)
{

}

static void DecreaseRespawn(void)
{

}

/*============================*/
/*                            */
/* Interface definitions.     */
/*                            */
void Update(const struct ActionTable *action_table,
            struct Field **game_field, struct StatsTable **stats_table)
{
  *game_field = &g_field;
  RemoveSuicides(action_table, stats_table);
  PlantsBombs(action_table);
  MovePlayers(action_table);
  Boom();
  DecreaseRespawn();
}
