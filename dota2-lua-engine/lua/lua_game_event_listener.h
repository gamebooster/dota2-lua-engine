// Copyright 2013 Karl Skomski - GPL v3
#pragma once

#include "..\source-sdk\game_event_listener.h"

class CLuaGameEventListener : public CGameEventListener {
public:
  CLuaGameEventListener(luabridge::LuaRef callback) : callback_(callback) {}
  void FireGameEvent(IGameEvent *event) {
    try {
      if (event) callback_(event);
    }
    catch (luabridge::LuaException const& e) {
      Warning("booster-lua: %s \n", e.what());
    }
  }
private:
  luabridge::LuaRef callback_;
};