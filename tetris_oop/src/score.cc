#include "../include/score.h"
#include "../include/game.h"
#include "../include/shape_point.h"

void
score::print_score() {
  current_state current_state;
  shape_point shape_point;
  game game;
  shape_point.set_locate(2, game.get_game_weight() + 4);
  shape_point.set_color(WHITE);
  shape_point.print_shape();
  current_state.save_current();
  current_state.move_current(2, game.get_game_weight() + 6);
  printf(player_name);
  current_state.resume_current();
  shape_point.set_locate(4, game.get_game_weight() + 4);
  shape_point.set_color(GREEN);
  shape_point.print_shape();
  current_state.save_current();
  current_state.move_current(4, game.get_game_weight());
  printf(score);
  current_state.resume_current();
}