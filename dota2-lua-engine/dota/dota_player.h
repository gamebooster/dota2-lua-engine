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

struct UserCMD {
  uint32_t vtable; //0x0000 
  int command_number; //0x0004 
  int tick_count; //0x0008 
  float angle_x; //0x000C 
  float angle_y; //0x0010 
  float angle_z; //0x0014 
  float forwardmove; //0x0018 
  float sidemove; //0x001C 
  float upmove; //0x0020 
  int buttons; //0x0024 
  BYTE impulse; //0x0028 
  BYTE N02E2995E; //0x0029 
  BYTE N02E2DCC7; //0x002A 
  BYTE N02E2995F; //0x002B 
  int entindex_under_cursor; //0x002C 
  int entindex_selected; //0x0030 
  WORD N0299E18C; //0x0034 
  WORD N02B7B1CC; //0x0036 
  WORD N0299E18B; //0x0038 
  WORD N02B7CFAC; //0x003A 
  WORD N0299E18A; //0x003C 
  WORD N02B7C270; //0x003E 
  DWORD N0299E189;  // 0x0040 order begin 
  DWORD order_number; //0x0044 
  DWORD N0299E187; //0x0048 
  DWORD N0299E7AF; //0x004C 
  DWORD N0299BCBB; //0x0050 
  DWORD N0299BCBC; //0x0054 
  DWORD N0299BCBD; //0x0058 
  DWORD order_id; //0x005C 
  DWORD N0299BCBF; //0x0060 
  DWORD N0299BCC0; //0x0064 
  DWORD N0299BCC1; //0x0068 
  DWORD N0299BCC2; //0x006C 
  DWORD N0299BCC3; //0x0070 
  DWORD N0299BCC4; //0x0074 
  uint16_t camera_x; //0x0078 
  uint16_t camera_y; //0x007A 
  BYTE N0299BCC6; //0x007C 
  BYTE N02B6D3A6; //0x007D 
  BYTE hud_shop_mode; //0x007E 
  BYTE hud_stats_dropdown_category_index; //0x007F 
  BYTE hud_stats_dropdown_sort_method; //0x0080 
};

class DotaPlayer : public BasePlayer {
 public:
  enum Order {
    kMoveToPosition = 1,
    kMoveToEntity   = 2,
    kAttackPosition = 3,
    kAttackEntity   = 4,
    kUseAbilityPosition = 5,
    kUseAbilityEntity = 6,
    kUseAbilityUnknown = 7,
    kUseAbility = 8,
    kToggleAbility = 9,
    kHoldPosition = 10,
    kUpdgradeAbility = 11,
    kAutocast = 20,
    kStop = 21,
    kBuyback = 23,
    kGlyph = 24,
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
  void UseAbilityPosition(DotaAbility* ability, Vector target) {
    PrepareUnitOrders(Order::kUseAbilityPosition, 0, target,
      ability->GetIndex(), 3, ability->GetCaster(), 0);
  }
  void UseAbilityEntity(DotaAbility* ability, BaseEntity* target) {
    PrepareUnitOrders(Order::kUseAbilityEntity, target->GetIndex(), vec3_origin,
                      ability->GetIndex(), 3, ability->GetCaster(), 0);
  }
  void UseAbilityTree(DotaAbility* ability, BaseEntity* target) {
    PrepareUnitOrders(Order::kUseAbilityUnknown, target->GetIndex(),
                      vec3_origin, ability->GetIndex(), 0, 0, 0);
  }
  void ToggleAbility(DotaAbility* ability) {
    PrepareUnitOrders(Order::kToggleAbility, 0, vec3_origin,
                      ability->GetIndex(), 0, 0, 0);
  }
  void UseAbility(DotaAbility* ability) {
    PrepareUnitOrders(Order::kUseAbility, 0, vec3_origin,
                      ability->GetIndex(), 0, 0, 0);
  }
  void AbilityAutocast(DotaAbility* ability) {
    PrepareUnitOrders(Order::kAutocast, 0, vec3_origin, ability->GetIndex(),
                      0, 0, 0);
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
                                         float, int, int, BaseNPC*, int)>
                                         (address);

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
  void CreateMove(float input_sample_time, UserCMD* cmd) {
    typedef void ( __thiscall* OriginalFn )( PVOID, float, UserCMD*);
    utils::GetVtableFunction<OriginalFn>(this, 261)
      (this, input_sample_time, cmd);
  }
};

}  // namespace dota
