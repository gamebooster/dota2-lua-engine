#pragma once

#include "SDK.h"
#include "Panels.h"

namespace sourcesdk {

typedef struct ScreenSize_t
{
  int screen_height;
  int screen_width;

} ScreenSize;


class DrawUtils {
public:
	DrawUtils(ISurface* surface, ISurfaceNew* surface_new,  EngineClient* engine_client);
	void DrawString( int x, int y, int r, int g, int b, int a, bool center, const char *text, ... );
	void DrawRect( int x, int y, int w, int h, int r, int g, int b, int a );
	void OutlineRect( int x, int y, int w, int h, int r, int g, int b, int a );
	bool WorldToScreenNew( Vector& vIn, Vector& vOut );
private:
	unsigned long font_;
  ScreenSize screen_size_;
  ISurface* surface_;
  ISurfaceNew* surface_new_;
  EngineClient* engine_client_;
};

}