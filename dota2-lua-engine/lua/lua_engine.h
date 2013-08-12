// Copyright 2013 Karl Skomski - GPL v3
#include "lua/lua_state.h"
#include "lua/lua_global.h"

#include <map>
#include <string>

namespace lua {
  class LuaEngine {
  public:
    void LoadScript(std::string name);
    void UnloadScript(std::string name);
    void UnloadScripts();
    void ExecuteScript(std::string name);
    static LuaEngine& GetInstance();

  private:
    std::map<std::string, LuaState> scripts_;

    LuaEngine();
    explicit LuaEngine(LuaEngine const&);
    void operator=(LuaEngine const&);
  };
}  // namespace lua
