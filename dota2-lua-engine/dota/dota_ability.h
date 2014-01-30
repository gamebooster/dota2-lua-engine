// Copyright 2013 Karl Skomski - GPL v3
#pragma once

#include "source-sdk/global_instance_manager.h"
#include "source-sdk/netvar.h"
#include "utils/global_address_retriever.h"
#include "dota/dota_particle.h"
#include "dota/dota_modifiermanager.h"
#include "dota/dota_constants.h"
#include "dota/dota_player.h"
#include "dota/dota_baseentity.h"

namespace dota {
  class BaseNPC;

class DotaAbility : public BaseEntity {
public:
  float GetCooldown() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTABaseAbility", "m_fCooldown");
    return *reinterpret_cast<float*>(this + offset);
  }
  int GetLevel() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTABaseAbility", "m_iLevel");
    return *reinterpret_cast<int*>(this + offset);
  }

  float GetOverrideCastPoint() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTABaseAbility", "m_flOverrideCastPoint");
    return *reinterpret_cast<float*>(this + offset);
  }
  float GetChannelStartTime() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTABaseAbility", "m_flChannelStartTime");
    return *reinterpret_cast<float*>(this + offset);
  }
  bool GetAutoCastState() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTABaseAbility", "m_bAutoCastState");
    return *reinterpret_cast<bool*>(this + offset);
  }
  int GetManaCost() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTABaseAbility", "m_iManaCost");
    return *reinterpret_cast<int*>(this + offset);
  }
  int GetCooldownLengthInteger() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTABaseAbility", "m_iCooldownLength");
    return *reinterpret_cast<int*>(this + offset);
  }
  float GetCooldownLengthFloat() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTABaseAbility", "m_flCooldownLength");
    return *reinterpret_cast<float*>(this + offset);
  }
  bool InAbilityPhase() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTABaseAbility", "m_bInAbilityPhase");
    return *reinterpret_cast<bool*>(this + offset);
  }
  bool IsActivated() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTABaseAbility", "m_bActivated");
    return *reinterpret_cast<bool*>(this + offset);
  }
  bool IsHidden() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTABaseAbility", "m_bHidden");
    return *reinterpret_cast<bool*>(this + offset);
  }
  int GetCastRange() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTABaseAbility", "m_iCastRange");
    return *reinterpret_cast<int*>(this + offset);
  }
  BaseNPC* GetCaster() {
    int handle = *reinterpret_cast<int*>(this + 0x148);
    if (handle == -1) return nullptr;

    return reinterpret_cast<BaseNPC*>(
      GlobalInstanceManager::GetClientEntityList()
        ->GetClientEntityFromHandle(handle));
  }
  int CanBeExecuted() {
    typedef int ( __thiscall* OriginalFn )(void* thisptr);
    return utils::GetVtableFunction<OriginalFn>(this, 200)(this);
  }
  int GetLevelSpecialValueFor(const char* key) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("DOTABaseAbility::GetLevelSpecialValueFor");

    if (key == nullptr) return 0;
    int level = GetLevel();
    if (level == 0) return 0;

    int value_struct = 0;
    __asm {
      push key
      mov eax, this
      call address
      mov [value_struct], eax
    }
    int special_value = *reinterpret_cast<int*>(
      value_struct + 0x18 + (level - 1) * 0x10);
    return special_value;
  }
  int GetAbilityType() {
    typedef int ( __thiscall* OriginalFn )(void* thisptr);
    return utils::GetVtableFunction<OriginalFn>(this, 204)(this);
  }
  void OnExecute(BaseEntity* target, int quick_cast) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("DOTABaseAbility::OnExecute");

    if (target == nullptr) return;

    __asm {
      push quick_cast
        push target
        mov eax, this
        call address
    }
  }
};

class DotaItem : public DotaAbility {
public:
  const char* GetName() {
    int offset = 0x768;
    return *reinterpret_cast<const char**>(
      *reinterpret_cast<int*>(this + offset));
  }
  int GetItemId() {
    int offset = 0x768;
    return *reinterpret_cast<int*>(
      (*reinterpret_cast<int*>(this + offset)) + 0x3c);
  }
  bool GetRequiresCharges() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_Item", "m_bRequiresCharges");
    return *reinterpret_cast<bool*>(this + offset);
  }
  int GetCurrentCharges() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_Item", "m_iCurrentCharges");
    return *reinterpret_cast<int*>(this + offset);
  }
  int GetInitialCharges() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_Item", "m_iInitialCharges");
    return *reinterpret_cast<int*>(this + offset);
  }
  int GetPurchaser() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_Item", "m_hPurchaser");
    return *reinterpret_cast<int*>(this + offset);
  }
  float GetPurchaseTime() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_Item", "m_flPurchaseTime");
    return *reinterpret_cast<float*>(this + offset);
  }
};

}  // namespace dota
