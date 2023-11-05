#include "stdio.h"
#include "unistd.h"
#define clear_screen           puts("\033[2J\033[1;1H")
#define beep_sound             puts("\a")
/*for ASCII colors*/
#define background_color(c, s) printf("\033[%dm" s, c ? c + 40 : 0)

#define hide_cursor()          puts("\033[?25l")
#define move(x, y)             printf("\033[%d;%dH", y, x)

#define columns                20
#define rows                   20
#define board_size             (columns * rows)

static int board[board_size];
static int shadow[board_size];

/*draw object*/
void
draw(int x, int y, int color) {
  move(x, y);
  background_color(color, "$$$");
}

/* update screen*/
int
update(void) {
  int x, y;
  for (y = 1; y < rows - 1; y++) {
    for (x = 0; x < columns; x++) {
      if (board[y * columns + x] - shadow[y * columns + x]) {
        int color = board[y * columns + x];

        shadow[y * columns + x] = color;
        draw(x*2+ 30, y, color);
      }
    }
  }
  move(rows + columns + 30, 5);
  fflush(stdout);
  return getchar();
}

int
main() {
  /* init ground*/
  int *ptr, i;
  ptr = board;
  for (i = board_size; i; i--) {
    if (i < 40 || i % columns < 2) {
      *ptr++ = 40;
    } else {
      *ptr++ = 0;
    }
  }
  clear_screen;
  hide_cursor();

  while (1) {
    beep_sound;
    update();
    usleep(15000);
  }

  return 0;
}
