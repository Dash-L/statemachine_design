#pragma once

#include "state-machine.h"

namespace Rocket {

struct Context {
  int totalLoops = 0;
  int loopsInState = 0;
};

CREATE_STATE_MACHINE(Context, A, B, C, D)

} // namespace Rocket
