#include "lua_state.hpp"
#include "lua_global.hpp"

#include <map>

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
    LuaEngine(LuaEngine const&);
    void operator=(LuaEngine const&);
  };
}