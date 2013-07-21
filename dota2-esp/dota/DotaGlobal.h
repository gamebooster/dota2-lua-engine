#pragma once

#include "..\source-sdk\SDK.h"
#include "..\source-sdk\netvar.h"

class CBasePlayer : CBaseEntity {
public:
  int GetPlayerId() {
    typedef int ( __thiscall* OriginalFn )( PVOID );
    return getvfunc<OriginalFn>(this, 282)(this);
  }
};

class CDotaPlayer : CBasePlayer {
public:
  int16 GetAssignedHero() {
    int offset = sourcesdk::NetVarManager::GetInstance().GetNetVarOffset("DT_DOTAPlayer","m_hAssignedHero");
    return *reinterpret_cast<int16*>(this + offset);
  }
};