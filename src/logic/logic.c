/**
  \file
  \brief File contains implementation logic-module interface.
*/

#include "logic.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "include/fire_field.h"
#include "include/player_info.h"

#define NO_FIRE -1
#define FIRE_TIME 4
#define DETONATION_TIME 3
#define DISCONNECT_TIME 6

#define RESPAWN_TIME 11

#define VERTICAL 0
#define HORIZONTAL 1

#define MIN_COORD 1
#define MAX_COORD 18

#define MAX_LENGTH 8

/*============================*/
/*                            */
/* Global variables.          */
/*                            */
static struct Field g_field;
static struct StatsTable g_table;
static struct FireField g_fire_field;
static struct PlayerInfo g_player_info[MAX_PLAYER_AMOUNT];

/*============================*/
/*                            */
/* Helpfull functions.        */
/*                            */

/* Interface. */
/**
  \brief Add new player in game.
  \param [in] action_table Table is sent from server module.
*/
static void AddPlayers(const struct ActionTable *action_table);

/**
  \brief Remove all players that has been disconected.
  \param [in] action_table Table is sent from server module.
*/
static void SuicidePhase(const struct ActionTable *action_table);

/**
  \brief Plant bombs.
  \param [in] action_table Table is sent from server module.
*/
static void PlantingPhase(struct ActionTable *action_table);

/**
  \brief Make all players' movements.
  \param [in] action_table Table is sent from server module.
*/
static void MovingPhase(const struct ActionTable *action_table);

/**
  \brief Detoante all bombs.
*/
static void BoomPhase(void);

/**
  \brief Decrease fire time. 
*/
static void FirePhase(void);

/**
  \brief Respawn players that need it.
*/
static void RespawnPhase(void);



static void ConvertActionTable(struct ActionTable *action_table);

static void FillFireCell(struct Position pos, int player_num);

static void DisconnectPlayer(int player_num);

static int CanPlant(int player_num, struct Position bomb_pos);

static void PlantBomb(uint8_t player_num, struct Position pos);

static int CanMove(int player_num, struct Position next_pos);

static void MovePlayer(int player_num, struct Position next_pos);

static void RespawnPlayer(int player_num);

static void DecreaseResTime(int player_num);


static void KillPlayer(int victim_num, int killer_num,
                       struct Position murder_pos);


static void DetonateBomb(struct Position bomb_pos);

static void DetonateSide(uint8_t horizontal, int bomb_num);

static void FillField(void);

static void InitializeFireField(void);

static void InitializePlayersInfo(void);

/* Definitons. */
void ConvertActionTable(struct ActionTable *action_table) {
  struct Position pos;
  int i;

  /* Converting move positins.*/
  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    if (action_table->player_info[i].suicide) {
      continue;
    }

    if (action_table->player_info[i].move_pos.x == 0 &&
        action_table->player_info[i].move_pos.y == 0) {
      continue;
    }

    pos = g_player_info[i].pos;
    action_table->player_info[i].move_pos.x += pos.x;
    action_table->player_info[i].move_pos.y += pos.y;
  }
}

void AddPlayers(const struct ActionTable *action_table) {
  int i;

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    if (!action_table->player_info[i].suicide &&
        g_player_info[i].state == INACTIVE) {
      RespawnPlayer(i);
    }
  }
}

void SuicidePhase(const struct ActionTable *action_table) {
  int i;

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    if (action_table->player_info[i].suicide &&
        g_player_info[i].state != INACTIVE) {
      ++g_player_info[i].suicide_time;
      if (g_player_info[i].suicide_time == DISCONNECT_TIME) {
        DisconnectPlayer(i);
      }
    } else {
      g_player_info[i].suicide_time = 0;
    }
  }
}

int CanPlant(int player_num, struct Position bomb_pos) { 
  uint8_t has_bomb = g_table.player_stats[player_num].bomb;
  uint8_t is_player_on_bomb = 
        g_player_info[player_num].bomb_info.pos.x == bomb_pos.x &&
        g_player_info[player_num].bomb_info.pos.y == bomb_pos.y;
  
  return (!is_player_on_bomb && has_bomb);
}

