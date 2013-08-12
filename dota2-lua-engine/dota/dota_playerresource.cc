// Copyright 2013 Karl Skomski - GPL v3
#include "dota_playerresource.h"

#include "utils/utils.h"
#include "source-sdk/source_sdk.h"
#include "source-sdk/netvar.h"
#include "utils/global_address_retriever.h"

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

  const char* DotaPlayerResource::GetPlayerName(int index) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("DotaPlayerResource::GetPlayerName");

    const char* name = nullptr;

    __asm {
      mov esi, this
      mov eax, index
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
      mov eax, index
      call address
      mov [name], eax
    }
    return name;
  }
}  // namespace dota
