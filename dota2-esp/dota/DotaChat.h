#include <functional>

namespace dota {
  class DotaChat {
  public:
    void EventPrintf(wchar_t* message);
    static DotaChat* GetInstance();
  private:
    static DotaChat* dota_chat_;
    static std::function<void(void*, wchar_t*, int)> event_printf_;

  };
}