#pragma once

#include "..\source-sdk\SDK.h"
#include "..\source-sdk\netvar.h"
#include "..\utils\utils.h"
#include "..\source-sdk\global_instance_manager.h"
#include "DotaItem.h"

class CBaseEntity;
class CBaseNpc;
class CAnnouncer;
class GameSystemsRetriever;

class GameSystemsRetriever {
public:
  GameSystemsRetriever() {
    if (game_systems_ == nullptr) {
      const uint32_t* pattern_address =  utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\x8B\x0D\x00\x00\x00\x00\x8B\x0C\x99"),
        "xx????xxx",
        0x2);

      game_systems_ = (CUtlVector<IGameSystem*>*)pattern_address;
    }
  }

  IGameSystem* FindByName(const char* name) {
    for (int i = 0; i < game_systems_->Count(); i++) {
      if (strcmp((*game_systems_)[i]->Name(), name) == 0) return (*game_systems_)[i];
    }
    return nullptr;
  }
  void DumpSystems() {
    for (int i = 0; i < game_systems_->Count(); i++) {
      utils::Log("hook: %s 0x%x", (*game_systems_)[i]->Name(), (*game_systems_)[i]);
    }
  }
private:
  static CUtlVector<IGameSystem*>* game_systems_;
};
class CDotaGameManager {
public:
  static CDotaGameManager* GetInstance() {
    if (instance_ == nullptr) {
      instance_ = (CDotaGameManager*)GameSystemsRetriever().FindByName("CDOTAGameManager");
    }
    return instance_;
  }

  KeyValues* GetItemDataByItemID(int item_id) {
    KeyValues* items = *(KeyValues**)(this + 0x1c);

    if (items == nullptr) return nullptr;
    KeyValues* sub = nullptr;
    for (sub = items->GetFirstSubKey(); ; sub = sub->GetNextKey() ) {
      if (sub == nullptr) break;
      if (sub->GetInt("ID", 0) == item_id ) return sub;
    }
    return nullptr;
  }
private:
  static CDotaGameManager* instance_;
};

class CCommandBuffer {
public:
  static CCommandBuffer* GetInstance() {
    if (instance_ == nullptr) {
      const uint32_t* pattern_address =  utils::FindPattern(
        "engine.dll",
        reinterpret_cast<unsigned char*>("\x81\xC7\x00\x00\x00\x00\x8B\x87\x00\x00\x00\x00"),
        "xx????xx????",
        0x2);

      instance_ = (CCommandBuffer*)pattern_address;
    }
    return instance_;
  }

  void SetWaitDelayTime(int delay) {
    *(int*)(this + 0x8040) = delay;
  }
  void SetWaitEnabled(bool enabled) {
    *(bool*)(this + 0x8049) = enabled;
  }
private:
  static CCommandBuffer* instance_;
};

class CBasePlayer : public CBaseEntity {
public:
  int GetPlayerId() {
    typedef int ( __thiscall* OriginalFn )( PVOID );
    return utils::GetVtableFunction<OriginalFn>(this, 282)(this);
  }
};

class CHudElement {

};

class CGameRules {
public:
  static CGameRules* GetInstance() {
    if (instance_ == nullptr) {
      const uint32_t* pattern_address =  utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\x8B\x0D\x00\x00\x00\x00\x85\xC9\x74\x08\x8B\x01\x8B\x10\x6A\x01\xFF\xD2\x56\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00"),
        "xx????xxxxxxxxxxxxxxx????????",
        0x2);

      instance_ = *(CGameRules**)pattern_address;
    }
    return instance_;
  }
  float GetGameTime() {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffsetL("DT_DOTAGamerulesProxy", "dota_gamerules_data", "m_fGameTime");
    return *(float*)(this + offset);
  }
  CAnnouncer* GetAnnouncer(int team_index) {
    if (get_announcer_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x85\xC0\x74\x1F\x38\x98\x00\x00\x00\x00"),
        "x????xxxxxx????");

      get_announcer_address_ = utils::GetAbsoluteAddress(pattern_address);
    }

    CAnnouncer* announcer;

    __asm {
      mov ecx, team_index
      mov eax, this
      call get_announcer_address_
      mov announcer, eax
    }

    return announcer;
  }
  CAnnouncer* GetKillingSpreeAnnouncer(int team_index) {
    if (get_announcer_killingspree_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\xEB\x3B\x8B\x0D\x00\x00\x00\x00"),
        "x????xxxx????");

      get_announcer_killingspree_address_ = utils::GetAbsoluteAddress(pattern_address);
    }

    CAnnouncer* announcer;

    __asm {
      mov ecx, team_index
        mov eax, this
        call get_announcer_killingspree_address_
        mov announcer, eax
    }

    return announcer;
  }
