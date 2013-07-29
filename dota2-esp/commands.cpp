#include "commands.h"
#include "source-sdk\global_instance_manager.h"
#include "dota\DotaGlobal.h"
#include "dota\DotaItem.h"

extern bool simulate_shift_down;

namespace commands {
  void Register() {
    GlobalInstanceManager::GetCVar()->RegisterConCommand(&dota_esp_draw); 
    GlobalInstanceManager::GetCVar()->RegisterConCommand(&announcer_command); 
    GlobalInstanceManager::GetCVar()->RegisterConCommand(&announcer_killstreak_command); 
    GlobalInstanceManager::GetCVar()->RegisterConCommand(&hud_command);
    GlobalInstanceManager::GetCVar()->RegisterConCommand(&shift_down);
    GlobalInstanceManager::GetCVar()->RegisterConCommand(&shift_up);
  }
  void Unregister() {
    GlobalInstanceManager::GetCVar()->UnregisterConCommand(&dota_esp_draw); 
    GlobalInstanceManager::GetCVar()->UnregisterConCommand(&announcer_command); 
    GlobalInstanceManager::GetCVar()->UnregisterConCommand(&announcer_killstreak_command); 
    GlobalInstanceManager::GetCVar()->UnregisterConCommand(&hud_command);
    GlobalInstanceManager::GetCVar()->UnregisterConCommand(&shift_down);
    GlobalInstanceManager::GetCVar()->UnregisterConCommand(&shift_up);
  }
  void SetAnnouncer( const CCommandContext &context, const CCommand &args) {
    if ( args.ArgC() < 2 ) {
      Msg("Usage: dota_set_announcer <item_id>\n");
      return;
    }
    CDotaPlayer* player = (CDotaPlayer*)GlobalInstanceManager::GetClientTools()->GetLocalPlayer();
    if (player == nullptr) {
      Msg("Could not get local player\n");
      return;
    }

    EconItemView* item = EconItemView::Create();
    int item_id = atoi(args.Arg(1));
    if (item_id == 0) {
      Msg("could not convert item_id to integer\n");
      return;
    }
    item->Init(item_id, 9999, 9999, 0);

    int team_index = player->GetTeamIndex();
    CGameRules* gamerules = CGameRules::GetInstance();

    if (gamerules == nullptr) {
      Msg("Could not get gamerules\n");
      return;
    }

    CAnnouncer* announcer = gamerules->GetAnnouncer(team_index);

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
    CDotaPlayer* player = (CDotaPlayer*)GlobalInstanceManager::GetClientTools()->GetLocalPlayer();
    if (player == nullptr) {
      Msg("Could not get local player\n");
      return;
    }

    EconItemView* item = EconItemView::Create();
    int item_id = atoi(args.Arg(1));
    if (item_id == 0) {
      Msg("could not convert item_id to integer\n");
      return;
    }
    item->Init(item_id, 9999, 9999, 0);

    int team_index = player->GetTeamIndex();
    CGameRules* gamerules = CGameRules::GetInstance();

    if (gamerules == nullptr) {
      Msg("Could not get gamerules\n");
      return;
    }

    CAnnouncer* announcer = gamerules->GetKillingSpreeAnnouncer(team_index);

    if (announcer == nullptr) {
      Msg("Could not get announcer\n");
      return;
    }

    announcer->SetAnnouncerItem(item, 1);
  }

  void SetHud( void ) {
    Msg("This is my SetHud\n");
  }

  void ShiftDown(const CCommandContext &context, const CCommand &args) {
    simulate_shift_down = true;
  }

  void ShiftUp(const CCommandContext &context, const CCommand &args) {
    simulate_shift_down = false;
  }

  CON_COMMAND( set_var, "Change cvar value" )
  {
    if ( args.ArgC() < 3 )
    {
      Msg("Usage: set_var <cvarname> <value>\n");
      return;
    }
    ConVar* temp = g_pCVar->FindVar(args.Arg(1));
    temp->SetValue(atoi(args.Arg(2)));
  }
}
