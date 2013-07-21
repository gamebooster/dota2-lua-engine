#pragma once

#include <windows.h>
#include <math.h>
#include <xstring>
#include "Vector.h"
#include "getvfunc.h"
#include "dt_recv2.h"
#include "Psapi.h"
#include "convar.h"

using namespace std;

typedef void* ( __cdecl* CreateInterface_t )( const char*, int* );
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

#define WIN32_LEAN_AND_MEAN
#pragma optimize("gsy",on)
#pragma warning( disable : 4244 ) //Possible loss of data

typedef float matrix3x4[3][4];
typedef short int16;
typedef unsigned short uint16;
typedef unsigned __int64 uint64;
typedef unsigned int uint32;
typedef int int32;
class model_t;

//GC
struct GCMsgHeaderPB_t
{
	uint32 msg;
	int32 headerLength;
};
struct CMsgGCBase
{
	enum { k_iMessage };
	GCMsgHeaderPB_t header;
	byte proto[4096];
};
//GC

typedef struct player_info_s
{
	uint64			xuid;
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

class ClientClass
{
public:
	const char* GetName( void )
	{
		return *(char**)(this+0x8);
	}
	RecvTable* GetTable( )
	{
		return *(RecvTable**)(this+0xC);
	}
	ClientClass* NextClass( )
	{
		return *(ClientClass**)(this+0x10);
	}
	int GetClassID( void )
	{
		return *(int*)(this+0x14);
	}
};

class CHLClient
{
public:
	void LevelInitPreEntity( char const* pMapName )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, char const*);
		getvfunc<OriginalFn>( this, 4 )( this, pMapName);
	}
	void HudUpdate( bool bActive )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, bool);
		getvfunc<OriginalFn>( this, 10 )( this, bActive);
	}
	ClientClass* GetAllClasses( void )
	{
		typedef ClientClass* ( __thiscall* OriginalFn )( PVOID ); //Anything inside a VTable is a __thiscall unless it completly disregards the thisptr. You can also call them as __stdcalls, but you won't have access to the __thisptr.
		return getvfunc<OriginalFn>( this, 7 )( this ); //Return the pointer to the head CClientClass.
	}
};
extern DWORD dwm_iHealth;
extern DWORD dwm_flMana;
extern DWORD dwm_flMaxMana;
extern DWORD dwm_hReplicatingOtherHeroModel;
extern DWORD dwm_fGameTime;
extern DWORD dwGameRules;
class CBaseEntity
{
public:
	int GetHealth()
	{
		return *( PINT )( ( DWORD )( this ) + dwm_iHealth );
	}
	float GetManaMax()
	{
		return *( PFLOAT )( ( DWORD )( this ) + dwm_flMaxMana );
	}
	float GetMana()
	{
		return *( PFLOAT )( ( DWORD )( this ) + dwm_flMana );
	}
	bool IsIllusion()
	{
		if ((*( PINT )( ( DWORD )( this ) + dwm_hReplicatingOtherHeroModel )) <= 0 )
			return false;
		else
			return true;
	}
	Vector& GetAbsOrigin( )
	{
		typedef Vector& ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>(this, 11)(this);
	}
	Vector& GetAbsAngles( )
	{
		typedef Vector& ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>(this, 10)(this);
	}
	const char* GetName( )
	{
		return (const char*)((DWORD)this+0x11A4);//ollyfind ", NAME: "
	}
	void GetWorldSpaceCenter( Vector& vWorldSpaceCenter)
	{
		Vector vMin, vMax;
		this->GetRenderBounds( vMin, vMax );
		vWorldSpaceCenter = this->GetAbsOrigin();
		vWorldSpaceCenter.z += (vMin.z + vMax.z) / 2;
	}
	model_t* GetModel( )
	{
		PVOID pRenderable = (PVOID)(this + 0x4);
		typedef model_t* ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pRenderable, 8 )( pRenderable );
	}
	bool SetupBones( matrix3x4 *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime )
	{
		PVOID pRenderable = (PVOID)(this + 0x4);
		typedef bool ( __thiscall* OriginalFn )( PVOID, matrix3x4*, int, int, float );
		return getvfunc<OriginalFn>( pRenderable, 16 )( pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime );
	}
	ClientClass* GetClientClass( )
	{
		PVOID pNetworkable = (PVOID)(this + 0x8);
		typedef ClientClass* ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pNetworkable, 2 )( pNetworkable );
	}
	bool IsDormant( )
	{
		PVOID pNetworkable = (PVOID)(this + 0x8);
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pNetworkable, 8 )( pNetworkable );
	}
	int GetIndex( )
	{
		PVOID pNetworkable = (PVOID)(this + 0x8);
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pNetworkable, 9 )( pNetworkable );
	}
	void GetRenderBounds( Vector& mins, Vector& maxs )
	{
		PVOID pRenderable = (PVOID)(this + 0x4);
		typedef void ( __thiscall* OriginalFn )( PVOID, Vector& , Vector& );
		getvfunc<OriginalFn>( pRenderable, 20 )( pRenderable, mins, maxs );
	}
};

