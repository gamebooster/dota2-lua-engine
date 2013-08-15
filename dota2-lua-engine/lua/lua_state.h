// Copyright 2013 Karl Skomski - GPL v3
#pragma once

#include <memory>
#include <string>

#include "utils/utils.h"
#include "LuaJIT/src/lua.h"
#include "LuaBridge/LuaBridge.h"
#include "lua/lua_game_event_listener.h"

namespace lua {
  class LuaState {
   public:
    LuaState();

    bool LoadFile(std::string name);
    void ExecuteProgram();
    void AddListener(CLuaGameEventListener* listener);

    inline operator lua_State*() {
      return state_.get();
    }
  private:
    std::shared_ptr<lua_State> state_;
  };
}  // namespace lua
