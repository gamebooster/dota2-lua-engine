#include "dota/dota_global.h"

namespace dota {
  CUtlVector<IGameSystem*>* GameSystemsRetriever::game_systems_ = nullptr;
  CDotaGameManager* CDotaGameManager::instance_ = nullptr;
}  // namespace dota
