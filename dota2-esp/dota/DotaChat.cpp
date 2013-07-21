#include "DotaChat.h"

#include "..\utils\utils.h"
#include <tchar.h>
#include <windows.h>

namespace dota {
  std::function<void(void*, wchar_t*, int)> DotaChat::event_printf_;
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

  void DotaChat::EventPrintf(wchar_t* message) {

      unsigned long EventPrintfAddress = (unsigned long)utils::FindPattern(
        _T("client.dll"),
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x33\xD2\x83\x7C\x24\x00\x00"),
        "x????xxxxx??");

      void (*ShowPopupMessage)(void*, wchar_t*, int) =
        reinterpret_cast<void(*)(void*, wchar_t*, int)>(utils::GetAbsoluteAddress(EventPrintfAddress));
      event_printf_ = reinterpret_cast<void(*)(void*, wchar_t*, int)>(utils::GetAbsoluteAddress(EventPrintfAddress));
    
    wchar_t dbgBuf[MAX_PATH];

   ShowPopupMessage(this, message, -1);
  }
}