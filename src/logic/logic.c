#include "logic.h"

#include "../include/position.h"

#define PLAYER_ALIVE -1

#define FIRE_TIME 3
#define RESPAWN_TIME 11

#define VERTICAL 0
#define HORIZONTAL 1

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct BombInfo
{
  uint8_t d_time;  /* Detonate time. */
  struct Position pos;
};

struct FireField {
  int **location;
};

/*============================*/
/*                            */
/* Global variables.          */
/*                            */
static struct Field g_field;
static struct StatsTable g_table;
static struct BombInfo g_bomb_info[MAX_PLAYER_AMOUNT];
static struct Position g_player_pos[MAX_PLAYER_AMOUNT];
static uint8_t g_res_time[MAX_PLAYER_AMOUNT];  /* Respawn time. */
static struct Field g_fire_field;

/*============================*/
/*                            */
/* Helpfull functions.        */
/*                            */

/* Interface. */
static void RemoveSuicides(const struct ActionTable *action_table);

static int CanPlant(int player_num, struct Position bomb_pos);

static void PlantBomb(struct Position pos);

static void PlantBombs(const struct ActionTable *action_table);

static int CanMove(int player_num, struct Position next_pos);

static void MovePlayer(int player_num, struct Position next_pos);

static void MovePlayers(const struct ActionTable *action_table);

static void Boom(void);

static void RespawnPlayer(int player_num);

static void DecreaseRespawn(void);


static void KillPlayer(int victim_num, int killer_num,
                       struct Position murder_pos);

static void DecreaseBombTime(void);

static void DecreaseFireTime(void);


static void DetonateBomb(struct Position bomb_pos);

static void DetonateSide(uint8_t horizontal, int bomb_num);

/* Definitons. */
void RemoveSuicides(const struct ActionTable *action_table)
{
  int i;

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    if (action_table->player_info[i].suicide) {
      g_table.player_stats[i].score  = 0;
      g_table.player_stats[i].length = 0;
      g_table.player_stats[i].death  = 0;
      g_table.player_stats[i].bomb   = 0;

      g_player_pos[i].x = 0;
      g_player_pos[i].y = 0;

      g_bomb_info[i].d_time = -1;
      g_bomb_info[i].pos.x = 0;
      g_bomb_info[i].pos.y = 0;
    }
  }
}

int CanPlant(int player_num, struct Position bomb_pos)
{
  return !(g_bomb_info[player_num].pos.x == bomb_pos.x &&
           g_bomb_info[player_num].pos.y == bomb_pos.y);
}

void PlantBomb(struct Position pos)
{
  g_field.location[pos.y][pos.x] = BOMB;
}

void PlantBombs(const struct ActionTable *action_table)
{
  int i;

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    if (action_table->player_info[i].bomb_pos.x == 0) {
      continue;
    }
    if (CanPlant(i, action_table->player_info[i].bomb_pos)) {
      PlantBomb(action_table->player_info[i].bomb_pos);
    }
  }
}

int CanMove(int player_num, struct Position next_pos)
{
  return (g_field.location[next_pos.y][next_pos.x] == EMPTY ||
          g_field.location[next_pos.y][next_pos.x] == FIRE);
}

void MovePlayer(int player_num, struct Position next_pos)
{
  g_field.location[g_player_pos[player_num].y][g_player_pos[player_num].x] =
    EMPTY;
  g_field.location[next_pos.y][next_pos.x] = (enum Cell) player_num;
  
  g_player_pos[player_num] = next_pos;
}

void MovePlayers(const struct ActionTable *action_table)
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

void Boom(void)
{
  DecreaseBombTime();
  DecreaseFireTime();
}

void RespawnPlayer(int player_num)
{
  int i, j;
  do {
    i = rand() % FIELD_SIZE;
    j = rand() % FIELD_SIZE;
  } while (g_field.location[i][j] != EMPTY);

  g_field.location[i][j] = PLAYER_1 + player_num;
  g_res_time[player_num] = PLAYER_ALIVE;
  g_table.player_stats[player_num].bomb = 1;
  g_table.player_stats[player_num].length = 1;
}

void DecreaseRespawn(void)
{
  int i;

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    if (g_res_time[i] == PLAYER_ALIVE) {
      continue;
    }

    if (g_res_time[i] == 0) {
      RespawnPlayer(i);
    }

    if (g_res_time[i] != PLAYER_ALIVE) {
      --g_res_time[i];
    }
  }
}

void KillPlayer(int victim_num, int killer_num,
                struct Position murder_pos)
{
  /* Changing statistics. */
  if (victim_num == killer_num) {
    /* Suicide. */
    ++g_table.player_stats[victim_num].death;
  } else {
    ++g_table.player_stats[killer_num].score;
    if (g_table.player_stats[killer_num].length > FIELD_SIZE)
      ++g_table.player_stats[killer_num].length;

    ++g_table.player_stats[victim_num].death;
  }

  g_field.location[murder_pos.y][murder_pos.x] = FIRE;
  g_res_time[victim_num] = RESPAWN_TIME;
  g_player_pos[victim_num].x = 0;
  g_player_pos[victim_num].y = 0;
}

