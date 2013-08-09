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

static utils::VtableHook* client_hook;
static utils::VtableHook* input_hook;
static HANDLE thread = nullptr;

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
  }

}

void __fastcall LevelInitPreEntity(void* thisptr, int edx, char const* pMapName ) {
  typedef void ( __thiscall* OriginalFunction )(void*, char const*);
  client_hook->GetMethod<OriginalFunction>(4)(thisptr, pMapName);

  illusions.clear();
}

void FinalizeHook() {
  if (client_hook != nullptr) {
    client_hook->Unhook();
    delete client_hook;
  }
  if (input_hook != nullptr) {
    input_hook->Unhook();
    delete input_hook;
  }

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