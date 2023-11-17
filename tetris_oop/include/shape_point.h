#include <iostream>
#include "current_state.h"
using namespace std;

enum color { CLEAR = 0, BLACK = 30, RED, GREEN, YELLOW, BLUE, PURPLE, DEEP_GREEN, WHITE };

class shape_point {
private:
  int color;
  int x;
  int y;

public:
  shape_point() {
    color = CLEAR;
    x = 0;
    y = 0;
  }

  void
  set_locate(const int x, const int y) {
    this->x = x;
    this->y = y;
  }

  void
  set_color(const int color) {
    this->color = color;
  }

  void print_shape();
};