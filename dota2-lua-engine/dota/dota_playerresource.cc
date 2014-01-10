// Copyright 2013 Karl Skomski - GPL v3
#include "dota_playerresource.h"

#include "utils/utils.h"
#include "source-sdk/source_sdk.h"
#include "source-sdk/netvar.h"
#include "utils/global_address_retriever.h"
#include "source-sdk/global_instance_manager.h"

namespace dota {
  DotaPlayerResource* DotaPlayerResource::GetPlayerResource() {
    return *reinterpret_cast<DotaPlayerResource**>(
      GlobalAddressRetriever::GetInstance()
        .GetDynamicAddress("PlayerResource"));
  }

  int DotaPlayerResource::GetLevel(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_PlayerResource", "m_iLevel");
    return *reinterpret_cast<int*>(this + offset + index * 4);
  }
  int DotaPlayerResource::GetKills(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_PlayerResource", "m_iKills");
    return *reinterpret_cast<int*>(this + offset + index * 4);
  }
  int DotaPlayerResource::GetAssists(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_PlayerResource", "m_iAssists");
    return *reinterpret_cast<int*>(this + offset + index * 4);
  }
  int DotaPlayerResource::GetDeaths(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_PlayerResource", "m_iDeaths");
    return *reinterpret_cast<int*>(this + offset + index * 4);
  }
  int DotaPlayerResource::GetStreak(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_PlayerResource", "m_iStreak");
    return *reinterpret_cast<int*>(this + offset + index * 4);
  }
  int DotaPlayerResource::GetRespawnSeconds(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_PlayerResource", "m_iRespawnSeconds");
    return *reinterpret_cast<int*>(this + offset + index * 4);
  }
  int DotaPlayerResource::GetLastHitCount(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_PlayerResource", "m_iLastHitCount");
    return *reinterpret_cast<int*>(this + offset + index * 4);
  }
  int DotaPlayerResource::GetDenyCount(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_PlayerResource", "m_iDenyCount");
    return *reinterpret_cast<int*>(this + offset + index * 4);
  }
  int DotaPlayerResource::GetBuybackCooldownTime(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_PlayerResource", "m_flBuybackCooldownTime");
    return *reinterpret_cast<int*>(this + offset + index * 4);
  }

  int DotaPlayerResource::GetReliableGold(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_PlayerResource", "m_iReliableGold");
    return *reinterpret_cast<int*>(this + offset + index * 4);
  }

  int DotaPlayerResource::GetTeam(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_PlayerResource", "m_iPlayerTeams");
    return *reinterpret_cast<int*>(this + offset + index * 4);
  }

  int DotaPlayerResource::GetUnreliableGold(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_PlayerResource", "m_iUnreliableGold");
    return *reinterpret_cast<int*>(this + offset + index * 4);
  }

  DotaPlayer* DotaPlayerResource::GetPlayerByPlayerId(int id) {
    for (int i = 1; i < 32; i++) {
      DotaPlayer* player = reinterpret_cast<DotaPlayer*>(
        GlobalInstanceManager::GetClientEntityList()->GetClientEntity(i));
      if (player == nullptr) continue;
      if (player->GetPlayerId() == id) return player;
    }
    return nullptr;
  }

  const char* DotaPlayerResource::GetPlayerName(int index) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("DotaPlayerResource::GetPlayerName");

    const char* name = nullptr;

    __asm {
      mov edi, index

      push this
      call address
      mov name, eax
    }
    return name;
  }

  const char* DotaPlayerResource::GetPlayerSelectedHero(int index) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("DotaPlayerResource::GetPlayerSelectedHero");

    const char* name = nullptr;

    __asm {
      push index
      call address
      mov [name], eax
    }
    return name;
  }
}  // namespace dota
