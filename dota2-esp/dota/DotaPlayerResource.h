#include <string>
#include <stdint.h>

namespace dota {
  class DotaPlayerResource {
   public:
     const char* GetPlayerName(int index);

     static DotaPlayerResource* GetPlayerResource();
   private:
    static DotaPlayerResource* player_resource_;
    static unsigned long get_player_name_address_;
  };
}