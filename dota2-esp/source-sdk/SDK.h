#pragma once

#include "..\utils\utils.h"

using namespace std;

typedef void* ( __cdecl* CreateInterface_t )( const char*, int* );
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

namespace dota {
 class BaseEntity;
}

typedef struct player_info_s {
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


abstract_class IGameSystem {
public:
  // GameSystems are expected to implement these methods.
  virtual char const *Name() = 0;

  // Init, shutdown
  // return true on success. false to abort DLL init!
  virtual bool Init() = 0;
  virtual void PostInit() = 0;
  virtual void Shutdown() = 0;

  // Level init, shutdown
  virtual void LevelInitPreEntity() = 0;
  // entities are created / spawned / precached here
  virtual void LevelInitPostEntity() = 0;

  virtual void LevelShutdownPreClearSteamAPIContext() {};
  virtual void LevelShutdownPreEntity() = 0;
  // Entities are deleted / released here...
  virtual void LevelShutdownPostEntity() = 0;
  // end of level shutdown

  // Called during game save
  virtual void OnSave() = 0;

  // Called during game restore, after the local player has connected and entities have been fully restored
  virtual void OnRestore() = 0;

  // Called every frame. It's safe to remove an igamesystem from within this callback.
  virtual void SafeRemoveIfDesired() = 0;

  virtual bool	IsPerFrame() = 0;

  // destructor, cleans up automagically....
  virtual ~IGameSystem();

  // Client systems can use this to get at the map name
  static char const*	MapName();

  // These methods are used to add and remove server systems from the
  // main server loop. The systems are invoked in the order in which
  // they are added.
  static void Add ( IGameSystem* pSys );
  static void Remove ( IGameSystem* pSys );
  static void RemoveAll (  );

  // These methods are used to initialize, shutdown, etc all systems
  static bool InitAllSystems();
  static void PostInitAllSystems();
  static void ShutdownAllSystems();
  static void LevelInitPreEntityAllSystems( char const* pMapName );
  static void LevelInitPostEntityAllSystems();
  static void LevelShutdownPreClearSteamAPIContextAllSystems(); // Called prior to steamgameserverapicontext->Clear()
  static void LevelShutdownPreEntityAllSystems();
  static void LevelShutdownPostEntityAllSystems();

  static void OnSaveAllSystems();
  static void OnRestoreAllSystems();

  static void SafeRemoveIfDesiredAllSystems();

  static void PreRenderAllSystems();
  static void UpdateAllSystems( float frametime );
  static void PostRenderAllSystems();
};

class ClientClass {
 public:
	const char* GetName() {
		return *(char**)(this + 0x8);
	}
	RecvTable* GetTable() {
		return *(RecvTable**)(this + 0xC);
	}
	ClientClass* NextClass() {
		return *(ClientClass**)(this + 0x10);
	}
	int GetClassID() {
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

class EngineClient {
public:
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
	int GetLocalPlayerIndex() {
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return utils::GetVtableFunction<OriginalFn>( this, 12 )( this );
	}
	float Time() {
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
	bool IsInGame() {
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

class IPanel {
public:
	const char *GetName(unsigned int vguiPanel) {
		typedef const char* ( __thiscall* OriginalFn )( PVOID, unsigned int );
		return utils::GetVtableFunction<OriginalFn>( this, 40 )( this, vguiPanel );
	}
	void PaintTraverse(int unknw, unsigned int vguiPanel, bool forceRepaint, bool allowForce) {
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
  dota::BaseEntity* GetClientEntity( int entnum ) {
    typedef dota::BaseEntity* ( __thiscall* OriginalFn )( PVOID, int );
    return utils::GetVtableFunction<OriginalFn>( this, 3 )( this, entnum );
  }
  dota::BaseEntity* GetClientEntityFromHandle( int hEnt ) {
    typedef dota::BaseEntity* ( __thiscall* OriginalFn )( PVOID, int );
    return utils::GetVtableFunction<OriginalFn>( this, 4 )( this, hEnt );
  }
  int GetHighestEntityIndex(void) {
    typedef int ( __thiscall* OriginalFn )( PVOID );
    return utils::GetVtableFunction<OriginalFn>( this, 6 )( this );
  }
};

class Vgui_IInput {
public:
  bool IsKeyDown( int key_code ) {
    typedef int ( __thiscall* OriginalFn )( PVOID, int );
    return utils::GetVtableFunction<OriginalFn>( this, 18 )( this, key_code );
  }
};

class ILocalize {
public:
  wchar_t *Find(char const *tokenName) {
    typedef wchar_t* ( __thiscall* OriginalFn )( PVOID, char const* );
    return utils::GetVtableFunction<OriginalFn>( this, 12 )( this, tokenName );
  }
};