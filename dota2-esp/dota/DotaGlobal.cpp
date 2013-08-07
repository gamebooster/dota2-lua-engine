#include "precompiled_headers.h"


#include "DotaGlobal.h"

const Vector vec3_invalid(FLT_MAX, FLT_MAX, FLT_MAX);
const Vector vec3_origin(0,0,0);

CUtlVector<IGameSystem*>* GameSystemsRetriever::game_systems_ = nullptr;

CDotaGameManager* CDotaGameManager::instance_ = nullptr;