void PlantBomb(uint8_t player_num, struct Position pos) {
  g_field.location[pos.y][pos.x] = PLAYER_1_BOMB + player_num;
  
  g_player_info[player_num].bomb_info.pos = pos;
  g_player_info[player_num].bomb_info.d_time = DETONATION_TIME;
  
  g_table.player_stats[player_num].bomb = 0;
}

void PlantingPhase(struct ActionTable *action_table) {
  int i;
  struct Position bomb_pos;

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    if (g_player_info[i].state != ALIVE) {
      continue;
    }
    
    if (CanPlant(i, action_table->player_info[i].bomb_pos)) {
      PlantBomb(i, action_table->player_info[i].bomb_pos);
      action_table->player_info[i].move_pos.x = 0;
      action_table->player_info[i].move_pos.y = 0;
    }
  }
}

void FillFireCell(struct Position pos, int player_num) {
  g_fire_field.cell[pos.y][pos.x].time = FIRE_TIME;
  g_fire_field.cell[pos.y][pos.x].player_num = player_num;
}

void DisconnectPlayer(int player_num) {
  struct Position *pos = &g_player_info[player_num].pos;
        
  g_field.location[pos->y][pos->x] = EMPTY;
  
  g_player_info[player_num].state = INACTIVE;
  g_player_info[player_num].pos.x = 0;
  g_player_info[player_num].pos.y = 0;
  g_player_info[player_num].suicide_time = 0;
  g_player_info[player_num].bomb_info.d_time = -1;
  g_player_info[player_num].bomb_info.pos.x = 0;
  g_player_info[player_num].bomb_info.pos.y = 0;
  g_player_info[player_num].res_time = 0;
  
  g_table.player_stats[player_num].score = 0;
  g_table.player_stats[player_num].length = 0;
  g_table.player_stats[player_num].death = 0;
  g_table.player_stats[player_num].bomb = 0; 
}

int CanMove(int player_num, struct Position next_pos) {
  int is_inside = next_pos.x > 0 &&
                  next_pos.y > 0 &&
                  next_pos.x < (FIELD_SIZE - 1) &&
                  next_pos.y < (FIELD_SIZE - 1);

  int is_valid = (g_field.location[next_pos.y][next_pos.x] == EMPTY ||
                  g_field.location[next_pos.y][next_pos.x] == FIRE);

  return is_inside && is_valid;
}

void MovePlayer(int player_num, struct Position next_pos) {
  struct Position *prev_pos = &g_player_info[player_num].pos;
  enum Cell *player_cell = &g_field.location[prev_pos->y][prev_pos->x];
  
  if (prev_pos->x != 0) {
    /* Player is not on starting pos. */
    if (*player_cell == (PLAYER_1_BOMB + player_num)) {
      /* Player is on bomb. */
      *player_cell = BOMB;
    } else {
      *player_cell = EMPTY;
    }
  }
  if (g_fire_field.cell[next_pos.y][next_pos.x].time == NO_FIRE) {
    g_field.location[next_pos.y][next_pos.x] = (enum Cell)(PLAYER_1 +
                                                           player_num);
    *prev_pos = next_pos;
  }
  else {
    KillPlayer(player_num,
               g_fire_field.cell[next_pos.y][next_pos.x].player_num,
               next_pos);
  }
}

void MovingPhase(const struct ActionTable *action_table) {
  int i;

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    if (g_player_info[i].state != ALIVE) {
      continue;
    }  

    if (CanMove(i, action_table->player_info[i].move_pos)) {
      MovePlayer(i, action_table->player_info[i].move_pos);  
    }
  }
}

void BoomPhase(void) {
  int i;

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    if (g_player_info[i].bomb_info.d_time > 0) {
      --g_player_info[i].bomb_info.d_time;
      if (g_player_info[i].bomb_info.d_time == 0) {
        DetonateBomb(g_player_info[i].bomb_info.pos);
      }
    }
  }
}

void RespawnPlayer(int player_num) {
  int i, j;
  
  if (g_player_info[player_num].res_time != 0)
    return;

  i = (player_num < 2) ? MIN_COORD : MAX_COORD - 1;
  j = (player_num % 2) ? MAX_COORD - 1 : MIN_COORD;

  g_field.location[i][j] = PLAYER_1 + player_num;
  g_player_info[player_num].state = ALIVE;
  g_player_info[player_num].pos.x = i;
  g_player_info[player_num].pos.y = j;
  g_player_info[player_num].suicide_time = 0;
  g_table.player_stats[player_num].bomb = 1;
  g_table.player_stats[player_num].length = 1;
}