void DecreaseBombTime(void)
{
  int i;

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    if (g_bomb_info[i].d_time >= 0) {
      --g_bomb_info[i].d_time;
    }
    if (g_bomb_info[i].d_time == 0) {
      DetonateBomb(g_bomb_info[i].pos);
    }
  }
}

void DecreaseFireTime(void)
{
  int i, j;

  for (i = 0; i < FIELD_SIZE; ++i) {
    for (j = 0; j < FIELD_SIZE; ++j) {
      if (g_fire_field.location[i][j] > 0) {
        /* Decrease fire time. */
        --g_fire_field.location[i][j];
      }
      if (g_fire_field.location[i][j] == 0) {
        /* Remove fire from game field. */
        g_field.location[i][j] = EMPTY;
      }
    }
  }
}

void DetonateBomb(struct Position bomb_pos)
{
  int bomb_num = -1;
  int i;

  for (i = 0; i < MAX_PLAYER_AMOUNT && bomb_num == -1; ++i) {
    if (bomb_pos.x == g_bomb_info[i].pos.x &&
        bomb_pos.y == g_bomb_info[i].pos.y) {
      bomb_num = i;
    }
  }

  /*  */
  g_field.location[bomb_pos.y][bomb_pos.x] = FIRE;
  g_bomb_info[bomb_num].d_time = -1;
  g_bomb_info[bomb_num].pos.x = 0;
  g_bomb_info[bomb_num].pos.y = 0;
  
  DetonateSide(VERTICAL,   bomb_num);
  DetonateSide(HORIZONTAL, bomb_num);
} 

void DetonateSide(uint8_t horizontal, int bomb_num)
{
  uint8_t *current_coord,
          *end_coord;
  uint8_t bomb_radius;
  struct Position current_pos,
                  end_pos;
  enum Cell *current_cell;

  end_pos = current_pos = g_bomb_info[bomb_num].pos;
  bomb_radius = g_table.player_stats[bomb_num].length;

  current_coord = horizontal ? &current_pos.x : &current_pos.y; 
  *current_coord -= bomb_radius;
  if (*current_coord < 0)
    *current_coord = 0;
  
  end_coord = horizontal ? &end_pos.x : &end_pos.y;
  *end_coord += bomb_radius;
  if (*end_coord >= FIELD_SIZE)
    *end_coord = FIELD_SIZE;

  for (*current_coord = *current_coord; *current_coord < *end_coord;
       ++(*current_coord)) {
    current_cell = &g_field.location[current_pos.y][current_pos.x];
    switch (*current_cell) {
      case EMPTY:
        *current_cell = FIRE;
        g_fire_field.location[current_pos.y][current_pos.x] = FIRE_TIME;
        break;
      case FIRE:
        g_fire_field.location[current_pos.y][current_pos.x] = FIRE_TIME;
        break;
      case BOX:
        *current_cell = EMPTY;
        break;
      case WALL:
        break;
      case BOMB:
        DetonateBomb(current_pos);
        break;
      case PLAYER_1:
      case PLAYER_2:
      case PLAYER_3:
      case PLAYER_4:
        KillPlayer((int) *current_cell, bomb_num, current_pos);
        break;
      case PLAYER_1_BOMB:
      case PLAYER_2_BOMB:
      case PLAYER_3_BOMB:
      case PLAYER_4_BOMB:
        DetonateBomb(current_pos);
        KillPlayer((int) *current_cell, bomb_num, current_pos);
        break;
    }
  }
}

/*============================*/
/*                            */
/* Interface definitions.     */
/*                            */
void Update(const struct ActionTable *action_table)
{
  RemoveSuicides(action_table);
  PlantBombs(action_table);
  MovePlayers(action_table);
  Boom();
  DecreaseRespawn();
}

void FillField()
{
  enum Cell **field;
  int i, j;
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

void SetUp(struct Field **game_field,
           struct StatsTable **stats_table)
{
  uint16_t i;
  srand(0);
  *game_field = 0;
  *stats_table = 0;

  /* Code below instantiate 2-dimentional array
   * First dimention is an array of pointers
   * Second dimention is an linear buffer of enum Cell */
  g_field.location = malloc(sizeof(void *) * FIELD_SIZE);
  if (g_field.location == 0) {
    perror("Field alloc(1) error");
    return;
  }
  memset(g_field.location, 0, sizeof(void *) * FIELD_SIZE);

  g_field.location[0] = malloc(sizeof(enum Cell) * FIELD_SIZE * FIELD_SIZE);
  if (g_field.location == 0) {
    perror("Field alloc(2) error");
    return;
  }
  memset(g_field.location[0], 0, sizeof(enum Cell) * FIELD_SIZE * FIELD_SIZE);

  for (i = 1; i < FIELD_SIZE; ++i) {
    g_field.location[i] = g_field.location[0] + FIELD_SIZE * i;
  }

  FillField();

  *game_field = &g_field;

  /* Init stats table */
  memset(g_table.player_stats, 0, sizeof(struct Dklb) * MAX_PLAYER_AMOUNT);

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    g_table.player_stats[0].bomb = 1;
    g_table.player_stats[0].length = 1;
  }

  *stats_table = &g_table;

  memset(g_bomb_info, 0, sizeof(g_bomb_info));
  memset(g_player_pos, 0, sizeof(g_player_pos));
}

void TearDown(void)
{
  free(g_field.location[0]);
  free(g_field.location);
}
