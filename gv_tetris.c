#include "stdio.h"

#define clear_screen           puts("\033[2J\033[1;1H")
#define background_color(c, s) printf("\033[%dm" s, c ? c + 40 : 0)
#define hide_cursor()          puts("\033[?25l")




int
main() {
  /* init ground*/

  clear_screen;
  background_color(10, " ");
  hide_cursor();
  return 0;
}
