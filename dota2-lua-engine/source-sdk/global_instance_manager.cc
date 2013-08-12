// Copyright 2013 Karl Skomski - GPL v3
#include "source-sdk/global_instance_manager.h"

static void* GetInterface(const char* module_name,
                          const char* interface_name) {
  CreateInterface_t factory = (CreateInterfaceFn)
    GetProcAddress(utils::GetModuleHandleSafe(module_name),
    "CreateInterface");
  return factory(interface_name, NULL);
}

CHLClient* GlobalInstanceManager::GetClient() {
  if (chlclient_ == nullptr) {
    chlclient_ = reinterpret_cast<CHLClient*>(
        GetInterface("client.dll", "VClient020"));
  }
  return chlclient_;
};
EntityList* GlobalInstanceManager::GetClientEntityList() {
  if (centlist_ == nullptr) {
    centlist_ = reinterpret_cast<EntityList*>(
      GetInterface("client.dll", "VClientEntityList003"));
  }
  return centlist_;
};
EngineClient* GlobalInstanceManager::GetEngineClient() {
  if (engineclient_ == nullptr) {
    engineclient_ = reinterpret_cast<EngineClient*>(
      GetInterface("engine.dll", "VEngineClient018"));
  }
  return engineclient_;
};
ISurface* GlobalInstanceManager::GetSurface() {
  if (surface_ == nullptr) {
    surface_ = reinterpret_cast<ISurface*>(
      GetInterface("vguimatsurface.dll", "VGUI_Surface031"));
  }
  return surface_;
};
ISurfaceNew* GlobalInstanceManager::GetSurfaceNew() {
  if (surface_new_ == nullptr) {
    surface_new_ = reinterpret_cast<ISurfaceNew*>(GetSurface() + 0x10);
  }
  return surface_new_;
};
IPanel* GlobalInstanceManager::GetPanel() {
  if (panel_ == nullptr) {
    panel_ =  reinterpret_cast<IPanel*>(
      GetInterface("vgui2.dll", "VGUI_Panel009"));
  }
  return panel_;
};
ClientTools* GlobalInstanceManager::GetClientTools() {
  if (clienttools_ == nullptr) {
    clienttools_ = reinterpret_cast<ClientTools*>(
      GetInterface("client.dll", "VCLIENTTOOLS001"));
  }
  return clienttools_;
};

ICvar* GlobalInstanceManager::GetCVar() {
  if (cvar_ == nullptr) {
    cvar_ = reinterpret_cast<ICvar*>(
      GetInterface("vstdlib.dll", "VEngineCvar007"));
  }
  return cvar_;
};

Vgui_IInput* GlobalInstanceManager::GetVguiInput() {
  if (vgui_input_ == nullptr) {
    vgui_input_ = reinterpret_cast<Vgui_IInput*>(
      GetInterface("vgui2.dll", "VGUI_Input005"));
  }
  return vgui_input_;
};

ILocalize* GlobalInstanceManager::GetLocalize() {
  if (localize_ == nullptr) {
    localize_ = reinterpret_cast<ILocalize*>(
      GetInterface("localize.dll", "Localize_001"));
  }
  return localize_;
};

IGameEventManager2* GlobalInstanceManager::GetGameEventManager() {
  if (game_event_manager_ == nullptr) {
    game_event_manager_ = reinterpret_cast<IGameEventManager2*>(
      GetInterface("engine.dll", "GAMEEVENTSMANAGER002"));
  }
  return game_event_manager_;
};

CHLClient* GlobalInstanceManager::chlclient_ = nullptr;
EntityList* GlobalInstanceManager::centlist_ = nullptr;
EngineClient* GlobalInstanceManager::engineclient_ = nullptr;
ISurface* GlobalInstanceManager::surface_ = nullptr;
ISurfaceNew* GlobalInstanceManager::surface_new_ = nullptr;
IPanel* GlobalInstanceManager::panel_ = nullptr;
ClientTools* GlobalInstanceManager::clienttools_ = nullptr;
ICvar* GlobalInstanceManager::cvar_ = nullptr;
Vgui_IInput* GlobalInstanceManager::vgui_input_ = nullptr;
ILocalize* GlobalInstanceManager::localize_ = nullptr;
IGameEventManager2* GlobalInstanceManager::game_event_manager_ = nullptr;
