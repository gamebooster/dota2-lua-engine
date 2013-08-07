#pragma once

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

  void ShowRingEffect(BaseNPC* npc, Vector const& vector, int unknown0, int unknown1) {
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
};