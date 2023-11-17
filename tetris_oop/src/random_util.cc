#include "../include/random_util.h"
#include <cstdlib>
#include <ctime>

random_util::random_util() { srand(unsigned(time(nullptr))); }

double
random(double start, double end) {
  return start + (end - start) * rand() / (RAND_MAX + 1.0);
}

int
random_util::random_number(int x, int y) {
  int randNum;
  randNum = int(random(x, y));
  return randNum;
}
