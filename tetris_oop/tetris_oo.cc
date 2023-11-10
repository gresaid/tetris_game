class Game {
public:
  void
  run() {
    while (doTick()) {
      usleep(10000);
      handleInput();
      update();
      draw();
    }
  }

private:
  int x = 431424;
  int y = 598356;
  int r = 427089;
  int px = 247872;
  int py = 799248;
  int pr;
  int c = 348480;
  int p = 615696;
  int tick;
  int board[20][10];
  int score = 0;
  int shapes[7][4] = {{x, y, x, y},
                      {r, p, r, p},
                      {c, c, c, c},
                      {599636, 431376, 598336, 432192},
                      {411985, 610832, 415808, 595540},
                      {px, py, px, py},
                      {614928, 399424, 615744, 428369}};

  int
  NUM(int x, int y) {
    return 3 & shapes[p][x] >> y;
  }

  int
  tick_rate() {
    if (++tick > 30) {
      tick = 0;
      if (check_hit(x, y + 1, r)) {
        if (!y) {
          return 0;
        }
        remove_line();
        new_piece();
      } else {
        y++;
        update_piece();
      }
    }
    return 1;
  }

  int
  check_hit(int x, int y, int r) {
    if (y + NUM(r, 18) > 19) {
      return 1;
    }
    set_piece(px, py, pr, 0);
    c = 0;
    for (int i = 0; i < 8; i += 2) {
      board[y + NUM(r, i * 2)][x + NUM(r, (i * 2) + 2)] && c++;
    }
    set_piece(px, py, pr, p + 1);
    return c;
  }

  void
  set_piece(int x, int y, int r, int v) {
    for (int i = 0; i < 8; i += 2) {
      board[NUM(r, i * 2) + y][NUM(r, (i * 2) + 2) + x] = v;
    }
  }
};