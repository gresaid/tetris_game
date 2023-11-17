#include <iostream>
#include "current_state.h"
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

  shape_point(int color, int x, int y) {
    this->color = color;
    this->x = x;
    this->y = y;
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

  int
  get_color() {
    return color;
  }

  void
  get_locate(int& x, int& y) {
    x = this->x;
    y = this->y;
  }

  void print_shape();
};