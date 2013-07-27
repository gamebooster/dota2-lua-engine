

namespace dota {
  class DotaChat {
  public:
    void EventPrintf(wchar_t const* message, int unknown);
    void MessagePrintf(int area, wchar_t const* message, int unknown0, int unknown1, float unknown2);
    static DotaChat* GetInstance();
  private:
    static DotaChat* dota_chat_;
    static unsigned long event_printf_address_;
    static unsigned long message_printf_address_;
  };
}