private:
  static CGameRules* instance_;
  static unsigned long get_announcer_address_;
  static unsigned long get_announcer_killingspree_address_;
};

class CHud {
public:
  static CHud* GetInstance() {
    if (instance_ == nullptr) {
      const uint32_t* pattern_address =  utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\x57\xBE\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x56"),
        "xx????x????x",
        0x2);

      instance_ = (CHud*)pattern_address;
    }
    return instance_;
  }
  CHudElement* FindElement(const char* name) {
    if (find_element_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\x89\x15\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x4D\xFC"),
        "xx????x????xxx");

      find_element_address_ = utils::GetAbsoluteAddress(pattern_address + 0x6);
    }

    __asm {
      mov eax, name
      mov esi, this
      call find_element_address_
    }
  }
private:
  static CHud* instance_;
  static unsigned long find_element_address_;
};

class CMinimap {
public:
  static void CreateLocationPing(Vector vector, int unknown) {
    if (create_location_ping_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x83\xC4\x10\xB0\x01\x8B\xE5"),
        "x????xxxxxxx");

      create_location_ping_address_ = utils::GetAbsoluteAddress(pattern_address);
    }


    float x = vector.x;
    float y = vector.y;
    float z = vector.z;

    __asm {
      push unknown
      push z
      push y
      push x
      call create_location_ping_address_
    }
  }
private:
  static unsigned long create_location_ping_address_;
};

class CModifierManager {
public:
  int GetModifierDefaultParams() {
    if (modifier_default_params_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\x74\x4E\x68\x00\x00\x00\x00\x8D\x55\xF4"),
        "xxx????xxx",
        0x3);

      modifier_default_params_ = pattern_address;
    }

    return modifier_default_params_;
  }
  float GetModifier_Constant_Additive_Internal(int modifier_index) {
    if (get_modifier_constant_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x83\xBF\x00\x00\x00\x00\x00\x0F\x57\xC9"),
        "x????xx?????xxx");

      get_modifier_constant_address_ = utils::GetAbsoluteAddress(pattern_address);
    }

    float constant = 0;
    int default_params_address = GetModifierDefaultParams();

    __asm {
      push default_params_address
      mov eax, modifier_index
      mov edx, this
      call get_modifier_constant_address_
      movss [constant], xmm0
    }

    return constant;
  }
private:
  static unsigned long get_modifier_constant_address_;
  static unsigned long modifier_default_params_;
};


class ParticleManager {
public:
  static void PrecacheParticleSystem(const char* name) {
    if (precache_particle_system_adddress_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(    
        "client.dll",
        reinterpret_cast<unsigned char*>("\x8B\x75\x0C\xE8\x00\x00\x00\x00\x85\xFF\x74\x07"),
        "xxxx????xxxx");

      precache_particle_system_adddress_ = utils::GetAbsoluteAddress(pattern_address + 0x3);
      if (precache_particle_system_adddress_ == 0) {
        utils::Log("hook: PrecacheParticleSystemError");
        return;
      }
    }

    __asm {
      mov esi, name
      call precache_particle_system_adddress_
    }
  }

