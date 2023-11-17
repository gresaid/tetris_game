#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H
#define GAME_HEIGHT 24
#define GAME_WEIGHT 17
#include "random_util.h"
#include "score.h"
#include "shape.h"

enum MARK { GAME_STOP = -1, GAME_PAUSE = 0, GAME_RUN = 1 };

class game {
private:
  int board_size[GAME_HEIGHT][GAME_WEIGHT]{};
  color p_color[24][17]{};
  Context* game_shape;

  Context* next_shape{};
  MARK mark;

public:
  int x;
  int y;

private:
  score game_score;

private:
  bool reset_board();
  bool is_touch_bottom();
  bool is_touch_left();
  bool is_touch_right();
  static char get_shape();
  bool set_board();
  bool clear_board();

public:
  game();

  ~game();

  void create_shape();
  void move(int dir);
  void rotate();
  void stop();
  void clear_line();
  void down(int level);
  static void print_next_shape(Context* m_graph);
  void game_init();
  MARK get_mark();
  void set_mark(MARK);
  static void print_help();
};
#endif