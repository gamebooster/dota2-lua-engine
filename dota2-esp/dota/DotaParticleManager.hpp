#pragma once

#include "DotaGlobal.h"
#include "..\utils\global_address_retriever.hpp"

namespace dota {

class ParticleManager {
public:
  static void PrecacheParticleSystem(const char* name) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("ParticleManager::PrecacheParticleSystem");

    __asm {
      mov esi, name
      call address
    }
  }

  void DestroyParticle(int particle_index, int stop_emission = 0) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("ParticleManager::DestroyParticle");

    __asm {
      mov ecx, this
      push stop_emission
      push particle_index
      call address
    }
  }

  int CreateParticle(const char* name, int attachment, BaseEntity* entity) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("ParticleManager::CreateParticle");

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
  void SetParticleControl(int particle_index, int index, Vector& vector) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("ParticleManager::SetParticleControl");

    float x = vector.x;
    float y = vector.y;
    float z = vector.z;

    __asm {
      push z
      push y
      push x
      push index
      mov eax, particle_index
      mov edx, this
      call address
    }
  }
  void SetParticleControlEnt(int particle_index, int unknown0, void* entity, int unknown1, const char* attached, Vector& vector) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("ParticleManager::SetParticleControlEnt");

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
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("ParticleManager::ReleaseParticleIndex");

    __asm {
      push this
      mov eax, particle_index
      call address
    }
  }
  static ParticleManager* ParticleManager::GetInstance() {
    if (instance_ == nullptr) {
      unsigned long system = (unsigned long)GameSystemsRetriever().FindByName("CDOTA_ParticleManagerSystem");
      instance_ = *(ParticleManager**)(system + 0xC);
    }
    return instance_;
  }
 private:
  static ParticleManager* instance_;
};

ParticleManager* ParticleManager::instance_ = nullptr;

}