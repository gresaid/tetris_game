#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <termios.h>
#include <time.h>

#define beep()                 puts("\a")
#define clear_screen()         puts("\033[2J\033[1;1H")
#define move(x, y)             printf("\033[%d;%dH", y, x)
#define hide_cursor()          puts("\033[?25l")
#define background_color(c, s) printf("\033[%dm" s, c ? c + 40 : 0)

#define SIGNAL(signo, cb)                                                                                              \
  sigemptyset(&sa.sa_mask);                                                                                            \
  sigaddset(&sa.sa_mask, signo);                                                                                       \
  sa.sa_flags = 0;                                                                                                     \
  sa.sa_handler = cb;                                                                                                  \
  sigaction(signo, &sa, NULL)

#define columns       10
#define rows          10
#define boarder_size  (rows * columns)

#define top_left      (-columns - 1)
#define top_center    (-columns)
#define top_right     (-columns + 1)
#define middle_left   (-1)
#define middle_right  (1)
#define bottom_left   (columns - 1)
#define bottom_center (columns)
#define bottom_right  (columns + 1)

#define DEFAULT_KEYS  "hlkjq"
#define KEY_LEFT      0
#define KEY_RIGHT     1
#define KEY_ROTATE    2
#define KEY_DROP      3
#define KEY_QUIT      4

static volatile sig_atomic_t running = 1;

static struct termios savemodes;
static int havemodes = 0;

static char* keys = DEFAULT_KEYS;
static int level = 1;
static int points = 0;
static int lines_cleared = 0;
static int board[boarder_size], shadow[boarder_size];

static int* peek_shape;
static int* shape;
static int color;
static int shapes[] = {
    7,  top_left,    top_center,    middle_right,
    2, /* ""__   */
    8,  top_right,   top_center,    middle_left,
    3, /* __""   */
    9,  middle_left, middle_right,  bottom_center,
    1, /* "|"    */
    3,  top_left,    top_center,    middle_left,
    4, /* square */
    12, middle_left, bottom_left,   middle_right,
    5, /* |"""   */
    15, middle_left, bottom_right,  middle_right,
    6, /* """|   */
    18, middle_left, middle_right,  2,
    7, /* ---- sticks out */
    0,  top_center,  middle_left,   bottom_left,
    2, /* /    */
    1,  top_center,  middle_right,  bottom_right,
    3, /* \    */
    10, top_center,  middle_right,  bottom_center,
    1, /* |-   */
    11, top_center,  middle_left,   middle_right,
    1, /* _|_  */
    2,  top_center,  middle_left,   bottom_center,
    1, /* -|   */
    13, top_center,  bottom_center, bottom_right,
    5, /* |_   */
    14, top_right,   middle_left,   middle_right,
    5, /* ___| */
    4,  top_left,    top_center,    bottom_center,
    5, /* "|   */
    16, top_right,   top_center,    bottom_center,
    6, /* |"   */
    17, top_left,    middle_right,  middle_left,
    6, /* |___ */
    5,  top_center,  bottom_center, bottom_left,
    6, /* _| */
    6,  top_center,  bottom_center, 2 * columns,
    7, /* | sticks out */
};

static void
draw(int x, int y, int c) {
  move(x, y);
  background_color(c, "  ");
}

static int
update() {
  int x, y;

  for (y = 1; y < rows - 1; y++) {
    for (x = 0; x < columns; x++) {
      if (board[y * columns + x] - shadow[y * columns + x]) {
        int c = board[y * columns + x]; /* color */

        shadow[y * columns + x] = c;
        draw(x * 2 + 28, y, c);
      }
    }
  }
  while (lines_cleared >= 10) {
    lines_cleared -= 10;
    level++;
  }

  move(26 + 28, 10);
  fflush(stdout);

  return getchar();
}

static int
fits_in(int* s, int pos) {
  if (board[pos] || board[pos + s[1]] || board[pos + s[2]] || board[pos + s[3]]) {
    return 0;
  }

  return 1;
}

static void
place(int* s, int pos, int c) {
  board[pos] = c;
  board[pos + s[1]] = c;
  board[pos + s[2]] = c;
  board[pos + s[3]] = c;
}

static int*
next_shape() {
  int pos = rand() % 7 * 5;
  int* next = peek_shape;
  peek_shape = &shapes[pos];
  if (!next) {
    return next_shape();
  }
  color = next[4];

  return next;
}

static int
tty_init(void) {
  struct termios modmodes;

  if (tcgetattr(fileno(stdin), &savemodes) < 0) {
    return -1;
  }

  havemodes = 1;
  hide_cursor();

  modmodes = savemodes;
  modmodes.c_lflag &= ~ICANON;
  modmodes.c_lflag &= ~ECHO;
  modmodes.c_cc[VMIN] = 1;
  modmodes.c_cc[VTIME] = 0;

  return tcsetattr(fileno(stdin), TCSANOW, &modmodes);
}

static int
tty_exit(void) {
  if (!havemodes) {
    return 0;
  }
  return tcsetattr(fileno(stdin), TCSANOW, &savemodes);
}

static void
alarm_handler(int signo) {
  static long h[4];

  (void)signo;

  /* On init from main() */
  if (!signo) {
    h[3] = 500000;
  }

  h[3] -= h[3] / (3000 - 10 * level);
  setitimer(0, (struct itimerval*)h, 0);
}

static void
exit_handler(int signo) {
  (void)signo;
  running = 0;
}

static void
sig_init(void) {
  struct sigaction sa;

  SIGNAL(SIGINT, exit_handler);
  SIGNAL(SIGTERM, exit_handler);
  SIGNAL(SIGALRM, alarm_handler);

  alarm_handler(0);
}

int
main(void) {
  int c = 0, i, j, *ptr;
  int pos = 17;
  int* backup;

  ptr = board;
  for (i = boarder_size; i; i--) {
    *ptr++ = i < 40 || i % columns < 2 ? 60 : 0;
  }

  srand((unsigned int)time(NULL));
  if (tty_init() == -1) {
    return 1;
  }

  sig_init();
  clear_screen();
  shape = next_shape();
  while (running) {
    if (c < 0) {
      if (fits_in(shape, pos + columns)) {
        pos += columns;
      } else {
        place(shape, pos, color);
        ++points;
        for (j = 0; j < 252; j = columns * (j / columns + 1)) {
          for (; board[++j];) {
            if (j % columns == 10) {
              lines_cleared++;
              beep();
              for (; j % columns; board[j--] = 0)
                ;
              for (; --j; board[j + columns] = board[j])
                ;
              c = update();
            }
          }
        }
        shape = next_shape();
        if (!fits_in(shape, pos = 17)) {
          c = keys[KEY_QUIT];
        }
      }
    }

    if (c == keys[KEY_LEFT]) {
      if (!fits_in(shape, --pos)) {
        ++pos;
      }
    }
    if (c == keys[KEY_ROTATE]) {
      backup = shape;
      shape = &shapes[5 * *shape];
      if (!fits_in(shape, pos)) {
        shape = backup;
      }
    }

    if (c == keys[KEY_RIGHT]) {
      if (!fits_in(shape, ++pos)) {
        --pos;
      }
    }

    if (c == keys[KEY_DROP]) {
      for (; fits_in(shape, pos + columns); ++points) {
        pos += columns;
      }
    }

    place(shape, pos, color);
    c = update();
    place(shape, pos, 0);
  }

  clear_screen();
  if (tty_exit() == -1) {
    return 1;
  }

  return 0;
}
