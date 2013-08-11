#include "draw_utils.h"
#include "global_instance_manager.h"

namespace sourcesdk {

DrawUtils::DrawUtils() : font_(GlobalInstanceManager::GetSurface()->CreateFont()) {
	GlobalInstanceManager::GetSurface()->SetFontGlyphSet( font_, "Tahoma", 14, 500, 0, 0, 0x200 );
}

void DrawUtils::DrawString( int x, int y, int r, int g, int b, int a, bool center, const char *text, ... ) {
	if( text == nullptr )
		return;

	va_list va_alist;
	char szBuffer[1024] = { '\0' };
	wchar_t szString[1024] = { '\0' };

	va_start( va_alist, text );
	vsprintf_s( szBuffer, text, va_alist );
	va_end( va_alist );

	wsprintfW( szString, L"%S", szBuffer );

	if(center){
		int textSizeWide, textSizeTall;
		GlobalInstanceManager::GetSurface()->GetTextSize(font_,szString,textSizeWide,textSizeTall);

		x-=(textSizeWide/2);
	}

	GlobalInstanceManager::GetSurfaceNew()->DrawSetTextFont( font_ );
	GlobalInstanceManager::GetSurfaceNew()->DrawSetTextPos( x, y );
	GlobalInstanceManager::GetSurfaceNew()->DrawSetTextColor( r,g,b,a );
	GlobalInstanceManager::GetSurfaceNew()->DrawPrintText( szString, wcslen( szString ) );
}

void DrawUtils::DrawRect( int x, int y, int w, int h, int r, int g, int b, int a )
{
	GlobalInstanceManager::GetSurfaceNew()->DrawSetColor( r,g,b,a );
	GlobalInstanceManager::GetSurfaceNew()->DrawFilledRect( x, y, x + w, y + h );
}

void DrawUtils::OutlineRect( int x, int y, int w, int h, int r, int g, int b, int a )
{
	GlobalInstanceManager::GetSurfaceNew()->DrawSetColor( r, g, b, a );
	GlobalInstanceManager::GetSurfaceNew()->DrawOutlinedRect( x, y, x + w, y + h );
}

int DrawUtils::FrustumTransform( const matrix3x4_t &worldToSurface, const Vector& point, Vector& screen )
{
  // UNDONE: Clean this up some, handle off-screen vertices
  float w;

  screen.x = worldToSurface[0][0] * point[0] + worldToSurface[0][1] * point[1] + worldToSurface[0][2] * point[2] + worldToSurface[0][3];
  screen.y = worldToSurface[1][0] * point[0] + worldToSurface[1][1] * point[1] + worldToSurface[1][2] * point[2] + worldToSurface[1][3];
  w		     = worldToSurface[3][0] * point[0] + worldToSurface[3][1] * point[1] + worldToSurface[3][2] * point[2] + worldToSurface[3][3];

  // Just so we have something valid here
  screen.z = 0.0f;

  bool behind;
  if( w < 0.001f )
  {
    behind = true;
    screen.x *= 100000;
    screen.y *= 100000;
  }
  else
  {
    behind = false;
    float invw = 1.0f / w;
    screen.x *= invw;
    screen.y *= invw;
  }

  return behind;
}

int DrawUtils::HudTransform( const Vector& point, Vector& screen ) {
  return FrustumTransform ( GlobalInstanceManager::GetEngineClient()->WorldToScreenMatrix(), point, screen );
}

bool DrawUtils::GetVectorInScreenSpace(Vector pos, int& x, int& y, Vector *vecOffset) {
  Vector screen;

  // Apply the offset, if one was specified
  if ( vecOffset != NULL )
    pos += *vecOffset;

  // Transform to HUD space
  int facing = HudTransform( pos, screen );
  x = 0.5f * ( 1.0f + screen[0] ) * GlobalInstanceManager::GetClient()->GetScreenWidth();
  y = 0.5f * ( 1.0f - screen[1] ) * GlobalInstanceManager::GetClient()->GetScreenHeight();

  // Make sure the player's facing it
  if ( facing )
  {
    // We're actually facing away from the Target. Stomp the screen position.
    x = -640;
    y = -640;
    return false;
  }

  return true;
}

bool DrawUtils::WorldToScreenNew( Vector& vIn, Vector& vOut ) {
	float w;
	bool behind = false;

	const matrix3x4_t& worldToScreen = GlobalInstanceManager::GetEngineClient()->WorldToScreenMatrix();

	vOut.x = worldToScreen[0][0] * vIn[0] + worldToScreen[0][1] * vIn[1] + worldToScreen[0][2] * vIn[2] + worldToScreen[0][3];
	vOut.y = worldToScreen[1][0] * vIn[0] + worldToScreen[1][1] * vIn[1] + worldToScreen[1][2] * vIn[2] + worldToScreen[1][3];
	w	   = worldToScreen[3][0] * vIn[0] + worldToScreen[3][1] * vIn[1] + worldToScreen[3][2] * vIn[2] + worldToScreen[3][3];
	vOut.z = 0.0f;	

	if( w < 0.001f ) {
		behind = true;
		vOut.x *= 100000;
		vOut.y *= 100000;
	} else {
		behind = false;
		float invw = 1.0f / w;
		vOut.x *= invw;
		vOut.y *= invw;
	}

	if( !behind ) {
		int sw = GlobalInstanceManager::GetClient()->GetScreenWidth();
		int	sh = GlobalInstanceManager::GetClient()->GetScreenHeight();

		float x = (float)sw / 2;
		float y = (float)sh / 2;

		x += 0.5f * vOut.x * sw + 0.5f;
		y -= 0.5f * vOut.y * sh + 0.5f;

		vOut.x = x;
		vOut.y = y;

		return true;
	}

	return false;
}

}  // namespace sourcesdk
