// Copyright 2013 Karl Skomski - GPL v3
#include "lua/lua_state.h"

namespace lua {
  static void Deleter(lua_State* state) {
    lua_close(state);
  }
  LuaState::LuaState() : state_(luaL_newstate(), Deleter) {
    luaL_openlibs(state_.get());
  }
  void LuaState::LoadFile(std::string name) {
    std::string module_path_with_filename = utils::GetModulePath(utils::GetCurrentModule());
    char module_drive[_MAX_DRIVE];
    char module_dir[_MAX_DIR];

    _splitpath(module_path_with_filename.c_str(), module_drive, module_dir, NULL, NULL);

    std::string module_path(module_drive);
    module_path += module_dir;
    module_path += name;

    int status = luaL_loadfile(state_.get(), module_path.c_str());
    if (status != 0) {
      Warning("booster-lua: %s \n",  lua_tostring(state_.get(), -1));
      lua_pop(state_.get(), 1);
    }
  }

  void LuaState::ExecuteProgram() {
    try {
      luabridge::LuaException::pcall(state_.get());
      luabridge::LuaRef init_function = luabridge::getGlobal(state_.get(), "Initialize");
      init_function();
    }
    catch (luabridge::LuaException const& e) {
      Warning("booster-lua: %s \n", e.what());
    }
    catch (...) {
      Warning("booster-lua: %s \n", "default lua exception");
    }
  }
}  // namespace lua
