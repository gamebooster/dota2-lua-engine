// Copyright 2013 Karl Skomski - GPL v3
#include <string>
#include "lua/lua_engine.h"

namespace lua {
  LuaEngine& LuaEngine::GetInstance() {
    static LuaEngine  instance;
    return instance;
  }
  void LuaEngine::LoadScript(std::string name) {
    UnloadScript(name);

    RegisterGlobalFunctions(scripts_[name]);
    if (scripts_[name].LoadFile(name) == false) {
      UnloadScript(name);
    }
  }
  void LuaEngine::UnloadScript(std::string name) {
    std::map<std::string, LuaState>::const_iterator it = scripts_.find(name);
    if (it != scripts_.end()) {
      scripts_.erase(name);
    }
  }
  void LuaEngine::ExecuteScript(std::string name) {
    std::map<std::string, LuaState>::iterator it = scripts_.find(name);
    if (it != scripts_.end()) {
      it->second.ExecuteProgram();
    }
  }

  LuaEngine::LuaEngine() {}

  void LuaEngine::UnloadScripts() {
    scripts_.clear();
  }
}  // namespace lua
