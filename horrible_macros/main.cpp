#include <functional>
#include <iostream>

enum class State { A, B, C, D };

#define INIT(...) __VA_OPT__(, ) __VA_ARGS__
#define UNPACK(...) __VA_ARGS__

#define STATE(stateId, initExpr, bodyExpr)                                     \
  case State::stateId:                                                         \
    static ::std::function<void()> handle##stateId = []() {};                  \
    if (firstIter) {                                                           \
      handle##stateId = [&INIT(initExpr)]() mutable { bodyExpr };              \
      firstIter = false;                                                       \
    }                                                                          \
    handle##stateId();                                                         \
    break;

#define TRANSIT(newState)                                                      \
  do {                                                                         \
    state = newState;                                                          \
    firstIter = true;                                                          \
  } while (0)

int main(int argc, char **argv) {
  State state = State::A;
  bool firstIter = true;

  for (size_t i = 0; i < 10; i++) {
    // clang-format off
    switch (state) {
      STATE(
          A,
          UNPACK(test = false, another = 2),
          std::cout << "State A" << std::endl;
          if (test) {
            TRANSIT(State::B);
          }
          test = true;
      )
      STATE(
          B, ,
          std::cout << "State B" << std::endl;
          TRANSIT(State::A);
      )
      STATE(C, , )
      STATE(D, , )
    }
    // clang-format on
  }
  return 0;
}

