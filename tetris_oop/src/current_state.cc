#include "../include/current_state.h"

void
current_state::move_current(const int x, const int y) {

  int i;
  for (int i = 0; i < y; ++i) {
    puts("\33[2C");
  }
  for (int i = 0; i < x; ++i) {
    puts("\33[1B");
  }
}

void
current_state::resume_current() {
  puts("\33[u");
}

void
current_state::save_current() {
  puts("\33[s");
}