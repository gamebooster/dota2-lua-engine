// Copyright 2013 Karl Skomski - GPL v3
#pragma once

#include "utils/global_address_retriever.h"
#include "source-sdk/global_instance_manager.h"
#include "dota/dota_baseentity.h"

namespace dota {
  class DotaInput {
   public:
    static DotaInput* GetInstance() {
      if (instance_ == nullptr) {
        instance_ = reinterpret_cast<DotaInput*>(
          GlobalAddressRetriever::GetInstance()
            .GetDynamicAddress("DotaInput"));
      }

      return instance_;
    }
    Vector Get3dPositionUnderCursor() {
      if (*reinterpret_cast<float*>(this + 0x210) == -1) {
        return vec3_invalid;
      }
      Vector vec;
      vec.x = *reinterpret_cast<float*>(this + 0x210);
      vec.y = *reinterpret_cast<float*>(this + 0x214);
      vec.z = *reinterpret_cast<float*>(this + 0x218);
      return vec;
    }
    BaseEntity* GetEntityUnderCursor() {
      if (*reinterpret_cast<int*>(this + 0x220) == -1) {
        return nullptr;
      }
      return GlobalInstanceManager::GetClientEntityList()
        ->GetClientEntityFromHandle(*reinterpret_cast<int*>(this + 0x220));
    }
   private:
     static DotaInput* instance_;
  };
}  // namespace dota
