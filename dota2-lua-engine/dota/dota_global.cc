// Copyright 2013 Karl Skomski - GPL v3
#include "dota/dota_global.h"

namespace dota {
  CUtlVector<IGameSystem*>* GameSystemsRetriever::game_systems_ = nullptr;
  CDotaGameManager* CDotaGameManager::instance_ = nullptr;
}  // namespace dota
