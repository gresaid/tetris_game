#include "../include/random_util.h"
#include "ctime"
#include "iostream"
#include "stdlib.h"

random_util::random_util() { srand(unsigned(time(0))); }

double
random_gen(double start, double end) {
  return start + (end - start) * rand() / (RAND_MAX + 1.0);
}

int
random_util::rand_number(int x, int y) {
  return int(random_gen(x, y));
}
