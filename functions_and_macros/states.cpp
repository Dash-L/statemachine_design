#include "states.h"
#include <iostream>
#include <optional>

std::optional<RocketState> RocketA(bool init, Context *ctx) {
  std::cout << "Hi from A" << std::endl;
  if (ctx->loopsInState == 2) {
    return std::make_optional(RocketState::B);
  }
  return std::nullopt;
}

std::optional<RocketState> RocketB(bool init, Context *ctx) {
  std::cout << "Hi from B" << std::endl;
  if (ctx->loopsInState == 1) {
    return std::make_optional(RocketState::C);
  }
  return std::nullopt;
}

std::optional<RocketState> RocketC(bool init, Context *ctx) {
  static bool should_tx{};
  if (init) {
    should_tx = false;
  }
  std::cout << "Hi from C" << std::endl;
  if (should_tx) {
    return std::make_optional(RocketState::D);
  }
  should_tx = true;
  return std::nullopt;
}

std::optional<RocketState> RocketD(bool init, Context *ctx) {
  std::cout << "Hi from D" << std::endl;
  return std::make_optional(RocketState::C);
}
