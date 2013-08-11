#pragma once

#include "lua/lua_state.h"

namespace lua {
  void RegisterGlobalFunctions(LuaState& state);
}