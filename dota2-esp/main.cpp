#include "precompiled_headers.h"

#include "source-sdk\SDK.h"

#include "dota\DotaPlayerResource.h"
#include "dota\DotaChat.h"
#include "dota\DotaParticleManager.hpp"
#include <boost/exception/diagnostic_information.hpp> 
#include "dota\DotaGlobal.h"

#include "utils\utils.h"
#include "utils\vmthooks.h"

#include "source-sdk\draw_utils.h"
#include "source-sdk\game_event_listener.h"
#include "source-sdk\global_instance_manager.h"

#include "lua/lua_global.hpp"

#include "commands.hpp"

static utils::VtableHook* panel_hook;
static utils::VtableHook* client_hook;
static utils::VtableHook* input_hook;
static HANDLE thread = nullptr;

static std::set<CBaseHandle> items;
static std::set<CBaseHandle> illusions;
void __fastcall CHudHealthBars_Paint(void* thisptr, int edx, void* guipaintsurface);
void __fastcall LevelInitPreEntity(void* thisptr, int edx, char const* pMapName );
bool __fastcall IsKeyDown(void* thisptr, int edx, int key_code );

static bool active_thread = true;
bool simulate_shift_down = false;
DWORD WINAPI LastHitThread( LPVOID lpArguments );



DWORD WINAPI InitializeHook( LPVOID lpArguments ) {
  while(utils::GetModuleHandleSafe("engine.dll" ) == nullptr
    ||  utils::GetModuleHandleSafe( "client.dll" ) == nullptr
    ||  utils::GetModuleHandleSafe( "steamclient.dll" ) == nullptr) {
    Sleep(100);
  }

  GlobalAddressRetriever::GetInstance();

  panel_hook = new utils::VtableHook(dota::Hud::GetInstance()->FindElement("CHudHealthBars"), 0x34);
  panel_hook->HookMethod(CHudHealthBars_Paint, 107);

  client_hook = new utils::VtableHook(GlobalInstanceManager::GetClient());
  client_hook->HookMethod(LevelInitPreEntity, 4);

  g_pCVar = (ICvar*)GlobalInstanceManager::GetCVar();
  Vgui_IInput* input = GlobalInstanceManager::GetVguiInput();
  
  input_hook = new utils::VtableHook(input);
  input_hook->HookMethod(IsKeyDown, 18);

  dota::CCommandBuffer::GetInstance()->SetWaitEnabled(true);

  dota::GameSystemsRetriever().DumpSystems();

  commands::Register();

  dota::DotaPlayerResource* player_resource = dota::DotaPlayerResource::GetPlayerResource();
  if (player_resource == nullptr) return 1;
  Msg("player_resource: 0x%x \n", player_resource);

  dota::DotaPlayer* local_player = (dota::DotaPlayer*)GlobalInstanceManager::GetClientTools()->GetLocalPlayer();
  if (local_player == nullptr) return 1;
  Msg("local_player: 0x%x \n", local_player);

  dota::BaseNPCHero* local_hero = (dota::BaseNPCHero*)GlobalInstanceManager::GetClientEntityList()->GetClientEntity(local_player->GetAssignedHero());
  if (local_hero == nullptr) return 1;
  Msg("local_hero: 0x%x \n", local_hero);

  // CreateThread( NULL, 0, LastHitThread, 0, 0, NULL);  

	return 1;
}

bool __fastcall IsKeyDown(void* thisptr, int edx, int key_code ) {
  typedef bool ( __thiscall* OriginalFunction )(void*, int code);
  if (key_code == 0x50 || key_code == 0x51) {
    if (simulate_shift_down) return true;
  }
  return input_hook->GetMethod<OriginalFunction>(18)(thisptr, key_code);
}

DWORD WINAPI LastHitThread( LPVOID lpArguments ) {
  do {
    dota::DotaPlayerResource* player_resource = dota::DotaPlayerResource::GetPlayerResource();
    if (player_resource == nullptr) {
      Sleep(500);
      continue;
    }

    dota::DotaPlayer* local_player = (dota::DotaPlayer*)GlobalInstanceManager::GetClientTools()->GetLocalPlayer();
    if (local_player == nullptr) {
      Sleep(500);
      continue;
    }

    int local_player_id = local_player->GetPlayerId();
    int local_team =  player_resource->GetTeam(local_player_id);
    dota::BaseNPC *local_hero = (dota::BaseNPC*)GlobalInstanceManager::GetClientEntityList()->GetClientEntity(local_player->GetAssignedHero());

    for (int i = 1; i < GlobalInstanceManager::GetClientEntityList()->GetHighestEntityIndex(); i++ ) {
      dota::BaseEntity *base_entity = GlobalInstanceManager::GetClientEntityList()->GetClientEntity(i);
      if (base_entity == nullptr) continue;

      const char* class_name = base_entity->GetClientClass()->GetName();

      if (strcmp(class_name, "CDOTA_BaseNPC_Creep_Lane") == 0) { // CDOTA_BaseNPC_Creep
        dota::BaseNPC* creep = (dota::BaseNPC*) base_entity;

        if (creep && local_hero->IsEntityLastHittable(creep) && local_hero->IsEntityInRange(creep, local_hero->GetAttackRange())) {
          local_player->PrepareUnitOrders(4, i);
          Sleep(100);
        }
      }
    }
    Sleep(50);
  } while (active_thread);
  return 1;
}

