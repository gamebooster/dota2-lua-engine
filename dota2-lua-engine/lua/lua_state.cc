// Copyright 2013 Karl Skomski - GPL v3
#include <string>

#include "lua/lua_state.h"

namespace lua {
  static void Deleter(lua_State* state) {
    lua_close(state);
  }
  LuaState::LuaState() : state_(luaL_newstate(), Deleter) {
    luaL_openlibs(state_.get());
  }
  bool LuaState::LoadFile(std::string name) {
    std::string module_dir = utils::GetModuleDirectory();
    module_dir += name;

    int status = luaL_loadfile(state_.get(), module_dir.c_str());
    if (status != 0) {
      Warning("booster-lua: %s \n",  lua_tostring(state_.get(), -1));
      lua_pop(state_.get(), 1);
      return false;
    }
    return true;
  }

  void LuaState::ExecuteProgram() {
    try {
      luabridge::LuaException::pcall(state_.get());
      luabridge::LuaRef init_function =
        luabridge::getGlobal(state_.get(), "Initialize");
      init_function();
    }
    catch(luabridge::LuaException const& e) {
      Warning("booster-lua: %s \n", e.what());
    }
    catch(...) {
      Warning("booster-lua: %s \n", "default lua exception");
    }
  }
}  // namespace lua