class EngineClient
{
public:
	float GetGameTime( void )
	{
		DWORD GameRules = *( PDWORD )(dwGameRules);
		return *( PFLOAT )( GameRules + dwm_fGameTime );;
	}
	void GetScreenSize( int& width, int& height )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, int& , int& );
		return getvfunc<OriginalFn>( this, 5 )( this, width, height );
	}
	bool GetPlayerInfo( int ent_num, player_info_t *pinfo )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID, int, player_info_t * );
		return getvfunc<OriginalFn>(this, 8)(this, ent_num, pinfo );
	}
	bool Con_IsVisible( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 11 )( this );
	}
	int GetLocalPlayer( void )
	{
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 12 )( this );
	}
	float Time( void )
	{
		typedef float ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 14 )( this );
	}
	void GetViewAngles( Vector& va )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, Vector& va );
		return getvfunc<OriginalFn>( this, 19 )( this, va );
	}
	void SetViewAngles( Vector& va )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, Vector& va );
		return getvfunc<OriginalFn>( this, 20 )( this, va );
	}
	int GetMaxClients( void )
	{
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 21 )( this );
	}
	bool IsInGame( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 26 )( this );
	}
	bool IsConnected( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 27 )( this );
	}
	bool IsDrawingLoadingImage( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 28 )( this );
	}
	const matrix3x4& WorldToScreenMatrix( void )
	{
		typedef const matrix3x4& ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>(this, 37)(this);
	}
	bool IsTakingScreenshot( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 91 )( this );
	}
	DWORD* GetNetChannelInfo( void )
	{
		typedef DWORD* ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 72 )( this );
	}
	void ClientCmd_Unrestricted( const char* chCommandString )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, const char * );
		return getvfunc<OriginalFn>( this, 105 )( this, chCommandString );
	}
};

class CModelInfo
{
public:
	const char*	GetModelName( const model_t* pModel )
	{
		typedef const char* ( __thiscall* OriginalFn )( PVOID, const model_t* );
		return getvfunc<OriginalFn>( this, 3 )( this, pModel );
	}
};

class IPanel
{
public:
	const char *GetName(unsigned int vguiPanel)
	{
		typedef const char* ( __thiscall* OriginalFn )( PVOID, unsigned int );
		return getvfunc<OriginalFn>( this, 40 )( this, vguiPanel );
	}
	void PaintTraverse(int unknw, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, int, unsigned int, bool, bool);
		return getvfunc<OriginalFn>( this, 45 )( this, unknw, vguiPanel, forceRepaint, allowForce);
	}
};
///
enum EGCResults
{
	k_EGCResultOK = 0,
	k_EGCResultNoMessage = 1,			// There is no message in the queue
	k_EGCResultBufferTooSmall = 2,		// The buffer is too small for the requested message
	k_EGCResultNotLoggedOn = 3,			// The client is not logged onto Steam
	k_EGCResultInvalidMessage = 4,		// Something was wrong with the message being sent with SendMessage
};
typedef uint32 AppId_t;
class IGCClient
{
public:
	EGCResults SendMess( uint32 unMsgType, const void *pubData, uint32 cubData )
	{
		typedef EGCResults ( __thiscall* OriginalFn )( PVOID, uint32 unMsgType, const void *pubData, uint32 cubData );
		return getvfunc<OriginalFn>( this, 0 )( this, unMsgType, pubData, cubData);
	}

	EGCResults RetrieveMessage( uint32 *punMsgType, void *pubDest, uint32 cubDest, uint32 *pcubMsgSize )
	{
		typedef EGCResults ( __thiscall* OriginalFn )( PVOID, uint32 *punMsgType, void *pubDest, uint32 cubDest, uint32 *pcubMsgSize );
		return getvfunc<OriginalFn>( this, 2 )( this, punMsgType, pubDest, cubDest, pcubMsgSize);
	}
};

class CEngineCVar
{
public:
	void RegisterConCommand( ConCommandBase *pCommandBase )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, ConCommandBase *pCommandBase );
		return getvfunc<OriginalFn>( this, 10 )( this, pCommandBase );
	}
	ConVar* FindVar( const char* szVar )
	{
		typedef ConVar* ( __thiscall* OriginalFn )( PVOID, const char* szVar );
		return getvfunc<OriginalFn>( this, 14 )( this, szVar );
	}
};