void FirePhase(void) {
  int i, j;

  for (i = 0; i < FIELD_SIZE; ++i) {
    for (j = 0; j < FIELD_SIZE; ++j) {
      if (g_fire_field.cell[i][j].time > 0) {
        /* Decrease fire time. */
        --g_fire_field.cell[i][j].time;
        if (g_fire_field.cell[i][j].time == 0) {
          /* Remove fire from game field. */
          g_field.location[i][j] = EMPTY;
          g_fire_field.cell[i][j].time = NO_FIRE;
        }
      } 
    }
  }
}

void RespawnPhase(void) {
  int i;

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    if (g_player_info[i].state == INACTIVE ||
        g_player_info[i].state == ALIVE) {
      continue;
    }

    DecreaseResTime(i);
    RespawnPlayer(i);
  }
}

void DecreaseResTime(int player_num) {
  if (g_player_info[player_num].res_time != 0) {
    --g_player_info[player_num].res_time;
  }
}

void KillPlayer(int victim_num, int killer_num,
                struct Position murder_pos) {
  ++g_table.player_stats[victim_num].death;
  if (victim_num != killer_num) {
    /* Suicide is not murder! */
    ++g_table.player_stats[killer_num].score;
    if (g_table.player_stats[killer_num].length <= MAX_LENGTH)
      ++g_table.player_stats[killer_num].length;
  }

  g_field.location[murder_pos.y][murder_pos.x] = FIRE;
  g_player_info[victim_num].state = DEAD;
  g_player_info[victim_num].res_time = RESPAWN_TIME;
  g_player_info[victim_num].pos.x = 0;
  g_player_info[victim_num].pos.y = 0;
  g_table.player_stats[victim_num].bomb = 0;
}

void DetonateBomb(struct Position bomb_pos) {
  int bomb_num = -1;
  int i;

  for (i = 0; i < MAX_PLAYER_AMOUNT && bomb_num == -1; ++i) {
    if (bomb_pos.x == g_player_info[i].bomb_info.pos.x &&
        bomb_pos.y == g_player_info[i].bomb_info.pos.y) {
      bomb_num = i;
    }
  }

  g_table.player_stats[bomb_num].bomb = 1;
  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    if (g_field.location[bomb_pos.y][bomb_pos.x] == (PLAYER_1_BOMB + i)) {
      /* Suicide. */
      KillPlayer(i, i, bomb_pos);
    }
  }
  g_field.location[bomb_pos.y][bomb_pos.x] = FIRE;
  DetonateSide(VERTICAL, bomb_num);
  DetonateSide(HORIZONTAL, bomb_num);
  
  g_player_info[bomb_num].bomb_info.d_time = -1;
  g_player_info[bomb_num].bomb_info.pos.x = 0;
  g_player_info[bomb_num].bomb_info.pos.y = 0;
}

void DetonateSide(uint8_t horizontal, int bomb_num) {
  uint8_t *current_coord,
          *end_coord;
  uint8_t bomb_radius;
  struct Position current_pos,
                  end_pos;
  enum Cell *current_cell;

  end_pos = current_pos = g_player_info[bomb_num].bomb_info.pos;
  bomb_radius = g_table.player_stats[bomb_num].length;

  current_coord = horizontal ? &current_pos.x : &current_pos.y;
  *current_coord -= bomb_radius;
  if (*current_coord < MIN_COORD)
    *current_coord = MIN_COORD;

  end_coord = horizontal ? &end_pos.x : &end_pos.y;
  *end_coord += bomb_radius + 1;
  if (*end_coord >= MAX_COORD)
    *end_coord = MAX_COORD;

  for (*current_coord = *current_coord; *current_coord < *end_coord;
       ++(*current_coord)) {
    current_cell = &g_field.location[current_pos.y][current_pos.x];
    switch (*current_cell) {
      case EMPTY:
        *current_cell = FIRE;
      case FIRE:
        FillFireCell(current_pos, bomb_num);
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
        FillFireCell(current_pos, bomb_num);
        KillPlayer((int) (*current_cell - PLAYER_1), bomb_num, current_pos);
        break;
      case PLAYER_1_BOMB:
      case PLAYER_2_BOMB:
      case PLAYER_3_BOMB:
      case PLAYER_4_BOMB:
        FillFireCell(current_pos, bomb_num);
        DetonateBomb(current_pos);
        /* Now *current_cell == FIRE, so we need sub FIRE
         * instead of PLAYER_1_BOMB. */
        KillPlayer((int) ((*current_cell) - FIRE), bomb_num, current_pos);
        break;
    }
  }
}

