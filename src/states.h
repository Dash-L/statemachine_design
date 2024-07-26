#pragma once

#include "state-machine.h"

#define STATES(X)                                                              \
  X(A)                                                                         \
  X(B)                                                                         \
  X(C)                                                                         \
  X(D)

namespace Rocket {

struct Context {
  int totalLoops = 0;
  int loopsInState = 0;
};

CREATE_STATE_MACHINE(Context, STATES)

} // namespace Rocket
