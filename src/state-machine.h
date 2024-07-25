#pragma once

/* clang-format off
 * State Machine Usage
 * Create a header (say `states.h`)
  * In that header, define your states like so:
    * #define STATES(args...) \
     *  STATE(State1, args) \
     *  STATE(State2, args) \
     *  etc...
     * NOTE: Ensure that if the last line ends with a `\` (it's not required) there is an empty line before any following code
  * NOTE: It is highly recommended that you wrap the following steps in a namespace of your choosing
    * This is required if you want multiple state machines, since it removes collisions between them
  * Define a struct to store data shared between all states. You might call it `Context`
  * Finally, call the MAKE_STATES macro like so:
      * MAKE_STATES(EnumType, Context, STATES)
        * Where:
          * EnumType is the **integer** type that will back the state enum, this may be something like `size_t` or `uint32_t`, for example
          * Context is the shared data struct you defined above
          * STATES is the macro you defined above
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
    * Create a `StateMachine`, passing it stateMap, whatever initial state you want, and the initial context object
    * In the loop:
      * Do whatever state agnostic processing you want (potentially involving the context object)
      * Call stateMachine.loop();
      * Do whatever state agnostic cleanup you want (possibly conditional based on stateMachine.justTransitioned())
 clang-format on */

#include <array>
#include <optional>

#define COMMA_SEP(val) val,
#define ADD(val) +1
#define FN(val, ContextType) ::std::optional<State> val(bool, ContextType *);
#define MAP_ENTRY(val, map_name)                                               \
  map_name[static_cast<size_t>(State::val)] = val;

#define STATE(state, X, ...) X(state __VA_OPT__(, ) __VA_ARGS__)

#define MAKE_STATES(Type, ContextType, states)                                 \
  enum class State : Type { states(COMMA_SEP) };                               \
  constexpr size_t NUM_STATES = states(ADD);                                   \
                                                                               \
  states(FN, ContextType);                                                     \
                                                                               \
  using fn_t = ::std::optional<State>(bool, ContextType *);                    \
  using arr_t = ::std::array<fn_t *, NUM_STATES>;                              \
  constexpr auto stateMap = []() constexpr {                                   \
    arr_t result = {};                                                         \
    states(MAP_ENTRY, result) return result;                                   \
  }();                                                                         \
                                                                               \
  class StateMachine {                                                         \
  public:                                                                      \
    StateMachine(const arr_t stateMap, State initState, Context *ctx)          \
        : stateMap(stateMap), state(initState), ctx(ctx) {}                    \
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
    arr_t stateMap;                                                            \
    State state;                                                               \
    Context *ctx;                                                              \
  };
