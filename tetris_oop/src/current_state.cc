#include "../include/current_state.h"
#include <cstdio>

void
current_state::move_current(const int x, const int y) {
  int i;
  for (i = 0; i < y; i++) {
    printf("\33[2C");
  }
  for (i = 0; i < x; i++) {
    printf("\33[1B");
  }
}

void
current_state::save_current() {
  printf("\33[s");
}

void
current_state::resume_current() {
  printf("\33[u");
}
