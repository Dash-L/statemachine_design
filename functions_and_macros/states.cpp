#include "states.h"
#include <iostream>
#include <optional>

namespace Rocket {
std::optional<State> A(bool init, Context *ctx) {
  std::cout << "Hi from A" << std::endl;
  if (ctx->loopsInState == 2) {
    return std::make_optional(State::B);
  }
  return std::nullopt;
}

std::optional<State> B(bool init, Context *ctx) {
  std::cout << "Hi from B" << std::endl;
  if (ctx->loopsInState == 1) {
    return std::make_optional(State::C);
  }
  return std::nullopt;
}

std::optional<State> C(bool init, Context *ctx) {
  static bool should_tx{};
  if (init) {
    should_tx = false;
  }
  std::cout << "Hi from C" << std::endl;
  if (should_tx) {
    return std::make_optional(State::D);
  }
  should_tx = true;
  return std::nullopt;
}

std::optional<State> D(bool init, Context *ctx) {
  std::cout << "Hi from D" << std::endl;
  return std::make_optional(State::C);
}
} // namespace Rocket
