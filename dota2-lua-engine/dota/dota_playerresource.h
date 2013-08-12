// Copyright 2013 Karl Skomski - GPL v3
#pragma once

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

     static const char* GetPlayerSelectedHero(int index);

     static DotaPlayerResource* GetPlayerResource();
  };
}