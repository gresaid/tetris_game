#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "shape_point.h"
#define DOWN  0
#define LEFT  1
#define RIGHT 2

class game_base {
protected:
  int x;
  int y;
  int a[3][3]{};

public:
  game_base() {
    int i, j;
    x = 0;
    y = 0;
    for (j = 0; j < 3; j++) {
      for (i = 0; i < 3; i++) {
        a[i][j] = 0;
      }
    }
  }

  int move(int dir);
  virtual int rotate();

  virtual void
  draw() {}

  void
  set_locate(int i, int b) {
    x = i;
    y = b;
  }

  void
  get_locate(int* pInt, int* b) const {
    *pInt = x;
    *b = y;
  }

  void printG(int color);

  void*
  get_array() {
    return (void*)a;
  }
};

class z_graph : public game_base {
public:
  void
  draw() override {
    a[0][0] = 1;
    a[0][1] = 1;
    a[0][2] = 0;
    a[1][0] = 0;
    a[1][1] = 1;
    a[1][2] = 1;
    a[2][0] = 0;
    a[2][1] = 0;
    a[2][2] = 0;
  }
};

class t_graph : public game_base {
public:
  void
  draw() override {
    a[0][0] = 1;
    a[0][1] = 1;
    a[0][2] = 1;
    a[1][0] = 0;
    a[1][1] = 1;
    a[1][2] = 0;
    a[2][0] = 0;
    a[2][1] = 0;
    a[2][2] = 0;
  }
};

class o_graph : public game_base {
public:
  void
  draw() override {
    a[0][0] = 1;
    a[0][1] = 1;
    a[0][2] = 0;
    a[1][0] = 1;
    a[1][1] = 1;
    a[1][2] = 0;
    a[2][0] = 0;
    a[2][1] = 0;
    a[2][2] = 0;
  }

  int
  rotate() override {
    return 0;
  }
};

class i_graph : public game_base {
public:
  void
  draw() override {
    a[0][0] = 0;
    a[0][1] = 1;
    a[0][2] = 0;
    a[1][0] = 0;
    a[1][1] = 1;
    a[1][2] = 0;
    a[2][0] = 0;
    a[2][1] = 1;
    a[2][2] = 0;
  }
};

class l_graph : public game_base {
public:
  void
  draw() override {
    a[0][0] = 0;
    a[0][1] = 1;
    a[0][2] = 0;
    a[1][0] = 0;
    a[1][1] = 1;
    a[1][2] = 0;
    a[2][0] = 0;
    a[2][1] = 1;
    a[2][2] = 1;
  }
};

class Context {
private:
  game_base* g_base;
  color game_color;

public:
  ~Context() { delete g_base; }

  explicit Context(char cType) {
    switch (cType) {
      case 'Z':
        g_base = new z_graph();
        game_color = YELLOW;
        break;
      case 'T':
        g_base = new t_graph();
        game_color = GREEN;
        break;
      case 'O':
        g_base = new o_graph();
        game_color = PURPLE;
        break;
      case 'I':
        g_base = new i_graph();
        game_color = DEEP_GREEN;
        break;
      case 'L':
        g_base = new l_graph();
        game_color = WHITE;
        break;
      default: printf("no %c type\n", cType); break;
    }
  }

  int
  move(int dir) {
    return g_base->move(dir);
  }

  int
  rotate() {
    return g_base->rotate();
  }

  void
  draw() {
    g_base->draw();
  }

  void
  set_locate(int a, int b) {
    g_base->set_locate(a, b);
  }

  void
  get_locate(int* a, int* b) {
    g_base->get_locate(a, b);
  }

  void*
  get_array() {
    g_base->get_array();
  }

  void
  printG(int color) {
    if (color == CLEAR) {
      g_base->printG(CLEAR);
    } else {
      g_base->printG(game_color);
    }
  }

  color
  get_color() {
    return game_color;
  }
};
