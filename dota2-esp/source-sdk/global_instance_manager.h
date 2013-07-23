#pragma once

#include "SDK.h"
#include "..\utils\utils.h"

class GlobalInstanceManager {
public:
  static CHLClient* GetClient() {
    if (chlclient_ == nullptr) {
      CreateInterface_t ClientFactory = ( CreateInterfaceFn ) GetProcAddress( utils::GetModuleHandleSafe( "client.dll" ), "CreateInterface" );
      chlclient_ = ( CHLClient* )ClientFactory( "VClient020", NULL);
    }
    return chlclient_;
  };
  static EntityList* GetClientEntityList() {
    if (centlist_ == nullptr) {
      CreateInterface_t ClientFactory = ( CreateInterfaceFn ) GetProcAddress( utils::GetModuleHandleSafe( "client.dll" ), "CreateInterface" );
      centlist_ = (EntityList*) ClientFactory( "VClientEntityList003", NULL );
    }
    return centlist_;
  };
  static EngineClient* GetEngineClient() {
    if (engineclient_ == nullptr) {
      CreateInterface_t EngineFactory = ( CreateInterfaceFn ) GetProcAddress( utils::GetModuleHandleSafe( "engine.dll" ), "CreateInterface" );
      engineclient_ = ( EngineClient* ) EngineFactory( "VEngineClient018", NULL );
    }
    return engineclient_;
  };
  static ISurface* GetSurface() {
    if (surface_ == nullptr) {
      CreateInterface_t VGUIFactory = ( CreateInterfaceFn ) GetProcAddress( utils::GetModuleHandleSafe( "vguimatsurface.dll" ), "CreateInterface" );
      surface_ = ( ISurface* ) VGUIFactory( "VGUI_Surface031", NULL );
    }
    return surface_;
  };
  static ISurfaceNew* GetSurfaceNew() {
    if (surface_new_ == nullptr) {
      surface_new_ = ( ISurfaceNew* )(GetSurface() + 0x10);
    }
    return surface_new_;
  };
  static IPanel* GetPanel() {
    if (panel_ == nullptr) {
      CreateInterface_t VGUI2Factory = ( CreateInterfaceFn ) GetProcAddress( utils::GetModuleHandleSafe( "vgui2.dll" ), "CreateInterface" );
      panel_ =  ( IPanel* ) VGUI2Factory( "VGUI_Panel009", NULL );
    }
    return panel_;
  };
  static ClientTools* GetClientTools() {
    if (clienttools_ == nullptr) {
      CreateInterface_t VClientFactory = ( CreateInterfaceFn ) GetProcAddress( utils::GetModuleHandleSafe( "client.dll" ), "CreateInterface" );
      clienttools_ =  ( ClientTools* ) VClientFactory( "VCLIENTTOOLS001", NULL );
    }
    return clienttools_;
  };

private:
  static CHLClient* chlclient_;
  static EntityList* centlist_;
  static EngineClient* engineclient_;
  static ISurface* surface_;
  static ISurfaceNew* surface_new_;
  static IPanel* panel_;
  static ClientTools* clienttools_;
};