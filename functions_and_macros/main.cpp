#include "state-machine.h"
#include "states.h"
#include <iostream>

int main(int argc, char **argv) {
  Context ctx{};
  StateMachine<RocketState, Context, NUM_Rocket_STATES> rocketStateMachine(
      RocketStateMap, RocketState::A, &ctx);

  for (size_t i = 0; i < 10; i++) {
    std::cout << "Loop " << ctx.totalLoops << ":" << std::endl;

    rocketStateMachine.loop();

    if (rocketStateMachine.justTransitioned()) {
      ctx.loopsInState = 0;
    } else {
      ctx.loopsInState++;
    }
    ctx.totalLoops++;
  }

  return 0;
}
