#pragma  once
#include "..\precompiled_headers.h"

#include "..\utils\utils.h"

class EconItemView {
public:
  void Init(int item_id, int unknown0, int unknown1, int unknown2) {
    if (init_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\x57\xE8\x00\x00\x00\x00\x8B\xB3\x00\x00\x00\x00"),
        "xx????xx????");

      init_address_ = utils::GetAbsoluteAddress(pattern_address + 0x1);
    }

    __asm {
      push unknown2
      push unknown1
      push unknown0
      push item_id
      mov esi, this
      call init_address_
    }
  }
  static EconItemView* Create() {
    if (constructor_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\x8B\xF0\xE8\x00\x00\x00\x00\x89\x45\x10\xEB\x07"),
        "xxx????xxxxx");

      constructor_address_ = utils::GetAbsoluteAddress(pattern_address + 0x2);
    }
    
    void* store = malloc(0x100);
    __asm {
      mov esi, store
      call constructor_address_
    }

    return (EconItemView*)store;
  }
private:
  static unsigned long constructor_address_;
  static unsigned long init_address_;
};