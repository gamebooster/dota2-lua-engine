class ParticleManager {
public:
  static void PrecacheParticleSystem(const char* name) {
    if (precache_particle_system_adddress_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(    
        "client.dll",
        reinterpret_cast<unsigned char*>("\x8B\x75\x0C\xE8\x00\x00\x00\x00\x85\xFF\x74\x07"),
        "xxxx????xxxx");

      precache_particle_system_adddress_ = utils::GetAbsoluteAddress(pattern_address + 0x3);
      if (precache_particle_system_adddress_ == 0) {
        utils::Log("hook: PrecacheParticleSystemError");
        return;
      }
    }

    __asm {
      mov esi, name
        call precache_particle_system_adddress_
    }
  }

  void DestroyParticle(int particle_index, int stop_emission = 0) {
    if (destroy_particle_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\x50\xE8\x00\x00\x00\x00\xC7\x86\x00\x00\x00\x00\x00\x00\x00\x00\x5F"),
        "xx????xx????????x");

      destroy_particle_address_ = utils::GetAbsoluteAddress(pattern_address + 0x1);
      if (destroy_particle_address_ == 0) {
        utils::Log("hook: DestroyParticleError");
        return;
      }
    }

    __asm {
      mov ecx, this
        push stop_emission
        push particle_index
        call destroy_particle_address_
    }
  }

  int CreateParticle(const char* name, int unknown, CBaseEntity* entity) {
    if (create_particle_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x6A\x00\x6A\x00\x68\x00\x00\x00\x00\x6A\x01"),
        "x????xxxxx????xx");

      create_particle_address_ = utils::GetAbsoluteAddress(pattern_address);
      if (create_particle_address_ == 0) {
        utils::Log("hook: CreateParticleError");
        return 0;
      }
    }

    int particle_index = 0;

    __asm {
      mov eax, this 
        mov esi, [eax+48]
      lea edx, [esi+1]
      mov [eax+48], edx
        push entity
        push unknown
        push esi
        push eax
        mov eax, name
        call create_particle_address_
        mov [particle_index], eax
    }

    return particle_index;
  }
  void SetParticleControl(int particle_index, int unknown, Vector& vector) {
    if (set_particle_control_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x8B\x15\x00\x00\x00\x00\x52\xEB\x41"),
        "x????xx????xxx");

      set_particle_control_address_ = utils::GetAbsoluteAddress(pattern_address);
      if (set_particle_control_address_ == 0) {
        utils::Log("hook: SetParticleControlError");
        return;
      }
    }

    float x = vector.x;
    float y = vector.y;
    float z = vector.z;

    __asm {
      push z
        push y
        push x
        push unknown
        mov eax, particle_index
        mov edx, this
        call set_particle_control_address_
    }
  }
  void SetParticleControlEnt(int particle_index, int unknown0, void* entity, int unknown1, const char* attached, Vector& vector) {
    if (set_particle_ent_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x51\x8B\xC7\xE8\x00\x00\x00\x00\x8B\xCE"),
        "x????xx????xxxx????xx");

      set_particle_ent_address_ = utils::GetAbsoluteAddress(pattern_address);
      if (set_particle_ent_address_ == 0) {
        utils::Log("hook: SetParticleControlEntError");
        return;
      }
    }
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
        call set_particle_ent_address_
    }
  }
  void ReleaseParticleIndex(int particle_index) {
    if (release_particle_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(    
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x8B\x44\x24\x34\x99"),
        "x????xxxxx");

      release_particle_address_ = utils::GetAbsoluteAddress(pattern_address);
      if (release_particle_address_ == 0) {
        utils::Log("hook: SetParticleControlEntError");
        return;
      }
    }

    __asm {
      push this
        mov eax, particle_index
        call release_particle_address_
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
  static unsigned long create_particle_address_;
  static unsigned long set_particle_ent_address_;
  static unsigned long release_particle_address_;
  static unsigned long precache_particle_system_adddress_;
  static unsigned long set_particle_control_address_;
  static unsigned long destroy_particle_address_;
  static ParticleManager* instance_;
};