#include "precompiled_headers.h"

#include "dota2-esp\commands.hpp"

#include "source-sdk\global_instance_manager.h"
#include "lua\lua_engine.hpp"


namespace commands {
  CON_COMMAND(set_var, "Change cvar value") {
    if (args.ArgC() < 3) {
      Msg("Usage: set_var <cvarname> <value>\n");
      return;
    }
    ConVar* temp = g_pCVar->FindVar(args.Arg(1));
    temp->SetValue(atoi(args.Arg(2)));
  }

  CON_COMMAND(lua_loadscript, "Load lua script") {
    if (args.ArgC() < 2) {
      Msg("Usage: lua_loadscript <script_name>\n");
      return;
    }
    lua::LuaEngine::GetInstance().LoadScript(args.Arg(1));
  }

  CON_COMMAND(lua_unloadscript, "Unload lua script") {
    if (args.ArgC() < 2) {
      Msg("Usage: lua_unloadscript <script_name>\n");
      return;
    }
    lua::LuaEngine::GetInstance().UnloadScript(args.Arg(1));
  }

  CON_COMMAND(lua_executescript, "Execute lua script") {
    if (args.ArgC() < 2) {
      Msg("Usage: lua_executescript <script_name>\n");
      return;
    }
    lua::LuaEngine::GetInstance().ExecuteScript(args.Arg(1));
  }

  void Register() {
    GlobalInstanceManager::GetCVar()->RegisterConCommand(&lua_loadscript_command);
    GlobalInstanceManager::GetCVar()->RegisterConCommand(&lua_unloadscript_command);
    GlobalInstanceManager::GetCVar()->RegisterConCommand(&lua_executescript_command);
  }
  void Unregister() {
    GlobalInstanceManager::GetCVar()->UnregisterConCommand(&lua_loadscript_command);
    GlobalInstanceManager::GetCVar()->UnregisterConCommand(&lua_unloadscript_command);
    GlobalInstanceManager::GetCVar()->UnregisterConCommand(&lua_executescript_command);
  }
}
