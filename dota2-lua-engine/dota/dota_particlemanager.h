// Copyright 2013 Karl Skomski - GPL v3
#pragma once

#include "dota/dota_global.h"
#include "utils/global_address_retriever.h"

namespace dota {

class ParticleManagerSystem {};

class ParticleManager {
 public:
  static void PrecacheParticleSystem(const char* name) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("ParticleManager::PrecacheParticleSystem");

    __asm {
      mov esi, name
      call address
    }
  }

  void DestroyParticle(int particle_index, int stop_emission = 0) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("ParticleManager::DestroyParticle");

    __asm {
      mov ecx, this
      push stop_emission
      push particle_index
      call address
    }
  }

  int CreateParticle(const char* name, int attachment, BaseEntity* entity) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("ParticleManager::CreateParticle");

    int particle_index = 0;

    __asm {
      mov eax, this 
      mov esi, [eax+ 0x48]
      lea edx, [esi+1]
      mov [eax+ 0x48], edx
      mov [particle_index], esi
      push entity
      push attachment
      push esi
      push eax
      mov eax, name
      call address
    }

    return particle_index;
  }
  void SetParticleControl(int particle_index,
                          int index,
                          const Vector& vector) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("ParticleManager::SetParticleControl");
    Vector vec = vector;
    __asm {
      push vec
      push index
      push particle_index
      push this
      call address
    }
  }
  void SetParticleControlEnt(int particle_index,
                             int unknown0,
                             void* entity,
                             int unknown1,
                             const char* attached,
                             const Vector& vector) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("ParticleManager::SetParticleControlEnt");

    float x = vector.x;
    float y = vector.y;
    float z = vector.z;

    __asm {
      push z
      push y
      push x
      push attached
      push unknown1
      push entity
      push unknown0
      push particle_index
      push this
      call address
    }
  }
  void ReleaseParticleIndex(int particle_index) {
    uint32_t address = GlobalAddressRetriever::GetInstance()
      .GetStaticAddress("ParticleManager::ReleaseParticleIndex");

    __asm {
      push this
      mov eax, particle_index
      call address
    }
  }
  static ParticleManager* ParticleManager::GetInstance() {
    if (instance_ == nullptr) {
      IGameSystem* system =
        GameSystemsRetriever().FindByName("CDOTA_ParticleManagerSystem");
      instance_ = reinterpret_cast<ParticleManagerSystem*>(system);
    }
    return *reinterpret_cast<ParticleManager**>(instance_ + 0xC);
  }

 private:
  static ParticleManagerSystem* instance_;
};

}  // namespace dota
