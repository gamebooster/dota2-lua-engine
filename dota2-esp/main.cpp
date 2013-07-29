#include <set>

#include "source-sdk\SDK.h"

#include "dota\DotaPlayerResource.h"
#include "dota\DotaChat.h"
#include "dota\DotaGlobal.h"

#include "utils\utils.h"
#include "utils\vmthooks.h"

#include "source-sdk\draw_utils.h"
#include "source-sdk\global_instance_manager.h"

#include "commands.h"

static utils::VMTManager* panel_hook;
static utils::VMTManager* client_hook;
static utils::VMTManager* input_hook;
static HANDLE thread = nullptr;

static std::set<CDotaItem*> items;
void __fastcall CHudHealthBars_Paint(void* thisptr, int edx, void* guipaintsurface);
void __fastcall LevelInitPreEntity(void* thisptr, int edx, char const* pMapName );
bool __fastcall IsKeyDown(void* thisptr, int edx, int key_code );

static bool active_thread = true;
bool simulate_shift_down = false;
DWORD WINAPI LastHitThread( LPVOID lpArguments );

DWORD WINAPI InitializeHook( LPVOID lpArguments ) {
	while(utils::GetModuleHandleSafe("engine.dll" ) == NULL
    &&  utils::GetModuleHandleSafe( "client.dll" ) == NULL
    &&  utils::GetModuleHandleSafe( "steamclient.dll" ) == NULL) {
		Sleep( 100 );
  }

  panel_hook = new utils::VMTManager(CHud::GetInstance()->FindElement("CHudHealthBars"), 0x34);
  panel_hook->HookMethod(CHudHealthBars_Paint, 107);

  client_hook = new utils::VMTManager(GlobalInstanceManager::GetClient());
  client_hook->HookMethod(LevelInitPreEntity, 4);

  g_pCVar = (ICvar*)GlobalInstanceManager::GetCVar();
  Vgui_IInput* input = GlobalInstanceManager::GetVguiInput();
  
  input_hook = new utils::VMTManager(input);
  input_hook->HookMethod(IsKeyDown, 18);

  CCommandBuffer::GetInstance()->SetWaitEnabled(true);

  GameSystemsRetriever().DumpSystems();

  commands::Register();

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

    CDotaPlayer* local_player = (CDotaPlayer*)GlobalInstanceManager::GetClientTools()->GetLocalPlayer();
    if (local_player == nullptr) {
      Sleep(500);
      continue;
    }

    int local_player_id = local_player->GetPlayerId();
    int local_team =  player_resource->GetTeam(local_player_id);
    CBaseNpc *local_hero = (CBaseNpc*)GlobalInstanceManager::GetClientEntityList()->GetClientEntity(local_player->GetAssignedHero());

    for (int i = 1; i < GlobalInstanceManager::GetClientEntityList()->GetHighestEntityIndex(); i++ ) {
      CBaseEntity *base_entity = GlobalInstanceManager::GetClientEntityList()->GetClientEntity(i);
      if (base_entity == nullptr) continue;

      const char* class_name = base_entity->GetClientClass()->GetName();

      if (strcmp(class_name, "CDOTA_BaseNPC_Creep_Lane") == 0) { // CDOTA_BaseNPC_Creep
        CBaseNpc* creep = (CBaseNpc*) base_entity;

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

  if (commands::dota_esp_draw.GetBool() == false) return;

  int offset_counter = 0;

  dota::DotaPlayerResource* player_resource = dota::DotaPlayerResource::GetPlayerResource();
  if (player_resource == nullptr) return;

  CBasePlayer* local_player = (CBasePlayer*)GlobalInstanceManager::GetClientTools()->GetLocalPlayer();
  if (local_player == nullptr) return;

  dota::DotaChat* chat = dota::DotaChat::GetInstance();
  if (chat == nullptr) return;

  int local_player_id = local_player->GetPlayerId();
  int local_team =  player_resource->GetTeam(local_player_id);

  for (int i = 1; i < 32; i++ ) {
    CBaseEntity *base_entity = GlobalInstanceManager::GetClientEntityList()->GetClientEntity(i);
    if (base_entity == nullptr) continue;

    const char* class_name = base_entity->GetClientClass()->GetName();

    if (class_name[1] == 'D' && class_name[5] == 'P' && class_name[8] == 'y') { // CDOTAPlayer
      CDotaPlayer *dota_player = (CDotaPlayer *) base_entity;
      int player_id = dota_player->GetPlayerId();

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

      CBaseNpcHero* hero = (CBaseNpcHero*)GlobalInstanceManager::GetClientEntityList()->GetClientEntity(dota_player->GetAssignedHero());
      if (hero == nullptr) continue;


      CUnitInventory* inventory = hero->GetInventory();
      if (inventory == nullptr) continue;

      for (int i = 0; i < 6; i++ ) {
        CDotaItem* item = inventory->GetItemInSlot(i);
        if (item == nullptr) continue;
        if (items.count(item) == 0) {
          std::wstring message = L" bought ";
          CDotaGameManager* game_manager = CDotaGameManager::GetInstance();
          if (game_manager == nullptr) return;
          KeyValues* item_data = game_manager->GetItemDataByItemID(item->GetItemId());
          if (item_data == nullptr) {
            message.append(utils::ConvertToWide(item->GetName()));
          } else {
            std::string name = "#DOTA_Tooltip_Ability_";
            name.append(item_data->GetString("AbilityName"));
            std::string::size_type found = name.find("recipe_");
            if (found != std::string::npos) {
              name.erase(found, 7);
              message.append(L"Recipe ");
            }
            message.append(GlobalInstanceManager::GetLocalize()->Find(name.c_str()));
          }
          chat->MessagePrintf(0, message.c_str(), player_id, 0, GlobalInstanceManager::GetEngineClient()->Time());

          items.insert(item);
        } 
      }
        
      int health = hero->GetHealth();

      float manaMax = hero->GetMaxMana();
      if (manaMax == 0) continue;
      float mana = hero->GetMana();

      int barWidth = 100;
      int lifeWidth = (int)(mana*100/manaMax);

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