// Copyright 2013 Karl Skomski - GPL v3
#pragma once

#include "utils/utils.h"
#include "LuaJIT/src/lua.h"
#include "LuaBridge/LuaBridge.h"
#include "lua/lua_game_event_listener.h"

#include <memory>

namespace lua {
  class LuaState {
   public:
    LuaState();

    void LoadFile(std::string name);
    void ExecuteProgram();
    void AddListener(CLuaGameEventListener* listener);

    inline operator lua_State*() {
      return state_.get();
    }
  private:
    std::shared_ptr<lua_State> state_;
  };
}