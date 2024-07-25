#pragma once

#include "state-machine.h"

#define STATES(args...)                                                        \
  STATE(A, args)                                                               \
  STATE(B, args)                                                               \
  STATE(C, args)                                                               \
  STATE(D, args)

namespace Rocket {
  struct Context {
    int totalLoops = 0;
    int loopsInState = 0;
  };

  MAKE_STATES(size_t, Context, STATES)
}