void InitializeFireField(void) {
  int i, j;

  for (i = 0; i < FIELD_SIZE; ++i) {
    for (j = 0; j < FIELD_SIZE; ++j) {
      g_fire_field.cell[i][j].time = NO_FIRE;
    }
  }
}

void InitializePlayersInfo(void) {
  int i;
  
  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    g_player_info[i].state = INACTIVE;
    g_player_info[i].pos.x = 0;
    g_player_info[i].pos.y = 0;
    g_player_info[i].bomb_info.d_time = 0;
    g_player_info[i].bomb_info.pos.x = 0;
    g_player_info[i].bomb_info.pos.y = 0;
    g_player_info[i].res_time = 0;
  }
}

/*============================*/
/*                            */
/* Interface definitions.     */
/*                            */
void Update(struct ActionTable *action_table) {
  AddPlayers(action_table);
  SuicidePhase(action_table);
  PlantingPhase(action_table);
  MovingPhase(action_table);
  BoomPhase();
  FirePhase();
  RespawnPhase();
}

void FillField(void) {
  enum Cell **field;
  int i, j;
  int to_deletei, to_deletej;
  field = g_field.location;

  for (i = 0; i < FIELD_SIZE; ++i) {
    for (j = 0; j < FIELD_SIZE; ++j) {
      field[i][j] = BOX;
    }
  }

  for (i = 2; i < FIELD_SIZE; i += 2) {
    for (j = 2; j < FIELD_SIZE; j += 2) {
      field[i][j] = WALL;
    }
  }

  field[1][1] = EMPTY;
  field[1][2] = EMPTY;
  field[2][1] = EMPTY;

  field[FIELD_SIZE - 2][1] = EMPTY;
  field[FIELD_SIZE - 2][2] = EMPTY;
  field[FIELD_SIZE - 3][1] = EMPTY;

  field[FIELD_SIZE - 2][FIELD_SIZE - 2] = EMPTY;
  field[FIELD_SIZE - 2][FIELD_SIZE - 3] = EMPTY;
  field[FIELD_SIZE - 3][FIELD_SIZE - 2] = EMPTY;

  field[1][FIELD_SIZE - 2] = EMPTY;
  field[1][FIELD_SIZE - 3] = EMPTY;
  field[2][FIELD_SIZE - 2] = EMPTY;

  for (i = 0; i < FIELD_SIZE; ++i) {
    field[i][0] =
    field[0][i] =
    field[i][FIELD_SIZE - 1] =
    field[FIELD_SIZE - 1][i] = WALL;
  }

  for (i = 0; i < 28; ++i) {
    do {
      to_deletei = rand() % FIELD_SIZE;
      to_deletej = rand() % FIELD_SIZE;
    } while (g_field.location[to_deletei][to_deletej] != BOX);

    g_field.location[to_deletei][to_deletej] = EMPTY;
  }

}

void SetUp(struct Field **game_field,
           struct StatsTable **stats_table) {
  uint16_t i;

  if (game_field == 0 || stats_table == 0) {
    perror("SetUp(): Argument pointer is null");
    return;
  }

  srand(0);
  *game_field = 0;
  *stats_table = 0;

  /* Code below instantiate 2-dimentional array
   * First dimention is an array of pointers
   * Second dimention is an linear buffer of enum Cell */
  g_field.location = malloc(sizeof(void *) * FIELD_SIZE);
  if (g_field.location == 0) {
    perror("SetUp(): Field alloc(1) error");
    return;
  }
  memset(g_field.location, 0, sizeof(void *) * FIELD_SIZE);

  g_field.location[0] = malloc(sizeof(enum Cell) * FIELD_SIZE * FIELD_SIZE);
  if (g_field.location == 0) {
    perror("SetUp(): Field alloc(2) error");
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

  InitializeFireField();
  InitializePlayersInfo();
}

void TearDown(void) {
  free(g_field.location[0]);
  free(g_field.location);
}
