#pragma once

#include "source-sdk\SDK.h"

namespace commands {
  void Register();
  void Unregister();

  void SetAnnouncer(const CCommandContext &context, const CCommand &args);
  void SetKillstreakAnnouncer(const CCommandContext &context, const CCommand &args);
  void SetHud(const CCommandContext &context, const CCommand &args);

  void ShiftDown(const CCommandContext &context, const CCommand &args);
  void ShiftUp(const CCommandContext &context, const CCommand &args);

  static ConVar dota_esp_draw( "dota_esp_draw", "1", FCVAR_NONE, "Draw the esp" );
}