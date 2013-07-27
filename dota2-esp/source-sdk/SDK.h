#pragma once

// extern source sdk

#define COMPILER_MSVC
#define COMPILER_MSVC64

#include "dt_recv.h"
#include "tier1\convar.h"

#include "..\utils\utils.h"

using namespace std;

typedef void* ( __cdecl* CreateInterface_t )( const char*, int* );
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

typedef struct player_info_s
{
	unsigned __int64			xuid;
	char			name[32];
	int				userID;
	char			guid[33];
	unsigned long	friendsID;
	char			friendsName[32];
	bool			fakeplayer;
	bool			ishltv;
	unsigned long	customFiles[4];
	unsigned char	filesDownloaded;
} player_info_t;

class ClientClass {
 public:
	const char* GetName( void ) {
		return *(char**)(this + 0x8);
	}
	RecvTable* GetTable( )
	{
		return *(RecvTable**)(this + 0xC);
	}
	ClientClass* NextClass( )
	{
		return *(ClientClass**)(this + 0x10);
	}
	int GetClassID( void )
	{
		return *(int*)(this + 0x14);
	}
};

class ClientTools {
public:
  void* GetLocalPlayer()
  {
    typedef void* ( __thiscall* OriginalFn )( PVOID);
    return utils::GetVtableFunction<OriginalFn>( this, 23 )( this);
  }
};

class CHLClient {
public:
	void LevelInitPreEntity( char const* pMapName ) {
		typedef void ( __thiscall* OriginalFn )( PVOID, char const*);
		utils::GetVtableFunction<OriginalFn>( this, 4 )( this, pMapName);
	}
	void HudUpdate( bool bActive ) {
		typedef void ( __thiscall* OriginalFn )( PVOID, bool);
		utils::GetVtableFunction<OriginalFn>( this, 10 )( this, bActive);
	}
	ClientClass* GetAllClasses( void ) {
		typedef ClientClass* ( __thiscall* OriginalFn )( PVOID );
		return utils::GetVtableFunction<OriginalFn>( this, 7 )( this );
	}
  int GetScreenWidth() {
    typedef int ( __cdecl* OriginalFn )();
    return utils::GetVtableFunction<OriginalFn>( this, 59 )();
  }
  int GetScreenHeight() {
    typedef int ( __cdecl* OriginalFn )();
    return utils::GetVtableFunction<OriginalFn>( this, 60 )();
  }
};

extern DWORD other_hero_model_offset;
extern DWORD game_time_offset;
extern DWORD game_rules_address;

class CBaseEntity {
public:
  template <class ValueType> ValueType GetValueWithOffset(int offset) {
    return *reinterpret_cast<ValueType*>(this + offset);
  }
  bool IsAlive() {
    typedef bool ( __thiscall* OriginalFn )(void*);
    return utils::GetVtableFunction<OriginalFn>(this, 147)(this);
  }
  int GetHealth() {
    typedef int ( __thiscall* OriginalFn )(void*);
    return utils::GetVtableFunction<OriginalFn>(this, 120)(this);
  }
  bool IsPlayer() {
    typedef bool ( __thiscall* OriginalFn )(void*);
    return utils::GetVtableFunction<OriginalFn>(this, 149)(this);
  }
	bool IsIllusion() {
		if ((*(int*)( (unsigned long)( this ) + other_hero_model_offset )) <= 0 )
			return false;
		else
			return true;
	}
	Vector& GetAbsOrigin() {
		typedef Vector& ( __thiscall* OriginalFn )( PVOID );
		return utils::GetVtableFunction<OriginalFn>(this, 11)(this);
	}
  Vector& GetAbsAngles() {
    typedef Vector& ( __thiscall* OriginalFn )( PVOID );
    return utils::GetVtableFunction<OriginalFn>(this, 10)(this);
  }
	Vector WorldSpaceCenter() {
		Vector vWorldSpaceCenter;
    return vWorldSpaceCenter;
	}
	ClientClass* GetClientClass() {
		PVOID pNetworkable = (PVOID)(this + 0x8);
		typedef ClientClass* ( __thiscall* OriginalFn )( PVOID );
		return utils::GetVtableFunction<OriginalFn>( pNetworkable, 2 )( pNetworkable );
	}
	int GetIndex() {
		PVOID pNetworkable = (PVOID)(this + 0x8);
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return utils::GetVtableFunction<OriginalFn>( pNetworkable, 9 )( pNetworkable );
	}
};

