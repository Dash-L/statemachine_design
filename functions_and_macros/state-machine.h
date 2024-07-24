#pragma once

/* clang-format off
 * State Machine Usage
 * Create a header (say `states.h`)
   * In that header, define a struct to store data shared between all states. You might call it `Context`
   * Now define your states like so:
     * #define STATES(args...) \
     *  STATE(State1, args) \
     *  STATE(State2, args) \
     *  etc...
     * NOTE: The last line must NOT end with a `\`
    * Finally, call the MAKE_STATES macro like so:
      * MAKE_STATES(Name, EnumType, Context, STATES)
        * Where:
          * Name is a prefix added to everything the macro generates, for example Rocket (NOTE: this is not a string)
          * EnumType is the **integer** type that will back the state enum, this may be something like `size_t` or `uint32_t`, for example
          * Context is the shared data struct you defined above
          * STATES is the macro you defined above
  * Create a corresponding cpp file (say `states.cpp`)
    * Implement one function per state, the function declarations are like the following:
      * std::optional<NameState> NameState1(bool initialize, Context *ctx) { ... }
      * so for example if your state machine was called `Rocket` and you had a `Boost` state, you would write
        * std::optional<RocketState> RocketBoost(bool initialize, Context *ctx) { ... }
      * inside these functions, you may use `static` variables to store any data local to the state
        * since static variables are initialized once, it is recommended to initialize all of them to dummy values at the top of the function
        * and then check `if (initialize) { ... }` to do the proper initialization.
        * This will ensure the states work properly even if they are transitioned to a second time
      * When you want to transition to a new state (for example Coast)
        * return std::make_optional(RocketState::Coast);
        * if you don't want to transition yet, return std::nullopt;
  * In the main file (which must include the states header):
    * Create your initial context object
    * Create a `StateMachine<NameState, Context, NUM_Name_STATES>`, passing it NameStateMap, whatever initial state you want, and the initial context object
      * Where `Name` is the name given in `MAKE_STATES`
    * In the loop:
      * Do whatever state agnostic processing you want (potentially involving the context object)
      * Call stateMachine.loop();
      * Do whatever state agnostic cleanup you want (possibly conditional based on stateMachine.justTransitioned())
 clang-format on */

#include <array>
#include <functional>
#include <optional>

#define COMMA_SEP(val) val,
#define ADD(val) +1
#define FN(val, name, ContextType)                                             \
  ::std::optional<name##State> name##val(bool, ContextType *);
#define MAP_ENTRY(val, name, map_name)                                         \
  map_name[static_cast<size_t>(name##State::val)] = name##val;

#define STATE(state, X, ...) X(state __VA_OPT__(, ) __VA_ARGS__)

#define MAKE_STATES(name, Type, ContextType, states)                           \
  enum class name##State : Type{states(COMMA_SEP)};                            \
  constexpr size_t NUM_##name##_STATES = states(ADD);                          \
                                                                               \
  states(FN, name, ContextType);                                               \
                                                                               \
  constexpr auto name##StateMap = []() constexpr {                             \
    using fn_t = ::std::optional<name##State>(bool, ContextType *);            \
    using result_t = ::std::array<fn_t *, NUM_##name##_STATES>;                \
    result_t result = {};                                                      \
    states(MAP_ENTRY, name, result) return result;                             \
  }();

template <typename StateEnum, typename ContextType, size_t NumStates>
class StateMachine {
  using state_fn_t = std::optional<StateEnum>(bool, ContextType *);
  using arr_t = std::array<state_fn_t *, NumStates>;

public:
  StateMachine(const arr_t stateMap, StateEnum initState, ContextType *ctx)
      : stateMap(stateMap), state(initState), ctx(ctx) {}

  void loop() {
    auto newState = stateMap[static_cast<size_t>(state)](firstIter, ctx);
    firstIter = false;
    if (newState) {
      state = *newState;
      firstIter = true;
    }
  }

  StateEnum currentState() { return state; }

  bool justTransitioned() { return firstIter; }

private:
  bool firstIter = true;
  arr_t stateMap;
  StateEnum state;
  ContextType *ctx;
};
