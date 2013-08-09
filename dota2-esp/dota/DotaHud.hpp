#pragma once

#include "DotaItem.h"
#include "..\utils\global_address_retriever.hpp"

namespace dota {

class CHudElement {

};

class Hud {
public:
  static void SelectHudSkin(EconItemView* item, bool unknown0) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("Hud::SelectHudSkin");

    __asm {
      push unknown0
      mov esi, item
      call address
    }
  }
  static Hud* GetInstance() {
    if (instance_ == nullptr) {
      instance_ = (Hud*)GlobalAddressRetriever::GetInstance().GetDynamicAddress("Hud");
    }
    return instance_;
  }
  CHudElement* FindSFElement(const char* name) {
    int list = *(int*)(this + 0x44);


    for (int i = 0; i < 64; i++) {
      const char* current_name = *(const char**)(list + 0x10 + (0x18 * i));

      if (current_name == nullptr) break;
      if (strcmp(name, current_name) == 0) return *(CHudElement**)(list + 0x10 + (0x18 * i) + 0x4);
    }

    return nullptr;
  }
  CHudElement* FindElement(const char* name) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("Hud::FindElement");

    __asm {
      mov eax, name
      mov esi, this
      call address
    }
  }
 private:
  static Hud* instance_;
};

class DotaSFHudOverlay {
 public:
  static DotaSFHudOverlay* GetInstance() {
    if (instance_ == nullptr) {
      instance_ = (DotaSFHudOverlay*)Hud::GetInstance()->FindSFElement("CDOTA_SF_Hud_Overlay");
    }
    return instance_;
  }

  void ShowSpecItemPickup(const char* hero, const char* item) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("SFHudOverlay::ShowSpecItemPickup");

    __asm {
      mov eax, item
      push hero
      push this
      call address
    }
  }
  void SendRoshanPopup(bool alive, float time) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("SFHudOverlay::SendRoshanPopup");

    __asm {
      mov edi, this
      movss xmm0, time
      push alive
      call address
    }
  }
 private:
  static DotaSFHudOverlay* instance_;
};

}