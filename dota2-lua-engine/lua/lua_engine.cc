#include "precompiled_headers.h"

#include "lua_engine.h"

namespace lua {
 LuaEngine& LuaEngine::GetInstance() {
    static LuaEngine  instance;
    return instance;
  }
  void LuaEngine::LoadScript(std::string name) {
    UnloadScript(name);

   RegisterGlobalFunctions(scripts_[name]);
   scripts_[name].LoadFile(name);
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

}