// Copyright 2013 Karl Skomski - GPL v3
#pragma  once

#include "utils/global_address_retriever.h"
#include "utils/utils.h"

namespace dota {

class EconItemView {
 public:
  void Init(int item_id, int unknown0, int unknown1, int unknown2) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("EconItemView::Init");

    __asm {
      push unknown2
      push unknown1
      push unknown0
      push item_id
      mov esi, this
      call address
    }
  }
  static EconItemView* Create() {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("EconItemView::Create");

    void* store = malloc(0x100);
    __asm {
      mov esi, store
      call address
    }

    return reinterpret_cast<EconItemView*>(store);
  }
};

}  // namespace dota
