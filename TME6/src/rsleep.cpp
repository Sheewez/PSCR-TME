#pragma once
#include <ctime>
#include <cstdlib>
#include "rsleep.h"

void randsleep() {

  int r = rand();
  double ratio = (double)r / (double) RAND_MAX;
  struct timespec tosleep;
  tosleep.tv_sec =0;
  // 300 millions de ns = 0.3 secondes
  tosleep.tv_nsec = 300000000 + ratio*700000000;
  struct timespec remain;
  while ( nanosleep(&tosleep, &remain) != 0) {//pour ne pas avoir a refaire le même temps tosleep dans le cas ou on reçoit un signal, on le change en remain
    tosleep = remain;
  }
}
