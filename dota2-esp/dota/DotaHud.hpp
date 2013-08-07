#include "DotaItem.h"

class CHudElement {

};

class CHud {
public:
  static void SelectHudSkin(EconItemView* item, bool unknown0) {
    if (select_hud_skin_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x5F\x5E\x5B\x8B\xE5\x5D\xC2\x08\x00\x0F\xB6\x43\x0E"),
        "x????xxxxxxxxxxxxx");

      select_hud_skin_address_ = utils::GetAbsoluteAddress(pattern_address);
    }

    __asm {
      push unknown0
        mov esi, item
        call select_hud_skin_address_
    }
  }
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
  static unsigned long select_hud_skin_address_;
};

class CDotaSFHudOverlay {
public:
  static CDotaSFHudOverlay* GetInstance() {
    if (instance_ == nullptr) {
      instance_ = (CDotaSFHudOverlay*)CHud::GetInstance()->FindSFElement("CDOTA_SF_Hud_Overlay");
    }
    return instance_;
  }

  void ShowSpecItemPickup(const char* hero, const char* item) {
    if (show_spec_item_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\x51\xE8\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00"),
        "xx????x????x????x????");

      show_spec_item_address_ = utils::GetAbsoluteAddress(pattern_address + 0x1);
    }

    __asm {
      mov eax, item
        push hero
        push this
        call show_spec_item_address_
    }
  }
private:
  static CDotaSFHudOverlay* instance_;
  static unsigned long show_spec_item_address_;
};