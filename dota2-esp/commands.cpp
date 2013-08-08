#include "precompiled_headers.h"

#include "commands.hpp"
#include "source-sdk\global_instance_manager.h"
#include "dota\DotaGlobal.h"
#include "dota\DotaItem.h"
#include "lua\lua_engine.hpp"

extern bool simulate_shift_down;

namespace commands {

  static ConCommand announcer_command( "dota_announcer_set", SetAnnouncer, "Set the announcer");
  static ConCommand announcer_killstreak_command( "dota_announcer_killstreak_set", SetKillstreakAnnouncer, "Set the killstreak announcer");
  static ConCommand hud_command("dota_hud_set", SetHud, "Set the hud");

  static ConCommand shift_down("+shift", ShiftDown, "shift down");
  static ConCommand shift_up("-shift", ShiftUp, "shift up");

  CON_COMMAND( set_var, "Change cvar value" ) {
    if ( args.ArgC() < 3 )
    {
      Msg("Usage: set_var <cvarname> <value>\n");
      return;
    }
    ConVar* temp = g_pCVar->FindVar(args.Arg(1));
    temp->SetValue(atoi(args.Arg(2)));
  }

  CON_COMMAND( lua_loadscript, "Load lua script" ) {
    if ( args.ArgC() < 2 )
    {
      Msg("Usage: lua_loadscript <script_name>\n");
      return;
    }
    lua::LuaEngine::GetInstance().LoadScript(args.Arg(1));
  }

  CON_COMMAND( lua_unloadscript, "Unload lua script" ) {
    if ( args.ArgC() < 2 )
    {
      Msg("Usage: lua_unloadscript <script_name>\n");
      return;
    }
    lua::LuaEngine::GetInstance().UnloadScript(args.Arg(1));
  }

  CON_COMMAND( lua_executescript, "Execute lua script" ) {
    if ( args.ArgC() < 2 )
    {
      Msg("Usage: lua_executescript <script_name>\n");
      return;
    }
    lua::LuaEngine::GetInstance().ExecuteScript(args.Arg(1));
  }

  void Register() {
    GlobalInstanceManager::GetCVar()->RegisterConCommand(&dota_esp_draw); 
    GlobalInstanceManager::GetCVar()->RegisterConCommand(&announcer_command); 
    GlobalInstanceManager::GetCVar()->RegisterConCommand(&announcer_killstreak_command); 
    GlobalInstanceManager::GetCVar()->RegisterConCommand(&hud_command);
    GlobalInstanceManager::GetCVar()->RegisterConCommand(&shift_down);
    GlobalInstanceManager::GetCVar()->RegisterConCommand(&shift_up);
    GlobalInstanceManager::GetCVar()->RegisterConCommand(&set_var_command);
    GlobalInstanceManager::GetCVar()->RegisterConCommand(&lua_loadscript_command);
    GlobalInstanceManager::GetCVar()->RegisterConCommand(&lua_unloadscript_command);
    GlobalInstanceManager::GetCVar()->RegisterConCommand(&lua_executescript_command);
  }
  void Unregister() {
    GlobalInstanceManager::GetCVar()->UnregisterConCommand(&dota_esp_draw); 
    GlobalInstanceManager::GetCVar()->UnregisterConCommand(&announcer_command); 
    GlobalInstanceManager::GetCVar()->UnregisterConCommand(&announcer_killstreak_command); 
    GlobalInstanceManager::GetCVar()->UnregisterConCommand(&hud_command);
    GlobalInstanceManager::GetCVar()->UnregisterConCommand(&shift_down);
    GlobalInstanceManager::GetCVar()->UnregisterConCommand(&shift_up);
    GlobalInstanceManager::GetCVar()->UnregisterConCommand(&set_var_command);
    GlobalInstanceManager::GetCVar()->UnregisterConCommand(&lua_loadscript_command);
    GlobalInstanceManager::GetCVar()->UnregisterConCommand(&lua_unloadscript_command);
    GlobalInstanceManager::GetCVar()->UnregisterConCommand(&lua_executescript_command);
  }

  void SetAnnouncer( const CCommandContext &context, const CCommand &args) {
    if ( args.ArgC() < 2 ) {
      Msg("Usage: dota_set_announcer <item_id>\n");
      return;
    }
    dota::DotaPlayer* player = (dota::DotaPlayer*)GlobalInstanceManager::GetClientTools()->GetLocalPlayer();
    if (player == nullptr) {
      Msg("Could not get local player\n");
      return;
    }

    dota::EconItemView* item = dota::EconItemView::Create();
    int item_id = atoi(args.Arg(1));
    if (item_id == 0) {
      Msg("could not convert item_id to integer\n");
      return;
    }
    item->Init(item_id, 9999, 9999, 0);

    int team_index = player->GetTeamIndex();
    dota::GameRules* gamerules = dota::GameRules::GetInstance();

    if (gamerules == nullptr) {
      Msg("Could not get gamerules\n");
      return;
    }

    dota::UnitAnnouncer* announcer = gamerules->GetAnnouncer(team_index);

    if (announcer == nullptr) {
      Msg("Could not get announcer\n");
      return;
    }

    announcer->SetAnnouncerItem(item, 1);
  }

  void SetKillstreakAnnouncer(const CCommandContext &context, const CCommand &args) {
    if ( args.ArgC() < 2 ) {
      Msg("Usage: dota_announcer_killstreak_set <item_id>\n");
      return;
    }
    dota::DotaPlayer* player = (dota::DotaPlayer*)GlobalInstanceManager::GetClientTools()->GetLocalPlayer();
    if (player == nullptr) {
      Msg("Could not get local player\n");
      return;
    }

    dota::EconItemView* item = dota::EconItemView::Create();
    int item_id = atoi(args.Arg(1));
    if (item_id == 0) {
      Msg("could not convert item_id to integer\n");
      return;
    }
    item->Init(item_id, 9999, 9999, 0);

    int team_index = player->GetTeamIndex();
    dota::GameRules* gamerules = dota::GameRules::GetInstance();

    if (gamerules == nullptr) {
      Msg("Could not get gamerules\n");
      return;
    }

    dota::UnitAnnouncer* announcer = gamerules->GetKillingSpreeAnnouncer(team_index);

    if (announcer == nullptr) {
      Msg("Could not get announcer\n");
      return;
    }

    announcer->SetAnnouncerItem(item, 1);
  }

  void SetHud( const CCommandContext &context, const CCommand &args ) {
    if ( args.ArgC() < 2 ) {
      Msg("Usage: dota_hud_set <item_id>\n");
      return;
    }

    dota::EconItemView* item = dota::EconItemView::Create();
    int item_id = atoi(args.Arg(1));
    if (item_id == 0) {
      Msg("could not convert item_id to integer\n");
      return;
    }
    item->Init(item_id, 9999, 9999, 0);

    dota::Hud::SelectHudSkin(item, 1);
  }

  void ShiftDown(const CCommandContext &context, const CCommand &args) {
    simulate_shift_down = true;
  }

  void ShiftUp(const CCommandContext &context, const CCommand &args) {
    simulate_shift_down = false;
  }
}