  void DestroyParticle(int particle_index, int stop_emission = 0) {
    if (destroy_particle_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\x50\xE8\x00\x00\x00\x00\xC7\x86\x00\x00\x00\x00\x00\x00\x00\x00\x5F"),
        "xx????xx????????x");

      destroy_particle_address_ = utils::GetAbsoluteAddress(pattern_address + 0x1);
      if (destroy_particle_address_ == 0) {
        utils::Log("hook: DestroyParticleError");
        return;
      }
    }

    __asm {
      mov ecx, this
      push stop_emission
      push particle_index
      call destroy_particle_address_
    }
  }

  int CreateParticle(const char* name, int unknown, CBaseEntity* entity) {
    if (create_particle_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x6A\x00\x6A\x00\x68\x00\x00\x00\x00\x6A\x01"),
        "x????xxxxx????xx");

      create_particle_address_ = utils::GetAbsoluteAddress(pattern_address);
      if (create_particle_address_ == 0) {
        utils::Log("hook: CreateParticleError");
        return 0;
      }
    }

    int particle_index = 0;

    __asm {
      mov eax, this 
      mov esi, [eax+48]
      lea edx, [esi+1]
      mov [eax+48], edx
      push entity
      push unknown
      push esi
      push eax
      mov eax, name
      call create_particle_address_
      mov [particle_index], eax
    }

    return particle_index;
  }
  void SetParticleControl(int particle_index, int unknown, Vector& vector) {
    if (set_particle_control_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x8B\x15\x00\x00\x00\x00\x52\xEB\x41"),
        "x????xx????xxx");

      set_particle_control_address_ = utils::GetAbsoluteAddress(pattern_address);
      if (set_particle_control_address_ == 0) {
        utils::Log("hook: SetParticleControlError");
        return;
      }
    }

    float x = vector.x;
    float y = vector.y;
    float z = vector.z;

    __asm {
      push z
      push y
      push x
      push unknown
      mov eax, particle_index
      mov edx, this
      call set_particle_control_address_
    }
  }
  void SetParticleControlEnt(int particle_index, int unknown0, void* entity, int unknown1, const char* attached, Vector& vector) {
    if (set_particle_ent_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x51\x8B\xC7\xE8\x00\x00\x00\x00\x8B\xCE"),
        "x????xx????xxxx????xx");

      set_particle_ent_address_ = utils::GetAbsoluteAddress(pattern_address);
      if (set_particle_ent_address_ == 0) {
        utils::Log("hook: SetParticleControlEntError");
        return;
      }
    }
    float x = vector.x;
    float y = vector.y;
    float z = vector.z;

    __asm {
      push z
      push y
      push x
      push attached
      push unknown1
      push entity
      push unknown0
      push particle_index
      push this
      call set_particle_ent_address_
    }
  }
  void ReleaseParticleIndex(int particle_index) {
    if (release_particle_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(    
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x8B\x44\x24\x34\x99"),
        "x????xxxxx");

      release_particle_address_ = utils::GetAbsoluteAddress(pattern_address);
      if (release_particle_address_ == 0) {
        utils::Log("hook: SetParticleControlEntError");
        return;
      }
    }

    __asm {
      push this
      mov eax, particle_index
      call release_particle_address_
    }
  }
  static ParticleManager* ParticleManager::GetInstance() {
    if (instance_ == nullptr) {
      const uint32_t* pattern_address =  utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\x8B\x3D\x00\x00\x00\x00\x8B\xCB\xFF\xD2"),
        "xx????xxxx",
        0x2);
        instance_ = *(ParticleManager**)pattern_address;
    }
    return instance_;
  }
private:
  static unsigned long create_particle_address_;
  static unsigned long set_particle_ent_address_;
  static unsigned long release_particle_address_;
  static unsigned long precache_particle_system_adddress_;
  static unsigned long set_particle_control_address_;
  static unsigned long destroy_particle_address_;
  static ParticleManager* instance_;
};

