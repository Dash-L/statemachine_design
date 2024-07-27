#pragma once

/* clang-format off
 * State Machine Usage
 * Create a header (say `states.h`)
  * NOTE: It is highly recommended that you wrap the following steps in a namespace of your choosing
    * This is required if you want multiple state machines, since it removes collisions between them
  * Define a struct to store data shared between all states. You might call it `Context`
  * Call the CREATE_STATE_MACHINE macro like so:
      * CREATE_STATE_MACHINE(Context, State1, State2, etc.)
  * Create a corresponding cpp file (say `states.cpp`)
    * Implement one function per state, the function declarations are like the following (remember to wrap in the same namespace as above, if any):
      * std::optional<State> State1(bool initialize, Context *ctx) { ... }
      * inside these functions, you may use `static` variables to store any data local to the state
        * since static variables are initialized once, it is recommended to initialize all of them to dummy values at the top of the function
        * and then check `if (initialize) { ... }` to do the proper initialization.
        * This will ensure the states work properly even if they are transitioned to a second time
      * When you want to transition to a new state (for example Coast)
        * return std::make_optional(State::Coast);
        * if you don't want to transition yet, return std::nullopt;
  * In the main file (which must include the states header):
    * Create your initial context object
    * Create a `StateMachine`, passing it whatever initial state you want and the initial context object
    * In the loop:
      * Do whatever state agnostic processing you want (potentially involving the context object)
      * Call stateMachine.loop();
      * Do whatever state agnostic cleanup you want (possibly conditional based on stateMachine.justTransitioned())
 clang-format on */

#include <optional>

#define CREATE_STATE_MACHINE(ContextType, states...)                           \
  enum class State : size_t { states };                                        \
                                                                               \
  using fn_t = ::std::optional<State>(bool, ContextType *);                    \
  fn_t states;                                                                 \
                                                                               \
  constexpr fn_t *stateMap[]{states};                                          \
                                                                               \
  class StateMachine {                                                         \
  public:                                                                      \
    StateMachine(State initState, ContextType *ctx)                            \
        : state(initState), ctx(ctx) {}                                        \
                                                                               \
    void loop() {                                                              \
      auto newState = stateMap[static_cast<size_t>(state)](firstIter, ctx);    \
      firstIter = false;                                                       \
      if (newState) {                                                          \
        state = *newState;                                                     \
        firstIter = true;                                                      \
      }                                                                        \
    }                                                                          \
                                                                               \
    State currentState() { return state; }                                     \
                                                                               \
    bool justTransitioned() { return firstIter; }                              \
                                                                               \
  private:                                                                     \
    bool firstIter = true;                                                     \
    State state;                                                               \
    ContextType *ctx;                                                          \
  };
