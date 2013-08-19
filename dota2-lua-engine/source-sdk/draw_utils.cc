// Copyright 2013 Karl Skomski - GPL v3
#include "source-sdk/draw_utils.h"
#include "source-sdk/global_instance_manager.h"

namespace sourcesdk {

  enum EFontFlags {
    FONTFLAG_NONE,
    FONTFLAG_ITALIC = 0x001,
    FONTFLAG_UNDERLINE = 0x002,
    FONTFLAG_STRIKEOUT = 0x004,
    FONTFLAG_SYMBOL = 0x008,
    FONTFLAG_ANTIALIAS = 0x010,
    FONTFLAG_GAUSSIANBLUR = 0x020,
    FONTFLAG_ROTARY = 0x040,
    FONTFLAG_DROPSHADOW = 0x080,
    FONTFLAG_ADDITIVE = 0x100,
    FONTFLAG_OUTLINE = 0x200,
    FONTFLAG_CUSTOM = 0x400
  };

  DrawUtils::DrawUtils() {
}
int DrawUtils::CreateFont(const char* name, int tall, int weight) {
  int font = GlobalInstanceManager::GetSurface()->CreateFont();
  GlobalInstanceManager::GetSurface()
    ->SetFontGlyphSet(font, name, tall, weight, 0, 0,
    EFontFlags::FONTFLAG_ANTIALIAS | EFontFlags::FONTFLAG_OUTLINE);
  return font;
}

void DrawUtils::DrawString(int font,
    int x, int y,
    int r, int g, int b, int a, bool center, const char *text, ... ) {
  if (text == nullptr) return;

  va_list va_alist;
  char buffer[1024] = { '\0' };
  wchar_t w_text[1024] = { '\0' };

  va_start(va_alist, text);
  vsprintf_s(buffer, text, va_alist);
  va_end(va_alist);

  wsprintfW(w_text, L"%S", buffer);

  if (center) {
    int text_wide, text_tall;
    GlobalInstanceManager::GetSurface()->GetTextSize(font, w_text,
                                                     text_wide, text_tall);

    x-= (text_wide/2);
  }

  GlobalInstanceManager::GetSurfaceNew()->DrawSetTextFont(font);
  GlobalInstanceManager::GetSurfaceNew()->DrawSetTextPos(x, y);
  GlobalInstanceManager::GetSurfaceNew()->DrawSetTextColor(r, g, b, a);
  GlobalInstanceManager::GetSurfaceNew()
    ->DrawPrintText(w_text, wcslen(w_text));
}

void DrawUtils::DrawRect(
  int x, int y, int w, int h, int r, int g, int b, int a ) {
  GlobalInstanceManager::GetSurfaceNew()->DrawSetColor(r, g, b, a);
  GlobalInstanceManager::GetSurfaceNew()->DrawFilledRect(x, y, x + w, y + h);
}

void DrawUtils::OutlineRect(
  int x, int y, int w, int h, int r, int g, int b, int a ) {
  GlobalInstanceManager::GetSurfaceNew()->DrawSetColor(r, g, b, a);
  GlobalInstanceManager::GetSurfaceNew()->DrawOutlinedRect(x, y, x + w, y + h);
}

int DrawUtils::FrustumTransform(
  const matrix3x4_t &worldToSurface, const Vector& point, Vector& screen) {
  // UNDONE: Clean this up some, handle off-screen vertices
  float w;

  screen.x = worldToSurface[0][0] * point[0] +
             worldToSurface[0][1] * point[1] +
             worldToSurface[0][2] * point[2] +
             worldToSurface[0][3];
  screen.y = worldToSurface[1][0] * point[0] +
             worldToSurface[1][1] * point[1] +
             worldToSurface[1][2] * point[2] + worldToSurface[1][3];
  w        = worldToSurface[3][0] * point[0] +
             worldToSurface[3][1] * point[1] +
             worldToSurface[3][2] * point[2] + worldToSurface[3][3];

  // Just so we have something valid here
  screen.z = 0.0f;

  bool behind;
  if (w < 0.001f) {
    behind = true;
    screen.x *= 100000;
    screen.y *= 100000;
  } else {
    behind = false;
    float invw = 1.0f / w;
    screen.x *= invw;
    screen.y *= invw;
  }

  return behind;
}

int DrawUtils::HudTransform(const Vector& point, Vector& screen) {
  return FrustumTransform(
    GlobalInstanceManager::GetEngineClient()->WorldToScreenMatrix(),
    point, screen);
}

bool DrawUtils::GetVectorInScreenSpace(
  Vector pos, int& x, int& y, Vector *vecOffset) {
    Vector screen;

    // Apply the offset, if one was specified
    if (vecOffset != NULL) pos += *vecOffset;

    // Transform to HUD space
    int facing = HudTransform(pos, screen);
    x = 0.5f * (1.0f + screen[0]) *
      GlobalInstanceManager::GetClient()->GetScreenWidth();
    y = 0.5f * (1.0f - screen[1]) *
      GlobalInstanceManager::GetClient()->GetScreenHeight();

    // Make sure the player's facing it
    if (facing) {
      // We're actually facing away from the Target. Stomp the screen position.
      x = -640;
      y = -640;
      return false;
    }

    return true;
}

}  // namespace sourcesdk
