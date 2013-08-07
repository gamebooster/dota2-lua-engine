#pragma once

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