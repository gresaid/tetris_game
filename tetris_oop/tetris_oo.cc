#include <ctime>
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
class piece;



class game_coordinator {
public:
  int tick_rate_update(piece& piece);
  void frame(board& board);

  int
  get_tick() {
    return tick;
  }

  void
  set_tick(int tick) {
    this->tick = tick;
  }

private:
  int tick;
};

class piece {
public:
  int bit_extract(int x, int y);
  void new_piece();
  void set_piece(int x, int y, int r, int v, board& game_board);
  void update_piece();

  int
  get_x() {
    return x;
  }

  int
  get_y() {
    return y;
  }

  int
  get_r() {
    return r;
  }

  int
  get_c() {
    return c;
  }

  void
  set_c(int c) {
    this->c = c;
  }

  int
  get_px() {
    return px;
  }

  int
  get_py() {
    return py;
  }

  int
  get_pr() {
    return pr;
  }

  int
  get_p() {
    return p;
  }

private:
  int x = 431424;
  int y = 598356;
  int r = 427089;
  int px = 247872;
  int py = 799248;
  int c = 348480;
  int p = 615696;
  int pr;
  int shapes[7][4] = {{x, y, x, y},
                      {r, p, r, p},
                      {c, c, c, c},
                      {599636, 431376, 598336, 432192},
                      {411985, 610832, 415808, 595540},
                      {px, py, px, py},
                      {614928, 399424, 615744, 428369}};
};

void
piece::new_piece() {
  y = py = 0;
  p = rand() % 7;
  r = pr = rand() % 4;
  x = px = rand() % (10 - bit_extract(r, 16));
}

int
piece::bit_extract(int x, int y) {
  return 3 & shapes[p][x] >> y;
}

void
piece::set_piece(int x, int y, int r, int v, board& game_board) {
  for (int i = 0; i < 8; i += 2) {
    game_board.board[bit_extract(r, i * 2) + y][bit_extract(r, (i * 2) + 2) + x] = v;
  }
}

void
board::remove_lines(piece& piece) {
  for (int row = piece.get_y(); row <= piece.get_y() + piece.bit_extract(piece.get_r(), 18); row++) {
    piece.set_c(1);
    for (int i = 0; i < 10; i++) {
      piece.set_c(piece.get_c() * board[row][i]);
    }
    if (!piece.get_c()) {
      continue;
    }
    for (int i = row - 1; i > 0; i--) {
      memcpy(&board[i + 1][0], &board[i][0], 40);
    }
    memset(&board[0][0], 0, 10);
    score++;
  }
}

int
board::check_hit(int x, int y, int r, piece& piece,class board &game_board) {
  if (piece.get_y() + piece.bit_extract(piece.get_r(), 18) > 19) {
    return 1;
  }
  piece.set_piece(piece.get_px(), piece.get_py(), piece.get_pr(), 0);
  piece.set_c(0);
  for (int i = 0; i < 8; i += 2) {
    game_board.board[piece.get_y() + piece.bit_extract(piece.get_r(),i*2)][piece.get_x()+piece.bit_extract(piece.get_r(),(i*2)+2)] && piece.get_c(piece.set_c(piece.get_c()+1));
  }
}
void
game_coordinator::tick_rate_update(piece& piece) {
  if (++tick > 30) {
    tick = 0;
    if (check_hit(piece.get_x(), piece.get_y() + 1, piece.get_r())) {
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
}
