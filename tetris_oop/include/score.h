#include "string"
#ifndef TETRIS_SCORE_H
#define TETRIS_SCORE_H

class score {
private:
  string player_name;
  int score;

public:
  score() {
    player_name = "aid";
    score = 0;
  };

  void
  set_name(string name) {
    player_name = name;
  };

  void
  set_score(int score) {
    this->score = score;
  };

  void print_score();
};
#endif //TETRIS_SCORE_H
