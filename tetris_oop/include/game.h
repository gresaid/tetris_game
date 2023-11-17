#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H
#define GAME_HEIGHT
#define GAME_WEIGHT
#include "shape.h"
#include "random_util.h"
#include "score.h"
class game {
private:
  int game_board[GAME_HEIGHT][GAME_HEIGHT];
  color game_color;
  Context* game_shape;
  Context* next_shape;
  Score game_score;

  bool is_touch_bottom();
  bool is_touch_left();
  bool is_touch_right();
  char get_shape();
  bool set_board();
  bool clear_board();

public:
  game();
  ~game();

  void create_shape();
  int move(int dir);
  void rotate();
  void clear_line();
  void print_next_shape(Context* game_shape);
  void game_init();
};

#endif //TETRIS_GAME_H
