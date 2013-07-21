#include "global_instance_manager.h"

CHLClient* GlobalInstanceManager::chlclient_ = nullptr;
CEntList* GlobalInstanceManager::centlist_ = nullptr;
EngineClient* GlobalInstanceManager::engineclient_ = nullptr;
ISurface* GlobalInstanceManager::surface_ = nullptr;
ISurfaceNew* GlobalInstanceManager::surface_new_ = nullptr;
IPanel* GlobalInstanceManager::panel_ = nullptr;
ClientTools* GlobalInstanceManager::clienttools_ = nullptr;