#pragma once

#include "..\source-sdk\SDK.h"

#include "..\source-sdk\global_instance_manager.h"
#include "..\utils\global_address_retriever.hpp"

#include "DotaItem.h"
#include "DotaHud.hpp"
#include "DotaGameRules.hpp"
#include "DotaPlayer.hpp"
#include "DotaParticle.hpp"

class GameSystemsRetriever {
public:
  GameSystemsRetriever() {
    game_systems_ = (CUtlVector<IGameSystem*>*)GlobalAddressRetriever::GetInstance().GetDynamicAddress("GameSystemsRetriever");
  }

  IGameSystem* FindByName(const char* name) {
    for (int i = 0; i < game_systems_->Count(); i++) {
      if (strcmp((*game_systems_)[i]->Name(), name) == 0) return (*game_systems_)[i];
    }
    return nullptr;
  }
  void DumpSystems() {
    for (int i = 0; i < game_systems_->Count(); i++) {
      utils::Log("hook: %s 0x%x", (*game_systems_)[i]->Name(), (*game_systems_)[i]);
    }
  }
private:
  static CUtlVector<IGameSystem*>* game_systems_;
};

class CDotaGameManager {
public:
  static CDotaGameManager* GetInstance() {
    if (instance_ == nullptr) {
      instance_ = (CDotaGameManager*)GameSystemsRetriever().FindByName("CDOTAGameManager");
    }
    return instance_;
  }

  KeyValues* GetItemDataByItemID(int item_id) {
    KeyValues* items = *(KeyValues**)(this + 0x1c);

    if (items == nullptr) return nullptr;
    KeyValues* sub = nullptr;
    for (sub = items->GetFirstSubKey(); ; sub = sub->GetNextKey() ) {
      if (sub == nullptr) break;
      if (sub->GetInt("ID", 0) == item_id ) return sub;
    }
    return nullptr;
  }
private:
  static CDotaGameManager* instance_;
};

class CCommandBuffer {
public:
  static CCommandBuffer* GetInstance() {
    return (CCommandBuffer*)GlobalAddressRetriever::GetInstance().GetDynamicAddress("CommandBuffer");
  }

  void SetWaitDelayTime(int delay) {
    *(int*)(this + 0x8040) = delay;
  }
  void SetWaitEnabled(bool enabled) {
    *(bool*)(this + 0x8049) = enabled;
  }
};