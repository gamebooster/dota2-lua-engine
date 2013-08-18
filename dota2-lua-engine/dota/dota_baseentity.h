// Copyright 2013 Karl Skomski - GPL v3
#pragma once

#include "source-sdk/source_sdk.h"

namespace dota {
  class BaseTempEntity {
   public:
     ClientClass* GetClientClass() {
       void* networkable = reinterpret_cast<void*>(this + 0x4);
       typedef ClientClass* ( __thiscall* OriginalFn )( void* );
       return utils::GetVtableFunction<OriginalFn>(networkable, 2)
         (networkable);
     }
     const char* GetName() {
       return *reinterpret_cast<const char**>(this + 0x30);
     }
  };
  class BaseEntity {
  public:
    const CBaseHandle& GetRefEHandle() {
      typedef CBaseHandle& (__thiscall* OriginalFn)(void* thisptr);
      return utils::GetVtableFunction<OriginalFn>(this, 2)(this);
    }
    int ComputeTranslucencyType() {
      typedef int ( __thiscall* OriginalFn )(void* thisptr);
      return utils::GetVtableFunction<OriginalFn>(this, 100)(this);
    }
    bool IsAlive() {
      typedef bool ( __thiscall* OriginalFn )(void* thisptr);
      return utils::GetVtableFunction<OriginalFn>(this, 147)(this);
    }
    int GetTeamIndex() {
      typedef int ( __thiscall* OriginalFn )(void* thisptr);
      return utils::GetVtableFunction<OriginalFn>(this, 87)(this);
    }
    int GetHealth() {
      typedef int ( __thiscall* OriginalFn )(void* thisptr);
      return utils::GetVtableFunction<OriginalFn>(this, 120)(this);
    }
    bool IsPlayer() {
      typedef bool ( __thiscall* OriginalFn )(void* thisptr);
      return utils::GetVtableFunction<OriginalFn>(this, 149)(this);
    }
    Vector& GetAbsOrigin() {
      typedef Vector& ( __thiscall* OriginalFn )( void* );
      return utils::GetVtableFunction<OriginalFn>(this, 11)(this);
    }
    Vector& GetAbsAngles() {
      typedef Vector& ( __thiscall* OriginalFn )( void* );
      return utils::GetVtableFunction<OriginalFn>(this, 10)(this);
    }
    Vector WorldSpaceCenter() {
      Vector vWorldSpaceCenter;
      return vWorldSpaceCenter;
    }
    ClientClass* GetClientClass() {
      void* networkable = reinterpret_cast<void*>(this + 0x8);
      typedef ClientClass* ( __thiscall* OriginalFn )( void* );
      return utils::GetVtableFunction<OriginalFn>(networkable, 2)
        (networkable);
    }
    int GetIndex() {
      void* networkable = reinterpret_cast<void*>(this + 0x8);
      typedef void ( __thiscall* OriginalFn )( void*, void* );
      int entindex;
      utils::GetVtableFunction<OriginalFn>(networkable, 10)
        (networkable, &entindex);
      return entindex;
    }
  };



}  // namespace dota