void __fastcall CHudHealthBars_Paint(void* thisptr, int edx, void* guipaintsurface) {
  typedef void ( __thiscall* OriginalFunction )(PVOID, PVOID);
  panel_hook->GetMethod<OriginalFunction>(107)(thisptr, guipaintsurface);

  int offset_counter = 0;

  dota::DotaPlayerResource* player_resource = dota::DotaPlayerResource::GetPlayerResource();
  if (player_resource == nullptr) return;

  dota::DotaPlayer* local_player = (dota::DotaPlayer*)GlobalInstanceManager::GetClientTools()->GetLocalPlayer();
  if (local_player == nullptr) return;

  dota::BaseNPCHero* local_hero = (dota::BaseNPCHero*)GlobalInstanceManager::GetClientEntityList()->GetClientEntity(local_player->GetAssignedHero());
  if (local_hero == nullptr) return;

  dota::DotaChat* chat = dota::DotaChat::GetInstance();
  if (chat == nullptr) return;

  int local_player_id = local_player->GetPlayerId();
  int local_team =  player_resource->GetTeam(local_player_id);

  for (int i = 1; i < GlobalInstanceManager::GetClientEntityList()->GetHighestEntityIndex(); i++ ) {
    dota::BaseEntity *base_entity = GlobalInstanceManager::GetClientEntityList()->GetClientEntity(i);
    if (base_entity == nullptr) continue;

    const char* class_name = base_entity->GetClientClass()->GetName();
    if (class_name == nullptr) continue;

    if (StringHasPrefix(class_name, "CDOTA_Unit_Hero_")) {
      dota::BaseNPCHero* hero = (dota::BaseNPCHero*)base_entity;
      if (hero == nullptr) continue;

      CBaseHandle hero_handle = hero->GetRefEHandle();

      if (hero->IsIllusion() && illusions.count(hero_handle) == 0) {
        dota::CNewParticleEffect* ghost_effect = hero->GetParticleProp()->Create("ghost", 1, -1);
        Vector vector2(200, 0, 0);
        ghost_effect->SetControlPoint(1, vector2);
        illusions.insert(hero_handle);
      }
    }

    if (V_stricmp("CDOTAPlayer", class_name) == 0) { // CDOTAPlayer
      dota::DotaPlayer *dota_player = (dota::DotaPlayer *) base_entity;
      int player_id = dota_player->GetPlayerId();

      if (player_id < 0 || player_id > 10) return;

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

      dota::BaseNPCHero* hero = (dota::BaseNPCHero*)GlobalInstanceManager::GetClientEntityList()->GetClientEntity(dota_player->GetAssignedHero());
      if (hero == nullptr) continue;

      if (team != local_team) {
        dota::UnitInventory* inventory = hero->GetInventory();
        if (inventory == nullptr) continue;

        for (int i = 0; i < 6; i++ ) {
          dota::DotaItem* item = inventory->GetItemInSlot(i);
          if (item == nullptr) continue;

          CBaseHandle item_handle = item->GetRefEHandle();

          if (items.count(item_handle) == 0) {
            dota::CDotaGameManager* game_manager = dota::CDotaGameManager::GetInstance();
            if (game_manager == nullptr) return;

            KeyValues* item_data = game_manager->GetItemDataByItemID(item->GetItemId());

            const char* hero_name = dota::DotaPlayerResource::GetPlayerSelectedHero(player_id);
            hero_name = StringAfterPrefix(hero_name, "npc_dota_hero_");
            const char* item_name = item_data->GetString("AbilityName");
            item_name = StringAfterPrefix(item_name, "item_");
            dota::DotaSFHudOverlay::GetInstance()->ShowSpecItemPickup(hero_name, item_name);

            items.insert(item_handle);
          } 
        }
      }

      int health = hero->GetHealth();

      float manaMax = hero->GetMaxMana();
      if (manaMax == 0) continue;
      float mana = hero->GetMana();

      int barWidth = 100;
      int lifeWidth = (mana * 100 / manaMax);

      Vector vecScreen;
      int xpos, ypos;

      if (sourcesdk::DrawUtils::GetInstance().GetVectorInScreenSpace(hero->GetAbsOrigin(), xpos, ypos)) {
        sourcesdk::DrawUtils::GetInstance().DrawRect(xpos-45,ypos-20-25,barWidth,10,0,0,0,255);
        sourcesdk::DrawUtils::GetInstance().DrawRect(xpos-45,ypos-20-25,lifeWidth,10,0,128,255,255);
        sourcesdk::DrawUtils::GetInstance().OutlineRect(xpos-45-1,ypos-20-25-1,barWidth+2,10+2,0,0,0,255);
        sourcesdk::DrawUtils::GetInstance().DrawString(xpos-45+(barWidth/2),ypos-23-25, 255,255,255,255, true, "%d / %d",(int)mana,(int)manaMax);
        sourcesdk::DrawUtils::GetInstance().DrawString(xpos-45+(barWidth/2),ypos - 100, 255,255,255,255, true, "%d", health);
      }
    }
  }

}

void __fastcall LevelInitPreEntity(void* thisptr, int edx, char const* pMapName ) {
  typedef void ( __thiscall* OriginalFunction )(void*, char const*);
  client_hook->GetMethod<OriginalFunction>(4)(thisptr, pMapName);

  items.clear();
  illusions.clear();
}

void FinalizeHook() {

  if (panel_hook != nullptr) {
    panel_hook->Unhook();
    delete panel_hook;
  }
  if (client_hook != nullptr) {
    client_hook->Unhook();
    delete client_hook;
  }
  if (input_hook != nullptr) {
    input_hook->Unhook();
    delete input_hook;
  }
  active_thread = false;
  commands::Unregister();

  Sleep(500);
}

int WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved) {
  if( DLL_PROCESS_ATTACH == fdwReason ) {
    thread = CreateThread( NULL, 0, InitializeHook, 0, 0, NULL);  
  } else if (DLL_PROCESS_DETACH == fdwReason) {
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
    FinalizeHook();
  }
  return 1;
}