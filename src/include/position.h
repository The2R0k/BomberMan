#ifndef _POSITION_H_
#define _POSITION_H_

/*message from client to server*/
struct Position {
  /*player's id*/
  int id;
  /*bomb position*/
  int x_pos_bomb;
  int y_pos_bomb;
  /*player position*/
  int x_pos_player;
  int y_pos_player;
};

#define MESGSIZE sizeof(struct Position)

#endif /* _POSITION_H_ */
