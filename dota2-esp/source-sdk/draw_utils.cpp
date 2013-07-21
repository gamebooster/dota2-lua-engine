#include "draw_utils.h"

namespace sourcesdk {

DrawUtils::DrawUtils(ISurface* surface, ISurfaceNew* surface_new,  EngineClient* engine_client) :
  font_(surface->CreateFont()),
  surface_(surface),
  surface_new_(surface_new),
  engine_client_(engine_client) {

	engine_client_->GetScreenSize(screen_size_.screen_width, screen_size_.screen_height);
	surface_->SetFontGlyphSet( font_, "Tahoma", 14, 500, 0, 0, 0x200 );
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
		surface_->GetTextSize(font_,szString,textSizeWide,textSizeTall);

		x-=(textSizeWide/2);
	}

	surface_new_->DrawSetTextFont( font_ );
	surface_new_->DrawSetTextPos( x, y );
	surface_new_->DrawSetTextColor( r,g,b,a );
	surface_new_->DrawPrintText( szString, wcslen( szString ) );
}

void DrawUtils::DrawRect( int x, int y, int w, int h, int r, int g, int b, int a )
{
	surface_new_->DrawSetColor( r,g,b,a );
	surface_new_->DrawFilledRect( x, y, x + w, y + h );
}

void DrawUtils::OutlineRect( int x, int y, int w, int h, int r, int g, int b, int a )
{
	surface_new_->DrawSetColor( r, g, b, a );
	surface_new_->DrawOutlinedRect( x, y, x + w, y + h );
}

bool DrawUtils::WorldToScreenNew( Vector& vIn, Vector& vOut ) {
	float w;
	bool behind = false;

	const matrix3x4& worldToScreen = engine_client_->WorldToScreenMatrix();

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
		int sw = screen_size_.screen_width;
		int	sh = screen_size_.screen_height;

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

}