class ISurfaceNew
{
public:
	void DrawSetColor(int r, int g, int b, int a)
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, int, int, int, int );
		getvfunc<OriginalFn>( this, 17 )( this, r, g, b, a );
	}
	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, int, int, int, int );
		getvfunc<OriginalFn>( this, 21 )( this, x0, y0, x1, y1 );
	}
	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, int, int, int, int );
		getvfunc<OriginalFn>( this, 19 )( this, x0, y0, x1, y1 );
	}
	void DrawSetTextFont(unsigned long font)
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, unsigned long );
		getvfunc<OriginalFn>( this, 13 )( this, font );
	}
	void DrawSetTextColor(int r, int g, int b, int a )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, int, int, int, int );
		getvfunc<OriginalFn>( this, 26 )( this, r, g, b, a );
	}
	void DrawSetTextPos(int x, int y )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, int, int );
		getvfunc<OriginalFn>( this, 27 )( this, x, y );
	}
	void DrawPrintText(const wchar_t *text, int textLen )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, const wchar_t *, int, int );
		return getvfunc<OriginalFn>( this, 28 )( this, text, textLen, 0 );
	}
};

class ISurface
{
public:
	void GetTextSize(unsigned long font,const wchar_t *text,int &wide, int &tall)
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, unsigned long,const wchar_t *,int &,int &);
		getvfunc<OriginalFn>( this, 60 )( this, font, text, wide, tall );
	}
	unsigned long CreateFont( )
	{
		typedef unsigned int ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 52 )( this );
	}
	void SetFontGlyphSet (unsigned long &font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, unsigned long, const char*, int, int, int, int, int, int, int );
		getvfunc<OriginalFn>( this, 53 )( this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0 );
	}
};

class CEntList
{
public:
	CBaseEntity* GetClientEntity( int entnum )
	{
		typedef CBaseEntity* ( __thiscall* OriginalFn )( PVOID, int );
		return getvfunc<OriginalFn>( this, 3 )( this, entnum );
	}
	CBaseEntity* GetClientEntityFromHandle( int hEnt )
	{
		typedef CBaseEntity* ( __thiscall* OriginalFn )( PVOID, int );
		return getvfunc<OriginalFn>( this, 4 )( this, hEnt );
	}
	int GetHighestEntityIndex(void)
	{
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 6 )( this );
	}
};

class IGameEvent
{
public:
	virtual ~IGameEvent() {};
	virtual const char *GetName() const = 0;	// get event name
	virtual bool  IsReliable() const = 0; // if event handled reliable
	virtual bool  IsLocal() const = 0; // if event is never networked
	virtual bool  IsEmpty(const char *keyName = NULL) = 0; // check if data field exists
	// Data access
	virtual bool  unk( const char *keyName = NULL, bool defaultValue = false ) = 0;
	virtual bool  GetBool( const char *keyName = NULL, bool defaultValue = false ) = 0;
	virtual int   GetInt( const char *keyName = NULL, int defaultValue = 0 ) = 0;
	virtual float GetFloat( const char *keyName = NULL, float defaultValue = 0.0f ) = 0;
	virtual const char *GetString( const char *keyName = NULL, const char *defaultValue = "" ) = 0;
	virtual void SetBool( const char *keyName, bool value ) = 0;
	virtual void SetInt( const char *keyName, int value ) = 0;
	virtual void SetFloat( const char *keyName, float value ) = 0;
	virtual void SetString( const char *keyName, const char *value ) = 0;
};
class IGameEventListener2
{
public:
	virtual	~IGameEventListener2( void ) {};
	// FireEvent is called by EventManager if event just occured
	// KeyValue memory will be freed by manager if not needed anymore
	virtual void FireGameEvent( IGameEvent *event ) = 0;
};
class IBaseInterface
{
public:
	virtual	~IBaseInterface() {}
};
class IGameEventManager2 : public IBaseInterface
{
public:
	virtual	~IGameEventManager2( void ) {};

	// load game event descriptions from a file eg "resource\gameevents.res"
	virtual int LoadEventsFromFile( const char *filename ) = 0;
	// removes all and anything
	virtual void  Reset() = 0;	
	// adds a listener for a particular event
	virtual bool AddListener( IGameEventListener2 *listener, const char *name, bool bServerSide ) = 0;
	// returns true if this listener is listens to given event
	virtual bool FindListener( IGameEventListener2 *listener, const char *name ) = 0;
	// removes a listener 
	virtual void RemoveListener( IGameEventListener2 *listener) = 0;
	// create an event by name, but doesn't fire it. returns NULL is event is not
	// known or no listener is registered for it. bForce forces the creation even if no listener is active
	virtual IGameEvent *CreateEvent( const char *name, bool bForce = false ) = 0;
	// fires a server event created earlier, if bDontBroadcast is set, event is not send to clients
	virtual bool FireEvent( IGameEvent *event, bool bDontBroadcast = false ) = 0;
	// fires an event for the local client only, should be used only by client code
	virtual bool FireEventClientSide( IGameEvent *event ) = 0;
	// create a new copy of this event, must be free later
	virtual IGameEvent *DuplicateEvent( IGameEvent *event ) = 0;
	// if an event was created but not fired for some reason, it has to bee freed, same UnserializeEvent
	virtual void FreeEvent( IGameEvent *event ) = 0;
	// write/read event to/from bitbuffer
	virtual bool SerializeEvent( IGameEvent *event, int *buf ) = 0;
	virtual IGameEvent *UnserializeEvent( int *buf ) = 0; // create new KeyValues, must be deleted
};