class EngineClient {
public:
	float GetGameTime() {
		DWORD GameRules = *( PDWORD )(game_rules_address);
		return *( PFLOAT )( GameRules + game_time_offset );;
	}
	void GetScreenSize(int& width, int& height) {
		typedef void ( __thiscall* OriginalFn )( PVOID, int& , int& );
		return utils::GetVtableFunction<OriginalFn>( this, 5 )( this, width, height );
	}
	bool GetPlayerInfo( int ent_num, player_info_t *pinfo ) {
		typedef bool ( __thiscall* OriginalFn )( PVOID, int, player_info_t * );
		return utils::GetVtableFunction<OriginalFn>(this, 8)(this, ent_num, pinfo );
	}
	bool Con_IsVisible() {
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return utils::GetVtableFunction<OriginalFn>( this, 11 )( this );
	}
	int GetLocalPlayerIndex ()
	{
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return utils::GetVtableFunction<OriginalFn>( this, 12 )( this );
	}
	float Time()
	{
		typedef float ( __thiscall* OriginalFn )( PVOID );
		return utils::GetVtableFunction<OriginalFn>( this, 14 )( this );
	}
	void GetViewAngles( Vector& va ) {
		typedef void ( __thiscall* OriginalFn )( PVOID, Vector& va );
		return utils::GetVtableFunction<OriginalFn>( this, 19 )( this, va );
	}
	void SetViewAngles( Vector& va ) {
		typedef void ( __thiscall* OriginalFn )( PVOID, Vector& va );
		return utils::GetVtableFunction<OriginalFn>( this, 20 )( this, va );
	}
	int GetMaxClients() {
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return utils::GetVtableFunction<OriginalFn>( this, 21 )( this );
	}
	bool IsInGame()
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return utils::GetVtableFunction<OriginalFn>( this, 26 )( this );
	}
	bool IsConnected() {
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return utils::GetVtableFunction<OriginalFn>( this, 27 )( this );
	}
	bool IsDrawingLoadingImage() {
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return utils::GetVtableFunction<OriginalFn>( this, 28 )( this );
	}
	const matrix3x4_t& WorldToScreenMatrix() {
		typedef const matrix3x4_t& ( __thiscall* OriginalFn )( PVOID );
		return utils::GetVtableFunction<OriginalFn>(this, 37)(this);
	}
	bool IsTakingScreenshot() {
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return utils::GetVtableFunction<OriginalFn>( this, 91 )( this );
	}
	DWORD* GetNetChannelInfo() {
		typedef DWORD* ( __thiscall* OriginalFn )( PVOID );
		return utils::GetVtableFunction<OriginalFn>( this, 72 )( this );
	}
	void ClientCmd_Unrestricted(const char* chCommandString) {
		typedef void ( __thiscall* OriginalFn )( PVOID, const char * );
		return utils::GetVtableFunction<OriginalFn>( this, 105 )( this, chCommandString );
	}
};

class IPanel
{
public:
	const char *GetName(unsigned int vguiPanel)
	{
		typedef const char* ( __thiscall* OriginalFn )( PVOID, unsigned int );
		return utils::GetVtableFunction<OriginalFn>( this, 40 )( this, vguiPanel );
	}
	void PaintTraverse(int unknw, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, int, unsigned int, bool, bool);
		return utils::GetVtableFunction<OriginalFn>( this, 45 )( this, unknw, vguiPanel, forceRepaint, allowForce);
	}
};

class ISurfaceNew {
 public:
	void DrawSetColor(int r, int g, int b, int a) {
		typedef void ( __thiscall* OriginalFn )( PVOID, int, int, int, int );
		utils::GetVtableFunction<OriginalFn>( this, 17 )( this, r, g, b, a );
	}
	void DrawOutlinedRect(int x0, int y0, int x1, int y1) {
		typedef void ( __thiscall* OriginalFn )( PVOID, int, int, int, int );
		utils::GetVtableFunction<OriginalFn>( this, 21 )( this, x0, y0, x1, y1 );
	}
	void DrawFilledRect(int x0, int y0, int x1, int y1) {
		typedef void ( __thiscall* OriginalFn )( PVOID, int, int, int, int );
		utils::GetVtableFunction<OriginalFn>( this, 19 )( this, x0, y0, x1, y1 );
	}
	void DrawSetTextFont(unsigned long font) {
		typedef void ( __thiscall* OriginalFn )( PVOID, unsigned long );
		utils::GetVtableFunction<OriginalFn>( this, 13 )( this, font );
	}
	void DrawSetTextColor(int r, int g, int b, int a ) {
		typedef void ( __thiscall* OriginalFn )( PVOID, int, int, int, int );
		utils::GetVtableFunction<OriginalFn>( this, 26 )( this, r, g, b, a );
	}
	void DrawSetTextPos(int x, int y ) {
		typedef void ( __thiscall* OriginalFn )( PVOID, int, int );
		utils::GetVtableFunction<OriginalFn>( this, 27 )( this, x, y );
	}
	void DrawPrintText(const wchar_t *text, int textLen ) {
		typedef void ( __thiscall* OriginalFn )( PVOID, const wchar_t *, int, int );
		return utils::GetVtableFunction<OriginalFn>( this, 28 )( this, text, textLen, 0 );
	}
};

class ISurface {
public:
	void GetTextSize(unsigned long font,const wchar_t *text,int &wide, int &tall) {
		typedef void ( __thiscall* OriginalFn )( PVOID, unsigned long,const wchar_t *,int &,int &);
		utils::GetVtableFunction<OriginalFn>( this, 60 )( this, font, text, wide, tall );
	}
	unsigned long CreateFont() {
		typedef unsigned int ( __thiscall* OriginalFn )( PVOID );
		return utils::GetVtableFunction<OriginalFn>( this, 52 )( this );
	}
	void SetFontGlyphSet (unsigned long &font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags ) {
		typedef void ( __thiscall* OriginalFn )( PVOID, unsigned long, const char*, int, int, int, int, int, int, int );
		utils::GetVtableFunction<OriginalFn>( this, 53 )( this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0 );
	}
};

class EntityList {
  public:
  CBaseEntity* GetClientEntity( int entnum ) {
    typedef CBaseEntity* ( __thiscall* OriginalFn )( PVOID, int );
    return utils::GetVtableFunction<OriginalFn>( this, 3 )( this, entnum );
  }
  CBaseEntity* GetClientEntityFromHandle( int hEnt ) {
    typedef CBaseEntity* ( __thiscall* OriginalFn )( PVOID, int );
    return utils::GetVtableFunction<OriginalFn>( this, 4 )( this, hEnt );
  }
  int GetHighestEntityIndex(void) {
    typedef int ( __thiscall* OriginalFn )( PVOID );
    return utils::GetVtableFunction<OriginalFn>( this, 6 )( this );
  }
};
