// Copyright 2013 Karl Skomski - GPL v3
#pragma once

#include "dota/dota_item.h"
#include "utils/global_address_retriever.h"

namespace dota {

class CHudElement {};

class Hud {
 public:
  static Hud* GetInstance() {
    if (instance_ == nullptr) {
      instance_ = reinterpret_cast<Hud*>(
        GlobalAddressRetriever::GetInstance().GetDynamicAddress("Hud"));
    }
    return instance_;
  }
  CHudElement* FindSFElement(const char* name) {
    int list = *reinterpret_cast<int*>(this + 0x44);


    for (int i = 0; i < 64; i++) {
      const char* current_name = *(const char**)(list + 0x10 + (0x18 * i));

      if (current_name == nullptr) break;
      if (strcmp(name, current_name) == 0) {
        return *reinterpret_cast<CHudElement**>(
          list + 0x10 + (0x18 * i) + 0x4);
      }
    }

    return nullptr;
  }
  CHudElement* FindElement(const char* name) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("Hud::FindElement");

    __asm {
      push name
      mov ecx, this
      call address
    }
  }

 private:
  static Hud* instance_;
};

class DotaSFActionPanel {
public:
  static DotaSFActionPanel* GetInstance() {
    if (instance_ == nullptr) {
      instance_ = reinterpret_cast<DotaSFActionPanel*>(
        Hud::GetInstance()->FindSFElement("CDOTA_SF_Hud_ActionPanel"));
    }
    return instance_;
  }

  void LoadHUDSkin(EconItemView* item, int unknown) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("SFActionPanel::LoadHudSkin");

    __asm {
      push unknown
      push item
      mov ecx, this
      call address
    }
  }

private:
  static DotaSFActionPanel* instance_;
};

class DotaSFHudOverlay {
 public:
  static DotaSFHudOverlay* GetInstance() {
    if (instance_ == nullptr) {
      instance_ = reinterpret_cast<DotaSFHudOverlay*>(
        Hud::GetInstance()->FindSFElement("CDOTA_SF_Hud_Overlay"));
    }
    return instance_;
  }

  void ShowSpecItemPickup(const char* hero, const char* item) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("SFHudOverlay::ShowSpecItemPickup");

    __asm {
      push item
      push hero
      mov ecx, this
      call address
    }
  }
  void SendRoshanPopup(int alive, float time) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("SFHudOverlay::SendRoshanPopup");

    __asm {
      mov ecx, this
      movss xmm0, time
      push alive
      call address
    }
  }

 private:
  static DotaSFHudOverlay* instance_;
};

}  // namespace dota