class CDotaItem : public CBaseEntity {
public:
  const char* GetName() {
    int offset = 0x778;
    return *(const char**)(*(int*)(this + offset));
  }
  int GetItemId() {
    int offset = 0x778;
    return *(int*)((*(int*)(this + offset)) + 0x3c);
  }
  bool GetRequiresCharges() {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_Item","m_bRequiresCharges");
    return *(bool*)(this + offset);
  }
  int GetCurrentCharges() {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_Item","m_iCurrentCharges");
    return *(int*)(this + offset);
  }
  int GetInitialCharges() {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_Item","m_iInitialCharges");
    return *(int*)(this + offset);
  }
  int GetPurchaser() {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_Item","m_hPurchaser");
    return *(int*)(this + offset);
  }
  float GetPurchaseTime() {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_Item","m_flPurchaseTime");
    return *(float*)(this + offset);
  }
};

class CUnitInventory {
public:
  CDotaItem* GetItemInSlot(int index) {
    int offset = 0x18;
    int handle = *(int*)(this + offset + index * 4);
    return (CDotaItem*)GlobalInstanceManager::GetClientEntityList()->GetClientEntityFromHandle(handle);
  }
};

class CAbility {

};

class CBaseNpc : public CBaseEntity {
public:
  int GetHealth() {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_BaseNPC","m_iHealth");
    return *(int*)(this + offset);
  }
  float GetMana() {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_BaseNPC","m_flMana");
    return *(float*)(this + offset);
  }
  float GetMaxMana() {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_BaseNPC","m_flMaxMana");
    return *(float*)(this + offset);
  }
  CUnitInventory* GetInventory() {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_BaseNPC","m_Inventory");
    return (CUnitInventory*)(this + offset);
  }
  CAbility* GetAbilityByIndex(int index) {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_BaseNPC","m_hAbilities");
    return (CAbility*)(this + offset + index * 4);
  }
  CModifierManager* GetModifierManager() {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_BaseNPC","m_ModifierManager");
    return (CModifierManager*)(this + offset);
  }
  float GetPhysicalArmor() {
    typedef float ( __thiscall* OriginalFn )( PVOID );
    return utils::GetVtableFunction<OriginalFn>(this, 261)(this);
  }

  bool IsEntityLastHittable(CBaseNpc* entity) {
    int health = entity->GetHealth();
    if (health <= 0) return false;
    int damage = GetDamageMin() + GetBonusDamage();
    float reduced_damage = damage - ((damage * entity->GetPhysicalArmorReduction()) / 100);
    return health / damage < 1;
  }

  float GetPhysicalArmorReduction() {
    typedef float ( __thiscall* OriginalFn )( PVOID );
    float armor = utils::GetVtableFunction<OriginalFn>(this, 261)(this);
    if (armor <= 0) return 0;
    return ((0.06 * armor) / (1 + 0.06 * armor)) * 100;
  }
  int GetDamageMin() {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_BaseNPC","m_iDamageMin");
    return *(int*)(this + offset);
  }
  int GetDamageMax() {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_BaseNPC","m_iDamageMax");
    return *(int*)(this + offset);
  }
  int GetBonusDamage() {
    typedef int ( __thiscall* OriginalFn )( PVOID );
    return utils::GetVtableFunction<OriginalFn>(this, 283)(this);
  }
  float GetAttackRange() {
    if (get_attack_range_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x51\x8B\xF3"),
        "x????xxx");

      get_attack_range_address_ = utils::GetAbsoluteAddress(pattern_address);
    }

    float range;

    __asm {
      mov esi, this
      call get_attack_range_address_
      movss [range], xmm0
    }

    return range;
  }
  bool IsEntityInRange(CBaseEntity* base_enity, float distance) {
    if (is_entity_in_range_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x84\xC0\x75\x36\x8B\x17"),
        "x????xxxxxx");

      is_entity_in_range_address_ = utils::GetAbsoluteAddress(pattern_address);
    }

    bool in_range;

    __asm {
      push distance
      mov esi, base_enity
      mov ecx, this
      call is_entity_in_range_address_
      mov in_range, al
    }

    return in_range;
  }
