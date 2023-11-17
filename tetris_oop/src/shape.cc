#include "../include/shape.h"

void
game_base::printG(int color) {
  int i, j;
  shape_point p;
  for (i = x; i < x + 3; i++) {
    for (j = y; j < y + 3; j++) {
      if (a[i - x][j - y] == 1) {
        p.set_locate(i, j);
        p.set_color(color);
        p.print_shape();
      }
    }
  }
}

int
game_base::move(int dir) {
  switch (dir) {
    case DOWN: x++; break;
    case LEFT: y--; break;
    case RIGHT: y++; break;
    default: break;
  }
  return 0;
}

int
game_base::rotate() {
  int i, j;
  int b[3][3];
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      b[2 - j][i] = a[i][j];
    }
  }
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      a[i][j] = b[i][j];
    }
  }
}
