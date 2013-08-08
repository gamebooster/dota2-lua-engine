#pragma once

#include "..\source-sdk\netvar.h"
#include "..\source-sdk\global_instance_manager.h"
#include "..\utils\global_address_retriever.hpp"
#include "DotaItem.h"
#include "DotaUnits.hpp"

namespace dota {

class GameRules {
public:
  static GameRules* GetInstance() {
    return *(GameRules**)GlobalAddressRetriever::GetInstance().GetDynamicAddress("GameRules");
  }
  float GetGameTime() {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffsetL("DT_DOTAGamerulesProxy", "dota_gamerules_data", "m_fGameTime");
    return *(float*)(this + offset);
  }
  UnitAnnouncer* GetAnnouncer(int team_index) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("GameRules::GetAnnouncer");
    UnitAnnouncer* announcer;

    __asm {
      mov ecx, team_index
        mov eax, this
        call address
        mov announcer, eax
    }

    return announcer;
  }
  UnitAnnouncer* GetKillingSpreeAnnouncer(int team_index) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("GameRules::GetKillingSpreeAnnouncer");

    UnitAnnouncer* announcer;

    __asm {
      mov ecx, team_index
        mov eax, this
        call address
        mov announcer, eax
    }

    return announcer;
  }
};

}