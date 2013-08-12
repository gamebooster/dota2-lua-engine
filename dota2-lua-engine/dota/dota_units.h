// Copyright 2013 Karl Skomski - GPL v3
#pragma once

#include "utils/global_address_retriever.h"
#include "dota/dota_particle.h"
#include "dota/dota_modifiermanager.h"
#include "dota/dota_constants.h"
#include "dota/dota_player.h"
#include "dota/dota_baseentity.h"

namespace dota {

class DotaItem : public BaseEntity {
 public:
  const char* GetName() {
    int offset = 0x778;
    return *reinterpret_cast<const char**>(
      *reinterpret_cast<int*>(this + offset));
  }
  int GetItemId() {
    int offset = 0x778;
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

class UnitInventory {
 public:
  DotaItem* GetItemInSlot(int index) {
    int offset = 0x18;
    int handle = *reinterpret_cast<int*>(this + offset + index * 4);
    return reinterpret_cast<DotaItem*>(
      GlobalInstanceManager::GetClientEntityList()
        ->GetClientEntityFromHandle(handle));
  }
};

class BaseNPC : public BaseEntity {
 public:
  ParticleProperty* GetParticleProp() {
    void* prop = reinterpret_cast<void*>(this + 0x10);
    typedef ParticleProperty* ( __thiscall* OriginalFn )(void* thisptr);
    return utils::GetVtableFunction<OriginalFn>(prop, 0)(prop);
  }
  float GetEffectiveInvisibilityLevel() {
    typedef float ( __thiscall* OriginalFn )(void* thisptr);
    return utils::GetVtableFunction<OriginalFn>(this, 277)(this);
  }

  bool IsEnemy(BaseEntity* entity) {
    DotaPlayer* local_player = reinterpret_cast<DotaPlayer*>(
      GlobalInstanceManager::GetClientTools()->GetLocalPlayer());
    int local_team = local_player->GetTeamIndex();
    if (local_team == dota::constants::kSpectatorTeam) return false;
    return local_player->GetTeamIndex() != entity->GetTeamIndex();
  }

  bool IsVisibleByEnemyTeam() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_BaseNPC", "m_iTaggedAsVisibleByTeam");
    return *reinterpret_cast<byte*>(this + offset) == 30;
  }
  int GetHealth() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_BaseNPC", "m_iHealth");
    return *reinterpret_cast<int*>(this + offset);
  }
  float GetVisibilityLevel() {
    int offset = 0x3160;
    return *reinterpret_cast<float*>(this + offset);
  }
  void SetVisibilityLevel(float level) {
    int offset = 0x3160;
    *reinterpret_cast<float*>(this + offset) = level;
    *reinterpret_cast<float*>(this + 0x118C) = level > 0 ? 1 : 0;
    GetEffectiveInvisibilityLevel();
  }
  float GetMana() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_BaseNPC", "m_flMana");
    return *reinterpret_cast<float*>(this + offset);
  }
  float GetMaxMana() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_BaseNPC", "m_flMaxMana");
    return *reinterpret_cast<float*>(this + offset);
  }
  UnitInventory* GetInventory() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_BaseNPC", "m_Inventory");
    return reinterpret_cast<UnitInventory*>(this + offset);
  }
  ModifierManager* GetModifierManager() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_BaseNPC", "m_ModifierManager");
    return reinterpret_cast<ModifierManager*>(this + offset);
  }
  float GetPhysicalArmor() {
    typedef float ( __thiscall* OriginalFn )(void* thisptr);
    return utils::GetVtableFunction<OriginalFn>(this, 261)(this);
  }

  bool IsEntityLastHittable(BaseNPC* entity) {
    int health = entity->GetHealth();
    if (health <= 0) return false;
    int damage = GetDamageMin() + GetBonusDamage();
    float reduced_damage =
      damage - ((damage * entity->GetPhysicalArmorReduction()) / 100);
    return health / reduced_damage < 1;
  }

  float GetPhysicalArmorReduction() {
    typedef float ( __thiscall* OriginalFn )(void* thisptr);
    float armor = utils::GetVtableFunction<OriginalFn>(this, 261)(this);
    if (armor <= 0) return 0;
    return ((0.06 * armor) / (1 + 0.06 * armor)) * 100;
  }
  int GetDamageMin() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_BaseNPC", "m_iDamageMin");
    return *reinterpret_cast<int*>(this + offset);
  }
  int GetDamageMax() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_BaseNPC", "m_iDamageMax");
    return *reinterpret_cast<int*>(this + offset);
  }
  int GetBonusDamage() {
    typedef int ( __thiscall* OriginalFn )(void* thisptr);
    return utils::GetVtableFunction<OriginalFn>(this, 283)(this);
  }
  float GetAttackRange() {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("BaseNpc::GetAttackRange");

    float range;

    __asm {
      mov esi, this
      call address
      movss [range], xmm0
    }

    return range;
  }
  bool IsEntityInRange(BaseEntity* base_enity, float distance) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("BaseNpc::IsEntityInRange");

    bool in_range;

    __asm {
      push distance
      mov esi, base_enity
      mov ecx, this
      call address
      mov in_range, al
    }

    return in_range;
  }
};

class UnitAnnouncer : public BaseNPC {
 public:
  void SetAnnouncerItem(EconItemView* item, bool unknown0) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("UnitAnnouncer::SetAnnouncerItem");

    __asm {
      mov ecx, item
      mov esi, this
      mov al, unknown0
      call address
    }
  }
};

class BaseNPCHero : public BaseNPC {
 public:
  bool IsIllusion() {
    int offset = sourcesdk::NetVarManager::GetInstance()
      .GetNetVarOffset("DT_DOTA_BaseNPC_Hero", "m_hReplicatingOtherHeroModel");
    return *reinterpret_cast<int*>(this + offset) != -1;
  }
};

}  // namespace dota
