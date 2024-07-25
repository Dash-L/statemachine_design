#include "states.h"
#include <iostream>

int main(int argc, char **argv) {
  Rocket::Context ctx{};
  Rocket::StateMachine rocketStateMachine(Rocket::stateMap, Rocket::State::A,
                                          &ctx);

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
