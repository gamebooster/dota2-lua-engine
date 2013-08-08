#pragma once

#include "..\source-sdk\SDK.h"

namespace dota {
  class BaseEntity {
  public:
    const CBaseHandle& GetRefEHandle() {
      typedef CBaseHandle& ( __thiscall* OriginalFn )(void*);
      return utils::GetVtableFunction<OriginalFn>(this, 2)(this);
    }
    int ComputeTranslucencyType() {
      typedef int ( __thiscall* OriginalFn )(void*);
      return utils::GetVtableFunction<OriginalFn>(this, 100)(this);
    }
    bool IsAlive() {
      typedef bool ( __thiscall* OriginalFn )(void*);
      return utils::GetVtableFunction<OriginalFn>(this, 147)(this);
    }
    int GetTeamIndex() {
      typedef int ( __thiscall* OriginalFn )(void*);
      return utils::GetVtableFunction<OriginalFn>(this, 87)(this);
    }
    int GetHealth() {
      typedef int ( __thiscall* OriginalFn )(void*);
      return utils::GetVtableFunction<OriginalFn>(this, 120)(this);
    }
    bool IsPlayer() {
      typedef bool ( __thiscall* OriginalFn )(void*);
      return utils::GetVtableFunction<OriginalFn>(this, 149)(this);
    }
    Vector& GetAbsOrigin() {
      typedef Vector& ( __thiscall* OriginalFn )( PVOID );
      return utils::GetVtableFunction<OriginalFn>(this, 11)(this);
    }
    Vector& GetAbsAngles() {
      typedef Vector& ( __thiscall* OriginalFn )( PVOID );
      return utils::GetVtableFunction<OriginalFn>(this, 10)(this);
    }
    Vector WorldSpaceCenter() {
      Vector vWorldSpaceCenter;
      return vWorldSpaceCenter;
    }
    ClientClass* GetClientClass() {
      PVOID pNetworkable = (PVOID)(this + 0x8);
      typedef ClientClass* ( __thiscall* OriginalFn )( PVOID );
      return utils::GetVtableFunction<OriginalFn>( pNetworkable, 2 )( pNetworkable );
    }
    int GetIndex() {
      PVOID pNetworkable = (PVOID)(this + 0x8);
      typedef int ( __thiscall* OriginalFn )( PVOID );
      return utils::GetVtableFunction<OriginalFn>( pNetworkable, 9 )( pNetworkable );
    }
  };

}