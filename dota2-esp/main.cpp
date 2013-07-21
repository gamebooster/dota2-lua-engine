#include <windows.h>
#include <stdio.h>
#include <stdexcept>

#include "dota\DotaPlayerResource.h"
#include "dota\DotaChat.h"
#include "dota\DotaGlobal.h"

#include "utils\utils.h"
#include "utils\vmthooks.h"

#include "source-sdk\SDK.h"
#include "source-sdk\draw_utils.h"
#include "source-sdk\global_instance_manager.h"

DWORD health_offset;
DWORD mana_offset;
DWORD max_mana_offset;
DWORD other_hero_model_offset;
DWORD game_time_offset;
DWORD game_rules_address;

static utils::VMTManager* panel_hook;

void __stdcall Hooked_PaintTraverseNew(int unknw, unsigned int vguiPanel, bool forceRepaint, bool allowForce );

void FillOffsets() {
	health_offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_BaseNPC","m_iHealth");
	mana_offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_BaseNPC","m_flMana");
	max_mana_offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_BaseNPC","m_flMaxMana");
	other_hero_model_offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_BaseNPC_Hero","m_hReplicatingOtherHeroModel");
	game_time_offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffsetL("DT_DOTAGamerulesProxy", "dota_gamerules_data", "m_fGameTime");
	game_rules_address = (DWORD)utils::FindPattern(
    "client.dll",
    reinterpret_cast<unsigned char*>("\x8B\x0D\x00\x00\x00\x00\x85\xC9\x74\x08\x8B\x01\x8B\x10\x6A\x01\xFF\xD2\x56\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00"),
    "xx????xxxxxxxxxxxxxxx????????",
    0x2);
}

DWORD WINAPI InitializeHook( LPVOID lpArguments ) {
	while(utils::GetModuleHandleSafe("engine.dll" ) == NULL
    &&  utils::GetModuleHandleSafe( "client.dll" ) == NULL
    &&  utils::GetModuleHandleSafe( "steamclient.dll" ) == NULL) {
		Sleep( 100 );
  }

  FillOffsets();

	panel_hook = new utils::VMTManager(GlobalInstanceManager::GetPanel());
	panel_hook->HookMethod(Hooked_PaintTraverseNew, 45 );

	return 0; 
}

void __stdcall Hooked_PaintTraverseNew(int unknw, unsigned int vguiPanel, bool forceRepaint, bool allowForce ) {
  typedef void ( __thiscall* OriginalFn )( PVOID, int, unsigned int, bool, bool);
  panel_hook->GetMethod<OriginalFn>(45)(GlobalInstanceManager::GetPanel(), unknw, vguiPanel, forceRepaint, allowForce);

  static unsigned int vguiMatSystemTopPanel;

  if (vguiMatSystemTopPanel == NULL) {
    const char* szName = GlobalInstanceManager::GetPanel()->GetName(vguiPanel);
    if( szName[0] == 'M' && szName[3] == 'S' ) vguiMatSystemTopPanel = vguiPanel;
    return;
  }


  if ( vguiMatSystemTopPanel == vguiPanel ) {
    if( GlobalInstanceManager::GetEngineClient()->IsDrawingLoadingImage() ||
        !GlobalInstanceManager::GetEngineClient()->IsInGame() ||
        !GlobalInstanceManager::GetEngineClient()->IsConnected() ||
        GlobalInstanceManager::GetEngineClient()->Con_IsVisible() ||
        (( GetAsyncKeyState(VK_F12) ||
           GlobalInstanceManager::GetEngineClient()->IsTakingScreenshot()))) {
      return;
    }

    int offset_counter = 0;

            dota::DotaPlayerResource* player_resource = dota::DotaPlayerResource::GetPlayerResource();
    CBasePlayer* local_player = (CBasePlayer*)GlobalInstanceManager::GetClientTools()->GetLocalPlayer();
    int local_player_id = local_player->GetPlayerId();
    int local_team =  player_resource->GetTeam(local_player_id);

    for (int i = 1; i < 32; i++ ) {
      CBaseEntity *base_entity = GlobalInstanceManager::GetClientEntityList()->GetClientEntity(i);
      if (base_entity == nullptr) continue;

      if (base_entity->IsPlayer() && base_entity->IsAlive() ) {
        CBasePlayer *base_player = (CBasePlayer *) base_entity;
        int player_id = base_player->GetPlayerId();
        if (player_id != (i - 1)) break;
        CDotaPlayer *dota_player = (CDotaPlayer *) base_entity;

        const char* player_name = player_resource->GetPlayerName(player_id);
        int level = player_resource->GetLevel(player_id);
        int gold = player_resource->GetUnreliableGold(player_id) + player_resource->GetReliableGold(player_id);
        int lasthits = player_resource->GetLastHitCount(player_id);
        int team = player_resource->GetTeam(player_id);

        if (team != local_team) {
        
        sourcesdk::DrawUtils::GetInstance().DrawString(5, 400 + offset_counter  * 30, 255,255,255,255, false, "%d", level);
        sourcesdk::DrawUtils::GetInstance().DrawString(25, 400 + offset_counter * 30, 255,255,255,255, false, "%s", player_name);
        sourcesdk::DrawUtils::GetInstance().DrawString(5, 400 + offset_counter  * 30 + 15, 255,255,255,255, false, "%d", gold);
        sourcesdk::DrawUtils::GetInstance().DrawString(40, 400 + offset_counter  * 30 + 15, 255,255,255,255, false, "%d", lasthits);

        offset_counter++;
        }

        base_entity = GlobalInstanceManager::GetClientEntityList()->GetClientEntity(dota_player->GetAssignedHero());
        if (base_entity == nullptr) continue;

        int health = base_entity->GetValueWithOffset<int>(health_offset);

        float manaMax = base_entity->GetValueWithOffset<float>(max_mana_offset);
        if (manaMax == 0) continue;
        float mana = base_entity->GetValueWithOffset<float>(mana_offset);

        int barWidth = 100;
        int lifeWidth = (int)(mana*100/manaMax);

        Vector vecScreen;
        if (!base_entity->IsIllusion()) {
          int xpos, ypos;

          if (sourcesdk::DrawUtils::GetInstance().GetVectorInScreenSpace(base_entity->GetAbsOrigin(), xpos, ypos)) {
            sourcesdk::DrawUtils::GetInstance().DrawRect(xpos-45,ypos-20-25,barWidth,10,0,0,0,255);
            sourcesdk::DrawUtils::GetInstance().DrawRect(xpos-45,ypos-20-25,lifeWidth,10,0,128,255,255);
            sourcesdk::DrawUtils::GetInstance().OutlineRect(xpos-45-1,ypos-20-25-1,barWidth+2,10+2,0,0,0,255);
            sourcesdk::DrawUtils::GetInstance().DrawString(xpos-45+(barWidth/2),ypos-23-25, 255,255,255,255, true, "%d / %d",(int)mana,(int)manaMax);
            sourcesdk::DrawUtils::GetInstance().DrawString(xpos-45+(barWidth/2),ypos - 100, 255,255,255,255, true, "%d", health);
          }
        }
      }
    }
  }
}

void FinalizeHook() {
  if (panel_hook != nullptr) {
    panel_hook->Unhook();
    delete panel_hook;
  }
}

int WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved) {
  HANDLE thread = nullptr;

  if( DLL_PROCESS_ATTACH == fdwReason ) {
    CreateThread( NULL, 0, InitializeHook, 0, 0, NULL);  
  } else if (DLL_PROCESS_DETACH == fdwReason) {
    FinalizeHook();
	}
	return 1;
}