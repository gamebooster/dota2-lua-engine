// Copyright 2013 Karl Skomski - GPL v3
#pragma once

#include "source-sdk/global_instance_manager.h"

class CGameEventListener : public IGameEventListener2 {
 public:
  CGameEventListener() : registered_for_events_(false) {}
  ~CGameEventListener() {
    StopListeningForAllEvents();
  }

  void ListenForGameEvent(const char *name) {
    registered_for_events_ = true;
    bool bServerSide = false;
    GlobalInstanceManager::GetGameEventManager()
      ->AddListener(this, name, bServerSide);
  }

  void StopListeningForAllEvents() {
    if ( registered_for_events_ ) {
      GlobalInstanceManager::GetGameEventManager()->RemoveListener(this);
      registered_for_events_ = false;
    }
  }

  virtual void FireGameEvent(IGameEvent *event) = 0;

  virtual int GetEventDebugID(void) {
    return 0;
  }

 private:
  bool registered_for_events_;
};
