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
};