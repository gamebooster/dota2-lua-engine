#pragma once

#include "DotaUnits.hpp"

class BasePlayer : public BaseEntity {
public:
  int GetPlayerId() {
    typedef int ( __thiscall* OriginalFn )( PVOID );
    return utils::GetVtableFunction<OriginalFn>(this, 282)(this);
  }
};

class DotaPlayer : public BasePlayer {
public:
  BaseEntity* GetSelectedUnit(int index) {
    if (*(int*)(this + 0x1A80) > index) {
      int entindex = *(int*)(*(int*)(this + 0x1A74) + index * 4);
      utils::Log("hook: %d", entindex);
      return GlobalInstanceManager::GetClientEntityList()->GetClientEntityFromHandle(entindex);
    }
    return nullptr;
  }
  static BaseEntity* GetLocalPlayerSelectedUnit() {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("DotaPlayer::GetLocalPlayerSelectedUnit");

    BaseEntity* (__cdecl *LocalPlayerSelectedUnit)() =
      reinterpret_cast<BaseEntity*(__cdecl *)()>(address);

    return LocalPlayerSelectedUnit();
  }
  int16 GetAssignedHero() {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTAPlayer","m_hAssignedHero");
    return *reinterpret_cast<int16*>(this + offset);
  }

  void ShowRingEffect(BaseNPC* npc, Vector const& vector, int unknown0, int unknown1) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("DotaPlayer::ShowRingEffect");

    __asm {
      push unknown1
      push unknown0
      push npc
      push this
      mov eax, vector
      call address
    }
  }
  void PrepareUnitOrders(int order, int entity_index) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("DotaPlayer::PrepareUnitOrders");

    void  (__stdcall *PrepareUnitOrders)(void*, int, int, int, int, int, int, int, int, int) =
      reinterpret_cast<void(__stdcall *)(void*, int, int, int, int, int, int, int, int, int)>(address);

    PrepareUnitOrders(this, order, entity_index, 0, 0, 0, 0, 0, 0, 0);
  }
  void SetClickBehaviour(int order) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("DotaPlayer::SetClickBehaviour");


    __asm {
      push 0
      mov eax, order
      mov ecx, this
      call address
    }
  }
};