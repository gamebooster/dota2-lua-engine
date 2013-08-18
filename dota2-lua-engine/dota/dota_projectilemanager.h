// Copyright 2013 Karl Skomski - GPL v3
#pragma once

#include "dota/dota_global.h"
#include "dota/dota_baseentity.h"

namespace dota {
  struct Projectile {
    uint32_t speed;
    Vector position;
    int sourceHandle;
    int targetHandle;
    Vector target;
    byte flag0;
    byte flag1;
    byte flag2;
    byte flag3;
    uint32_t unknown1;
    uint32_t unknown2;
    dota::BaseTempEntity* entity;
  };
  class ProjectileManager {
   public:
    static ProjectileManager* GetInstance() {
      if (instance_ == nullptr) {
        instance_ = reinterpret_cast<ProjectileManager*>(
          GameSystemsRetriever().FindByName("C_DOTA_ProjectileManager"));
      }
      return instance_;
    }

    Projectile* GetProjectileByIndex(int index) {
      return *reinterpret_cast<Projectile**>(this + 0x18 + index * 4);
    }
   private:
     static ProjectileManager* instance_;
  };
}  // namespace dota
