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

  static ConCommand announcer_command( "dota_announcer_set", SetAnnouncer, "Set the announcer");
  static ConCommand announcer_killstreak_command( "dota_announcer_killstreak_set", SetKillstreakAnnouncer, "Set the killstreak announcer");
  static ConCommand hud_command("dota_hud_set", SetHud, "Set the hud");

  static ConCommand shift_down("+shift", ShiftDown, "shift down");
  static ConCommand shift_up("-shift", ShiftUp, "shift up");
}