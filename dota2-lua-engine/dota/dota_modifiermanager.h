// Copyright 2013 Karl Skomski - GPL v3
#pragma once

#include "utils/global_address_retriever.h"

namespace dota {

class ModifierManager {
 public:
  int GetModifierDefaultParams() {
    return GlobalAddressRetriever::GetInstance()
      .GetDynamicAddress("ModifierDefaultParams");
  }
  float GetModifier_Constant_Additive_Internal(int modifier_index) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress(
        "ModifierManager::GetModifier_Constant_Additive_Internal");

    float constant = 0;
    int default_params_address = GetModifierDefaultParams();

    __asm {
      push default_params_address
      mov eax, modifier_index
      mov edx, this
      call address
      movss [constant], xmm0
    }

    return constant;
  }
};

}  // namespace dota
