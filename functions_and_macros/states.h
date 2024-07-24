#pragma once

#include "state-machine.h"

struct Context {
  int totalLoops = 0;
  int loopsInState = 0;
};

#define STATES(args...)                                                        \
  STATE(A, args)                                                               \
  STATE(B, args)                                                               \
  STATE(C, args)                                                               \
  STATE(D, args)

MAKE_STATES(Rocket, size_t, Context, STATES)
