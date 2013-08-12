// Copyright 2013 Karl Skomski - GPL v3
#pragma once

#include "source-sdk/source_sdk.h"
#include "utils/utils.h"

class GlobalInstanceManager {
 public:
  static CHLClient* GetClient();
  static EntityList* GetClientEntityList();
  static EngineClient* GetEngineClient();
  static ISurface* GetSurface();
  static ISurfaceNew* GetSurfaceNew();
  static IPanel* GetPanel();
  static ClientTools* GetClientTools();
  static ICvar* GetCVar();
  static Vgui_IInput* GetVguiInput();
  static ILocalize* GetLocalize();
  static IGameEventManager2* GetGameEventManager();

 private:
  static CHLClient* chlclient_;
  static EntityList* centlist_;
  static EngineClient* engineclient_;
  static ISurface* surface_;
  static ISurfaceNew* surface_new_;
  static IPanel* panel_;
  static ClientTools* clienttools_;
  static ICvar* cvar_;
  static Vgui_IInput* vgui_input_;
  static ILocalize* localize_;
  static IGameEventManager2* game_event_manager_;
};
