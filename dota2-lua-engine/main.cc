// Copyright 2013 Karl Skomski - GPL v3

#include "source-sdk/source_sdk.h"

#include "utils/utils.h"
#include "utils/vmthooks.h"
#include "utils/global_address_retriever.h"

#include "source-sdk/global_instance_manager.h"

#include "lua/lua_engine.h"

#include "dota2-lua-engine/commands.h"
#include "dota/dota_global.h"

static HANDLE thread = nullptr;
static utils::VtableHook* client_hook;
ICvar *g_pCVar;

void __fastcall LevelInitPreEntity(void* thisptr, int edx, char const* map);

DWORD WINAPI InitializeHook(void* arguments) {
  while (utils::GetModuleHandleSafe("engine.dll") == nullptr
    ||  utils::GetModuleHandleSafe("client.dll") == nullptr
    ||  utils::GetModuleHandleSafe("steamclient.dll") == nullptr) {
    Sleep(100);
  }

  // Preload addresses
  GlobalAddressRetriever::GetInstance();

  client_hook = new utils::VtableHook(GlobalInstanceManager::GetClient());
  client_hook->HookMethod(LevelInitPreEntity, 4);

  g_pCVar = reinterpret_cast<ICvar*>(GlobalInstanceManager::GetCVar());
  commands::Register();

  dota::GameSystemsRetriever().DumpSystems();

  return 1;
}

void __fastcall LevelInitPreEntity(void* thisptr, int edx, char const* map) {
  typedef void ( __thiscall* OriginalFunction )(void*, char const*);
  client_hook->GetMethod<OriginalFunction>(4)(thisptr, map);

  lua::LuaEngine::GetInstance().UnloadScripts();
}

void FinalizeHook() {
  if (client_hook != nullptr) {
    delete client_hook;
  }
  lua::LuaEngine::GetInstance().UnloadScripts();
  commands::Unregister();
}

int WINAPI DllMain(HINSTANCE instance, DWORD reason, PVOID reserved) {
  if (reason == DLL_PROCESS_ATTACH) {
    thread = CreateThread(nullptr, 0, InitializeHook, 0, 0, nullptr);
  } else if (reason == DLL_PROCESS_DETACH) {
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
    FinalizeHook();
  }
  return 1;
}
