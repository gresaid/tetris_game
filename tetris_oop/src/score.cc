#include "../include/score.h"
#include "../include/shape_point.h"

void
score::print_score() {
  current_state current_state;
  shape_point shape_point;

  shape_point.set_locate(2, 19);
  shape_point.set_color(WHITE);
  shape_point.print_shape();

  current_state.save_current();
  current_state.move_current(2, 21);
  std::cout << "player_name: " << player;
  current_state.resume_current();

  shape_point.set_locate(4, 19);
  shape_point.set_color(GREEN);
  shape_point.print_shape();

  current_state.save_current();
  current_state.move_current(4, 21);
  std::cout << "score : " << i;
  current_state.resume_current();
}