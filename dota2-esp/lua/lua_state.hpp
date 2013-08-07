#pragma once

#include "..\utils\utils.h"
#include "..\..\lua\src\lua.hpp"
#include "..\..\LuaBridge\LuaBridge.h"
#include "lua_game_event_listener.hpp"

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