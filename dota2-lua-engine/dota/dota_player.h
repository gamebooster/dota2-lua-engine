// Copyright 2013 Karl Skomski - GPL v3
#pragma once

#include "source-sdk/netvar.h"
#include "utils/global_address_retriever.h"
#include "dota/dota_baseentity.h"
#include "dota/dota_ability.h"

namespace dota {

class BaseNPC;

class BasePlayer : public BaseEntity {
 public:
  int GetPlayerId() {
    typedef int ( __thiscall* OriginalFn )(void* thisptr);
    return utils::GetVtableFunction<OriginalFn>(this, 282)(this);
  }
};

class DotaPlayer : public BasePlayer {
 public:
   enum Order {
     kMoveToPosition = 1,
     kMoveToEntity   = 2,
     kAttackPosition = 3,
     kAttackEntity   = 4,
     kUpdgradeAbility = 11,
     kHoldPosition = 10,
     kDoubleTapAbility = 8,
     kToggleAbility = 9,
     kStop = 21,
     kGlyph = 24,
     kBuyback = 23
   };

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

  void AttemptToUpgrade(DotaAbility* ability) {
    PrepareUnitOrders(Order::kUpdgradeAbility, 0, vec3_origin,
                      ability->GetIndex(), 0, 0, 0);
  }
  void MoveToPosition(Vector target) {
    PrepareUnitOrders(Order::kMoveToPosition, 0, target, 0, 0, 0, 0);
  }
  void MoveToEntity(BaseEntity* target) {
    PrepareUnitOrders(Order::kMoveToEntity, target->GetIndex(),
                      vec3_origin, 0, 0, 0, 0);
  }
  void AttackPosition(Vector target) {
    PrepareUnitOrders(Order::kAttackPosition, 0, target, 0, 0, 0, 0);
  }
  void AttackEntity(BaseEntity* target) {
    PrepareUnitOrders(Order::kAttackEntity, target->GetIndex(),
                      vec3_origin, 0, 0, 0, 0);
  }
  void HoldPosition() {
    PrepareUnitOrders(Order::kHoldPosition, 0, vec3_origin, 0, 0, 0, 0);
  }
  void Stop() {
    PrepareUnitOrders(Order::kStop, 0, vec3_origin, 0, 0, 0, 0);
  }
  void ToggleAbility(DotaAbility* ability) {
    PrepareUnitOrders(Order::kToggleAbility, 0, vec3_origin, ability->GetIndex(), 0, 0, 0);
  }
  void CreateDoubleTapCastOrder(DotaAbility* ability) {
    PrepareUnitOrders(Order::kDoubleTapAbility, 0, vec3_origin, ability->GetIndex(), 0, 0, 0);
  }
  void UseGlyph() {
    PrepareUnitOrders(Order::kGlyph, 0, vec3_origin, 0, 2, 0, 0);
  }
  void UseBuyback() {
    PrepareUnitOrders(Order::kBuyback, 0, vec3_origin, 0, 2, 0, 0);
  }

  void PrepareUnitOrders(int order, int entindex, Vector vec, int entindex_2,
                         int order_issuer, BaseNPC* base_npc, int unknown) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("DotaPlayer::PrepareUnitOrders");

    void  (__stdcall *PrepareUnitOrders)(void*, int, int, float, float,
                                         float, int, int, BaseNPC*, int) =
      reinterpret_cast<void(__stdcall *)(void*, int, int, float, float,
                                         float, int, int, BaseNPC*, int)>(address);

    PrepareUnitOrders(this, order, entindex, vec.x, vec.y, vec.z,
                      entindex_2, order_issuer, base_npc, unknown);
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
