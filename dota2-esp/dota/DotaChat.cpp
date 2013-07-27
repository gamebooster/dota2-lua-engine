#include "DotaChat.h"

#include "..\utils\utils.h"
#include "..\source-sdk\SDK.h"

namespace dota {
  unsigned long DotaChat::event_printf_address_ = 0;
  unsigned long DotaChat::message_printf_address_ = 0;
  DotaChat* DotaChat::dota_chat_ = nullptr;

  DotaChat* DotaChat::GetInstance() {
    if (dota_chat_ != nullptr) return dota_chat_;

    const uint32_t* pattern_address =  utils::FindPattern(
      _T("client.dll"),
      reinterpret_cast<unsigned char*>("\x56\x8B\x35\x00\x00\x00\x00\x85\xF6\x74\x24"),
      "xxx????xxxx",
      0x3);

    dota_chat_ = *(dota::DotaChat**)pattern_address;
    return dota_chat_;
  }

  void DotaChat::MessagePrintf(int area, wchar_t const* message, int player_index, int priority, float time) {
    if (message_printf_address_ == 0) { 
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        _T("client.dll"),
        reinterpret_cast<unsigned char*>("\x33\xC0\xE8\x00\x00\x00\x00\x5F\xB0\x01"),
        "xxx????xxx");

      message_printf_address_ = utils::GetAbsoluteAddress(pattern_address + 0x2);
      if (message_printf_address_ == 0) {
        utils::Log("hook: MessagePrintfError");
        return;
      }
    }
    __asm {
      push time
      push player_index
      push message
      push area
      push this
      mov eax, priority
      call message_printf_address_
    }
  }

  void DotaChat::EventPrintf(wchar_t const* message, int unknown) {
    if (event_printf_address_ == 0) { 
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        _T("client.dll"),
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x33\xD2\x83\x7C\x24\x00\x00"),
        "x????xxxxx??");

      event_printf_address_ = utils::GetAbsoluteAddress(pattern_address);
      utils::Log("hook: 0x%x", event_printf_address_);
      if (event_printf_address_ == 0) {
        utils::Log("hook: EventPrintfError");
        return;
      }
    }
    void (__stdcall *EventPrintInternal)(void*, wchar_t const*, int) =
      reinterpret_cast<void(__stdcall*)(void*, wchar_t const*, int)>(event_printf_address_);
    
    EventPrintInternal(this, message, unknown);
  }
}