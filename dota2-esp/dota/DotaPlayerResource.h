#include <string>
#include <stdint.h>

namespace dota {
  class DotaPlayerResource {
   public:
     const char* GetPlayerName(int index);
     int GetLevel(int index);
     int GetKills(int index);
     int GetAssists(int index);
     int GetDeaths(int index);
     int GetStreak(int index);
     int GetRespawnSeconds(int index);
     int GetLastHitCount(int index);
     int GetDenyCount(int index);
     int GetBuybackCooldownTime(int index);
     int GetReliableGold(int index);
     int GetUnreliableGold(int index);
     int GetTeam(int index);

     static DotaPlayerResource* GetPlayerResource();
   private:
    static DotaPlayerResource* player_resource_;
    static unsigned long get_player_name_address_;
  };
}