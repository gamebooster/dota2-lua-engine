#include "global_instance_manager.h"

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