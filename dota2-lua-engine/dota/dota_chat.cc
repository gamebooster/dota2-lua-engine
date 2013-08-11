#include "dota/dota_chat.h"

#include "utils/utils.h"
#include "source-sdk/source_sdk.h"
#include "dota/dota_global.h"

#include "utils/global_address_retriever.h"

namespace dota {

  DotaChat* DotaChat::GetInstance() {
    return (DotaChat*)Hud::GetInstance()->FindSFElement("CDOTA_SF_Hud_Chat");
  }

  void DotaChat::MessagePrintf(int area, wchar_t const* message, int player_index, int priority, float time) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("DotaChat::MessagePrintf");

    __asm {
      push time
      push player_index
      push message
      push area
      push this
      mov eax, priority
      call address
    }
  }

  void DotaChat::EventPrintf(wchar_t const* message, int unknown) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("DotaChat::EventPrintf");

    void (__stdcall *EventPrintInternal)(void*, wchar_t const*, int) =
      reinterpret_cast<void(__stdcall*)(void*, wchar_t const*, int)>(address);
    
    EventPrintInternal(this, message, unknown);
  }
}  // namespace dota
