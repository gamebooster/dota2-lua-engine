// Copyright 2013 Karl Skomski - GPL v3
#pragma once

#include "dota/dota_baseentity.h"
#include "dota/dota_units.h"

namespace dota {

class BasePlayer : public BaseEntity {
 public:
  int GetPlayerId() {
    typedef int ( __thiscall* OriginalFn )(void* thisptr);
    return utils::GetVtableFunction<OriginalFn>(this, 282)(this);
  }
};

class DotaPlayer : public BasePlayer {
 public:
  static BaseEntity* GetLocalPlayerSelectedUnit() {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("DotaPlayer::GetLocalPlayerSelectedUnit");

    dota::BaseEntity* (__cdecl *LocalPlayerSelectedUnit)() =
      reinterpret_cast<BaseEntity*(__cdecl *)()>(address);

    return LocalPlayerSelectedUnit();
  }
  int16 GetAssignedHero() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTAPlayer", "m_hAssignedHero");
    return *reinterpret_cast<int16*>(this + offset);
  }

  void PrepareUnitOrders(int order, int entity_index) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("DotaPlayer::PrepareUnitOrders");

    void  (__stdcall *PrepareUnitOrders)(void*, int, int, int, int,
                                         int, int, int, int, int) =
      reinterpret_cast<void(__stdcall *)(void*, int, int, int, int,
                                         int, int, int, int, int)>(address);

    PrepareUnitOrders(this, order, entity_index, 0, 0, 0, 0, 0, 0, 0);
  }
  void SetClickBehaviour(int order) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("DotaPlayer::SetClickBehaviour");


    __asm {
      push 0
      mov eax, order
      mov ecx, this
      call address
    }
  }
};

}  // namespace dota
