#include "../include/game.h"
#include <cstdio>
#include <cstring>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define BOARD_ROW 24
#define BOARD_COL 17
#define SPEED      500000
#define BOARD_SIZE (BOARD_COL * BOARD_ROW * sizeof(int))
#define SHAPE_SIZE (3 * 3 * sizeof(int))

pthread_mutex_t mutex_lock; //////////////////////////////////////////////////////

static int stop_flag;
static random_util r;

void
game::print_next_shape(Context* m_graph) {
  int x = 7;
  int y = 19;
  int i, j;
  current_state c;
  shape_point p;

  current_state::save_current();
  c.move_current(6, 19);
  std::cout << "next : ";
  current_state::resume_current();

  int a[3][3] = {0};
  memcpy(a, m_graph->get_array(), SHAPE_SIZE);
  for (i = x; i < x + 3; i++) {
    for (j = y; j < y + 3; j++) {
      p.set_locate(i, j);
      p.set_color(CLEAR);
      p.print_shape();
      if (a[i - x][j - y] == 1) {

        p.set_color(m_graph->get_color());
        p.print_shape();
      }
    }
  }
}

void
game::print_help() {
  current_state c;
  c.save_current();
  c.move_current(10, 19);
  std::cout << "a: go to left" << endl;
  c.resume_current();
  c.save_current();
  current_state::move_current(11, 19);
  std::cout << "d: go to right" << endl;
  c.resume_current();
  c.save_current();
  c.move_current(12, 19);
  std::cout << "w: rotate cube" << endl;
  c.resume_current();
  c.save_current();
  c.move_current(13, 19);
  std::cout << "game_score: go to down" << endl;
  c.resume_current();
  c.save_current();
  c.move_current(14, 19);
  std::cout << "p: run or pause" << endl;
  c.resume_current();
}

void
game::game_init() {
  print_help();

  next_shape = new Context(get_shape());
  next_shape->draw();
  next_shape->set_locate(1, 7);

  create_shape();
}

MARK
game::get_mark() {
  return mark;
}

void
game::set_mark(MARK mark) {
  this->mark = mark;
}

game::~game() {
  if (nullptr != game_shape) {
    delete game_shape;
    game_shape = nullptr;
  }

  if (nullptr != next_shape) {
    delete next_shape;
    next_shape = nullptr;
  }
}

game::game() {
  game_shape = nullptr;
  x = 1;
  y = 7;
  shape_point p;
  int i;
  game_score.print_score();

  mark = GAME_RUN;

  memset((void*)board_size, 0, BOARD_SIZE);
  memset((void*)p_color, 0, BOARD_SIZE);
  for (i = 0; i < 24; i++) {
    p.set_locate(i, 0);
    p.set_color(BLUE);
    p.print_shape();
    p.set_locate(i, 16);
    p.set_color(BLUE);
    p.print_shape();
    board_size[i][0] = 1;
    board_size[i][16] = 1;
  }
  for (i = 0; i < 17; i++) {
    p.set_locate(23, i);
    p.set_color(BLUE);
    p.print_shape();
    p.set_locate(0, i);
    p.set_color(RED);
    p.print_shape();
    board_size[23][i] = 1;
    board_size[0][i] = 1;
  }
  fflush(stdout);
}

char
game::get_shape() {
  char ch;
  switch (r.random_number(1, 6)) {
    case 1: ch = 'Z'; break;
    case 2: ch = 'T'; break;
    case 3: ch = 'O'; break;
    case 4: ch = 'I'; break;
    case 5: ch = 'L'; break;
    default:
      std::cout << "no this shape type" << endl;
      ch = '\0';
      break;
  }
  return ch;
}

bool
game::clear_board() {
  int i, j;
  int b[3][3] = {0};

  game_shape->printG(CLEAR);
  memcpy(b, game_shape->get_array(), SHAPE_SIZE);
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      board_size[i + x][j + y] -= b[i][j];
      p_color[i][j] = CLEAR;
    }
  }
  return true;
}

bool
game::reset_board() {
  int i, j;
  int b[3][3] = {0};

  memcpy(b, game_shape->get_array(), SHAPE_SIZE);
  for (i = x; i < x + 3; i++) {
    for (j = y; j < y + 3; j++) {
      board_size[i][j] += b[i - x][j - y];
      p_color[i][j] = game_shape->get_color();
    }
  }
  return true;
}

bool
game::set_board() {
  int i, j;
  int b[3][3] = {0};

  game_shape->get_locate(&x, &y);
  memcpy(b, game_shape->get_array(), SHAPE_SIZE);

  for (i = x; i < x + 3; i++) {
    for (j = y; j < y + 3; j++) {
      board_size[i][j] += b[i - x][j - y];
      if (board_size[i][j] > 1) {
        std::cout << "game over" << endl;
        system("stty icanon echo");
        exit(0);
      }
    }
  }
  return true;
}

void
game::create_shape() {
  game_shape = next_shape;
  set_board();
  game_shape->printG(YELLOW);

  next_shape = new Context(get_shape());
  next_shape->draw();
  next_shape->set_locate(1, 7);
  print_next_shape(next_shape);
}

