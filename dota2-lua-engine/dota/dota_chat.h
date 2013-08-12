// Copyright 2013 Karl Skomski - GPL v3
#pragma once

namespace dota {
  class DotaChat {
  public:
    void EventPrintf(wchar_t const* message, int unknown);
    void MessagePrintf(int area, wchar_t const* message, int unknown0, int unknown1, float unknown2);

    static DotaChat* GetInstance();
  };
}