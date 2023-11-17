#ifndef TETRIS_SCORE_H
#define TETRIS_SCORE_H
#include <iostream>
#include <string>
#include <utility>

class score {
  std::string player;
  int i;

public:
  score() {
    player = "aid";
    i = 0;
  };

  void
  set_player_name(std::string name) {
    player = std::move(name);
  };

  void
  set_score(int s) {
    i = s;
  };

  void print_score();
};

#endif //TETRIS_SCORE_H