private:
  static unsigned long is_entity_in_range_address_;
  static unsigned long get_attack_range_address_;
};

class CAnnouncer : public CBaseNpc {
public:
  void SetAnnouncerItem(EconItemView* item, bool unknown0) {
    if (set_announcer_item_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\xEB\x2F\x68\x00\x00\x00\x00"),
        "x????xxx????");

      set_announcer_item_address_ = utils::GetAbsoluteAddress(pattern_address);
    }

    __asm {
      mov ecx, item
        mov esi, this
        mov al, unknown0
        call set_announcer_item_address_
    }
  }
private:
  static unsigned long set_announcer_item_address_;
};

class CBaseNpcHero : public CBaseNpc {
public:
  bool IsIllusion() {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTA_BaseNPC_Hero","m_hReplicatingOtherHeroModel");
    return *(int*)(this + offset) <= 0;
  }
};

class CDotaPlayer : public CBasePlayer {
public:
  CBaseEntity* GetSelectedUnit(int index) {
    if (*(int*)(this + 0x1A80) > index) {
      int entindex = *(int*)(*(int*)(this + 0x1A74) + index * 4);
      utils::Log("hook: %d", entindex);
      return GlobalInstanceManager::GetClientEntityList()->GetClientEntityFromHandle(entindex);
    }
    return nullptr;
  }
  static CBaseEntity* GetLocalPlayerSelectedUnit() {
    if (get_local_selected_unit_address == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x85\xC0\x74\x50\x53"),
        "x????xxxxx");

      get_local_selected_unit_address = utils::GetAbsoluteAddress(pattern_address);
    }

    CBaseEntity* (__cdecl *LocalPlayerSelectedUnit)() =
      reinterpret_cast<CBaseEntity*(__cdecl *)()>(get_local_selected_unit_address);

    return LocalPlayerSelectedUnit();
  }
  int16 GetAssignedHero() {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTAPlayer","m_hAssignedHero");
    return *reinterpret_cast<int16*>(this + offset);
  }

  void ShowRingEffect(CBaseNpc* npc, Vector const& vector, int unknown0, int unknown1) {
    if (show_ring_effect_address == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\xEB\x78\x33\xD2"),
        "x????xxxx");

      show_ring_effect_address = utils::GetAbsoluteAddress(pattern_address);
    }

    __asm {
      push unknown1
      push unknown0
      push npc
      push this
      mov eax, vector
      call show_ring_effect_address
    }
  }
  void PrepareUnitOrders(int order, int entity_index) {
    if (prepare_unit_orders_address == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x83\xCF\xFF\xEB\x1E"),
        "x????xxxxx");

      prepare_unit_orders_address = utils::GetAbsoluteAddress(pattern_address);
    }

    void  (__stdcall *PrepareUnitOrders)(void*, int, int, int, int, int, int, int, int, int) =
      reinterpret_cast<void(__stdcall *)(void*, int, int, int, int, int, int, int, int, int)>(prepare_unit_orders_address);

    PrepareUnitOrders(this, order, entity_index, 0, 0, 0, 0, 0, 0, 0);
  }
  void SetClickBehaviour(int order) {
    if (set_click_behaviour_address == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x81\x4D\x00\x00\x00\x00\x00"),
        "x????xx?????");

      set_click_behaviour_address = utils::GetAbsoluteAddress(pattern_address);
    }


    __asm {
      push 0
      mov eax, order
      mov ecx, this
      call set_click_behaviour_address
    }
  }
private:
  static unsigned long prepare_unit_orders_address;
  static unsigned long get_local_selected_unit_address;
  static unsigned long set_click_behaviour_address;
  static unsigned long show_ring_effect_address;
};