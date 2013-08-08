#pragma once

#include "..\utils\global_address_retriever.hpp"

namespace dota {

class CNewParticleEffect {
public:
  void SetControlPoint(int index, Vector const& vector) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("CNewParticleEffect::SetControlPoint");
    Vector local_vector = vector;

    __asm {
      lea esi, [local_vector]
      push index
      mov edi, this
      call address
    }
  }
};

class ParticleProperty {
public:
  void StopEmissionAndDestroyImmediately(CNewParticleEffect* effect) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("ParticleProperty::StopEmissionAndDestroyImmediately");
    __asm {
      mov eax, effect
      mov ecx, this
      call address
    }
  }
  CNewParticleEffect* Create(char const* name, int particle_attachment, int attachment_point, Vector vector = vec3_origin) {
    uint32_t address = GlobalAddressRetriever::GetInstance().GetStaticAddress("ParticleProperty::Create");

    CNewParticleEffect* effect = nullptr;

    float x = vector.x;
    float y = vector.y;
    float z = vector.z;

    __asm {
      push z
      push y
      push x
      push attachment_point
      push particle_attachment
      push this
      mov eax, name
      call address
      mov effect, eax
    }

    return effect;
  }
};

}