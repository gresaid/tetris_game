#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H
#define GAME_HEIGHT 24
#define GAME_WEIGHT 17
#include "random_util.h"
#include "score.h"
#include "shape.h"

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

  int
  get_game_weight() {
    return GAME_WEIGHT;
  }

  int
  get_game_height() {
    return GAME_HEIGHT;
  }
};

#endif //TETRIS_GAME_H
