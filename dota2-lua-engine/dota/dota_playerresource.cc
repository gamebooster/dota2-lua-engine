#include "precompiled_headers.h"

#include "dota_playerresource.h"

#include "utils/utils.h"
#include "source-sdk/source_sdk.h"
#include "source-sdk/netvar.h"
#include "utils/global_address_retriever.h"

namespace dota {
  DotaPlayerResource* DotaPlayerResource::GetPlayerResource() {
    return *(DotaPlayerResource**)GlobalAddressRetriever::GetInstance().GetDynamicAddress("PlayerResource");
  }

  int DotaPlayerResource::GetLevel(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_PlayerResource","m_iLevel");
    return *(int*)(this + offset + index * 4);
  }
  int DotaPlayerResource::GetKills(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_PlayerResource","m_iKills");
    return *(int*)(this + offset + index * 4);
  }
  int DotaPlayerResource::GetAssists(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_PlayerResource","m_iAssists");
    return *(int*)(this + offset + index * 4);
  }
  int DotaPlayerResource::GetDeaths(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_PlayerResource","m_iDeaths");
    return *(int*)(this + offset + index * 4);
  }
  int DotaPlayerResource::GetStreak(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_PlayerResource","m_iStreak");
    return *(int*)(this + offset + index * 4);
  }
  int DotaPlayerResource::GetRespawnSeconds(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_PlayerResource","m_iRespawnSeconds");
    return *(int*)(this + offset + index * 4);
  }
  int DotaPlayerResource::GetLastHitCount(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_PlayerResource","m_iLastHitCount");
    return *(int*)(this + offset + index * 4);
  }
  int DotaPlayerResource::GetDenyCount(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_PlayerResource","m_iDenyCount");
    return *(int*)(this + offset + index * 4);
  }
  int DotaPlayerResource::GetBuybackCooldownTime(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_PlayerResource","m_flBuybackCooldownTime");
    return *(int*)(this + offset + index * 4);
  }

  int DotaPlayerResource::GetReliableGold(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_PlayerResource","m_iReliableGold");
    return *(int*)(this + offset + index * 4);
  }

  int DotaPlayerResource::GetTeam(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_PlayerResource","m_iPlayerTeams");
    return *(int*)(this + offset + index * 4);
  }

  int DotaPlayerResource::GetUnreliableGold(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_PlayerResource","m_iUnreliableGold");
    return *(int*)(this + offset + index * 4);
  }

  const char* DotaPlayerResource::GetPlayerName(int index) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("DotaPlayerResource::GetPlayerName");

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
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("DotaPlayerResource::GetPlayerSelectedHero");

    const char* name = nullptr;

    __asm {
      mov eax, index
      call address
      mov [name], eax
    }
    return name;
  }
}  // namespace dota
