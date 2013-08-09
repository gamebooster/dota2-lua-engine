#include "precompiled_headers.h"

#include "source-sdk\SDK.h"

#include "dota\DotaPlayerResource.h"
#include "dota\DotaChat.h"
#include "dota\DotaParticleManager.hpp"
#include "dota\DotaGlobal.h"

#include "utils\utils.h"
#include "utils\vmthooks.h"

#include "source-sdk\draw_utils.h"
#include "source-sdk\game_event_listener.h"
#include "source-sdk\global_instance_manager.h"

#include "lua\lua_global.hpp"
#include "lua\lua_engine.hpp"

#include "commands.hpp"

static HANDLE thread = nullptr;
static utils::VtableHook* client_hook;

void __fastcall LevelInitPreEntity(void* thisptr, int edx, char const* pMapName );

DWORD WINAPI InitializeHook( LPVOID lpArguments ) {
  while(utils::GetModuleHandleSafe("engine.dll" ) == nullptr
    ||  utils::GetModuleHandleSafe( "client.dll" ) == nullptr
    ||  utils::GetModuleHandleSafe( "steamclient.dll" ) == nullptr) {
    Sleep(100);
  }

  // Preload addresses
  GlobalAddressRetriever::GetInstance();

  client_hook = new utils::VtableHook(GlobalInstanceManager::GetClient());
  client_hook->HookMethod(LevelInitPreEntity, 4);

  g_pCVar = (ICvar*)GlobalInstanceManager::GetCVar();
  commands::Register();  

	return 1;
}

void __fastcall LevelInitPreEntity(void* thisptr, int edx, char const* pMapName ) {
  typedef void ( __thiscall* OriginalFunction )(void*, char const*);
  client_hook->GetMethod<OriginalFunction>(4)(thisptr, pMapName);

  lua::LuaEngine::GetInstance().UnloadScripts();
}

void FinalizeHook() {
  if (client_hook != nullptr) {
    delete client_hook;
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