#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "shape_point.h"

class g_base {
private:
  int x;
  int y;
  int a[3][3];

public:
  g_base() {
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
  set_locate(int a, int b) {
    x = a;
    y = b;
  }

  void
  get_locate(int* a, int* b) {
    *a = x;
    *b = y;
  }

  void printG(int color);

  void*
  get_array() {
    return (void*)a;
  }
};

class z_graph : public g_base {
public:
  void
  draw() {
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

class t_graph : public g_base {
public:
  void
  draw() {
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

class o_graph : public g_base {
public:
  void
  draw() {
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

  virtual int
  rotate() {}
};

class i_graph : public g_base {
public:
  void
  draw() {
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

class l_graph : public g_base {
public:
  void
  draw() {
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
  Gbase* gbase;
  color clr;

public:
  ~Context() { delete gbase; }

  Context(char cType) {
    switch (cType) {
      case 'Z':
        gbase = new z_graph();
        clr = YELLOW;
        break;
      case 'T':
        gbase = new t_graph();
        clr = GREEN;
        break;
      case 'O':
        gbase = new o_graph();
        clr = PURPLE;
        break;
      case 'I':
        gbase = new i_graph();
        clr = DEEP_GREEN;
        break;
      case 'L':
        gbase = new l_graph();
        clr = WHITE;
        break;
      default: printf("no %c type\n", cType); break;
    }
  }

  int
  move() {
    return gbase->move(dir);
  }

  int
  rotate() {
    return gbase->rotate();
  }

  void
  draw() {
    gbase->draw();
  }

  void
  set_locate() {
    gbase->set_locate(a, b);
  }

  void
  get_locate() {
    gbase->get_locate(a, b);
  }

  void*
  get_array() {
    gbase->get_array();
  }

  void
  printG(int color) {
    if (color == CLEAR) {
      gbase->printG(CLEAR);
    } else {
      gbase->printG(clr);
    }
  }

  void
  set_color(color clr) {
    this->clr = clr;
  }

  color
  get_color() {
    return clr;
  }
};