void
game::move(int dir) {

  if (GAME_RUN != mark) {
    return;
  }
  clear_board();
  pthread_mutex_lock(&mutex_lock);
  switch (dir) {
    case DOWN:
      if (!is_touch_bottom()) {
        game_shape->move(DOWN);
        set_board();
        game_shape->printG(YELLOW);
      } else {
        reset_board();
        game_shape->printG(YELLOW);
        clear_line();
        stop();
      }
      break;
    case LEFT:
      if (!is_touch_left()) {
        game_shape->move(LEFT);
        set_board();
        game_shape->printG(YELLOW);
      } else {
        reset_board();
        game_shape->printG(YELLOW);
      }

      break;
    case RIGHT:
      if (!is_touch_right()) {
        game_shape->move(RIGHT);
        set_board();
        game_shape->printG(YELLOW);
      } else {
        reset_board();
        game_shape->printG(YELLOW);
      }
      break;
    default: break;
  }
  pthread_mutex_unlock(&mutex_lock);
}

void
game::rotate() {
  int i, j;
  int flag = 0;
  int b[3][3] = {0};
  int temp[3][3] = {0};

  game_shape->get_locate(&x, &y);
  memcpy(b, game_shape->get_array(), SHAPE_SIZE);
  clear_board();
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      temp[2 - j][i] = b[i][j];
    }
  }
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      if (temp[i][j] == 1 && board_size[x + i][y + j] == 1) {
        flag = 1;
        break;
      }
    }
    if (flag == 1) {
      break;
    }
  }
  if (flag == 0) {
    game_shape->rotate();
  }
  set_board();
  game_shape->printG(YELLOW);
}

void
game::stop() {
  delete game_shape;
  game_shape = nullptr;
  stop_flag = 1;
  create_shape();
}

bool
game::is_touch_bottom() {
  int i, j;
  int cube_x, cube_y;
  int b[3][3] = {0};
  int flag = false;

  game_shape->get_locate(&cube_x, &cube_y);
  memcpy(b, game_shape->get_array(), SHAPE_SIZE);

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      if (b[i][j] == 1 && board_size[i + cube_x + 1][j + cube_y] == 1) {
        flag = true;
        break;
      }
    }
    if (flag == true) {
      break;
    }
  }
  return flag;
}

bool
game::is_touch_left() {
  int i, j;
  int cube_x, cube_y;
  int b[3][3] = {0};
  int flag = false;

  game_shape->get_locate(&cube_x, &cube_y);
  memcpy(b, game_shape->get_array(), SHAPE_SIZE);

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      if (b[i][j] == 1 && board_size[i + cube_x][j + cube_y - 1] == 1) {
        flag = true;
        break;
      }
    }
    if (flag == true) {
      break;
    }
  }
  return flag;
}

bool
game::is_touch_right() {
  int i, j;
  int cube_x, cube_y;
  int b[3][3] = {0};
  int flag = false;

  game_shape->get_locate(&cube_x, &cube_y);
  memcpy(b, game_shape->get_array(), SHAPE_SIZE);

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      if (b[i][j] == 1 && board_size[i + cube_x][j + cube_y + 1] == 1) {
        flag = true;
        break;
      }
    }
    if (flag == true) {
      break;
    }
  }
  return flag;
}

void
game::clear_line() {
  int i, j;
  int flag = 0;
  static int count = 0;
  for (i = 22; i > 0; i--) {
    for (j = 1; j < 16; j++) {
      if (board_size[i][j] == 0) {
        flag = 1;
      }
    }
    if (flag == 0) {
      count++;
      game_score.set_score(count);
      game_score.print_score();
      down(i);
      i++;
    }
    flag = 0;
  }
}

void
game::down(int level) {
  int i, j;

  for (i = level; i > 1; i--) {
    for (j = 1; j < 16; j++) {
      board_size[i][j] = board_size[i - 1][j];
    }
  }
  shape_point p;
  for (i = 1; i < 23; i++) {
    for (j = 1; j < 16; j++) {
      if (board_size[i][j] == 1) {
        p.set_locate(i, j);
        p.set_color(p_color[i][j]);
        p.print_shape();
      }
      if (board_size[i][j] == 0) {
        p.set_locate(i, j);
        p.set_color(CLEAR);
        p.print_shape();
      }
    }
  }
}

[[noreturn]] void*
listenKey(void* ptr) {
  game* ptrg = (game*)ptr;
  char key;
  while (true) {
    system("stty -icanon -echo");
    key = getchar();
    system("stty icanon echo");
    switch (key) {
      case 'a': ptrg->move(LEFT); break;
      case 'd': ptrg->move(RIGHT); break;
      case 'w': ptrg->rotate(); break;
      case 's':
        while (true) {
          if (stop_flag == 1) {
            stop_flag = 0;
            break;
          }
          ptrg->move(DOWN);
        }
        break;
      case 68: ptrg->move(LEFT); break;
      case 67: ptrg->move(RIGHT); break;
      case 65: ptrg->rotate(); break;
      case 66:
        while (true) {
          if (stop_flag == 1) {
            stop_flag = 0;
            break;
          }
          ptrg->move(DOWN);
        }
        break;

      case 'p':
        if (GAME_RUN == ptrg->get_mark()) {
          ptrg->set_mark(GAME_PAUSE);
        } else {
          ptrg->set_mark(GAME_RUN);
        }
        break;

      default: break;
    }
  }
}

int
main() {
  pthread_t t1;
  pthread_mutex_init(&mutex_lock, nullptr);
  system("clear");
  game g;
  g.game_init();
  pthread_create(&t1, nullptr, listenKey, (void*)(&g));

  while (true) {
    fflush(stdout);
    usleep(SPEED);
    g.move(DOWN);
  }
}
