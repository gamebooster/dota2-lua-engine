#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdexcept>
#include <d3d9.h>

#include "..\AntTweakBar\include\AntTweakBar.h"

#include "dota\DotaPlayerResource.h"
#include "dota\DotaChat.h"

#include "utils\utils.h"
#include "utils\vmthooks.h"

#include "source-sdk\SDK.h"
#include "source-sdk\Panels.h"
#include "source-sdk\draw_utils.h"
#include "source-sdk\netvar.h"
#include "source-sdk\global_instance_manager.h"

extern "C" LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
extern "C" LRESULT CALLBACK WinHookProc(int nCode, WPARAM wParam, LPARAM lParam);
DWORD __stdcall InstallHook(void*);
void ReleaseHook();

static TwBar *pBar = nullptr;
static HWND targetWindow = nullptr;
static WNDPROC OldWindowProc = nullptr;

static vtable::VMTManager* hook_manager;

//offsets
DWORD dwm_iHealth;
DWORD dwm_flMana;
DWORD dwm_flMaxMana;
DWORD dwm_hReplicatingOtherHeroModel;
DWORD dwm_fGameTime;
DWORD dwGameRules;

sourcesdk::NetVarManager* netvar_manager = nullptr;
DrawUtils* draw_utils = nullptr;
vtable::VMTManager* panel_hook;

void FillOffsets() {
	dwm_iHealth = netvar_manager->GetNetVarOffset("DT_DOTA_BaseNPC","m_iHealth");
	dwm_flMana = netvar_manager->GetNetVarOffset("DT_DOTA_BaseNPC","m_flMana");
	dwm_flMaxMana = netvar_manager->GetNetVarOffset("DT_DOTA_BaseNPC","m_flMaxMana");
	dwm_hReplicatingOtherHeroModel = netvar_manager->GetNetVarOffset("DT_DOTA_BaseNPC_Hero","m_hReplicatingOtherHeroModel");
	dwm_fGameTime = netvar_manager->GetNetVarOffsetL("DT_DOTAGamerulesProxy", "dota_gamerules_data", "m_fGameTime");
	dwGameRules = (DWORD)utils::FindPattern(
    "client.dll",
    reinterpret_cast<unsigned char*>("\x8B\x0D\x00\x00\x00\x00\x85\xC9\x74\x08\x8B\x01\x8B\x10\x6A\x01\xFF\xD2\x56\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00"),
    "xx????xxxxxxxxxxxxxxx????????",
    0x2);
	utils::Log("found dwGameRules at [0x%X]",dwGameRules);
}

		void __stdcall Hooked_PaintTraverseNew(int unknw, unsigned int vguiPanel, bool forceRepaint, bool allowForce );


DWORD WINAPI InitializeHook( LPVOID lpArguments ) {
		while(utils::GetModuleHandleSafe("engine.dll" ) == NULL
      &&  utils::GetModuleHandleSafe( "client.dll" ) == NULL
      &&  utils::GetModuleHandleSafe( "steamclient.dll" ) == NULL) {
			Sleep( 100 );
    }

		netvar_manager = new sourcesdk::NetVarManager(GlobalInstanceManager::GetClient());

		CreateInterface_t VStdFactory = ( CreateInterfaceFn ) GetProcAddress( utils::GetModuleHandleSafe( "vstdlib.dll" ), "CreateInterface" );
		CEngineCVar* engine_cvar = ( CEngineCVar* ) VStdFactory( "VEngineCvar007", NULL );


    draw_utils = new DrawUtils(GlobalInstanceManager::GetSurface(), GlobalInstanceManager::GetSurfaceNew(), GlobalInstanceManager::GetEngineClient());

		panel_hook = new vtable::VMTManager(GlobalInstanceManager::GetPanel());
		panel_hook->HookMethod(Hooked_PaintTraverseNew, 45 );
			
	
	return 0; 
}

void FinalizeHook() {
  if (panel_hook != nullptr) {
    panel_hook->Unhook();
    delete panel_hook;
  }
}

void __stdcall Hooked_PaintTraverseNew(int unknw, unsigned int vguiPanel, bool forceRepaint, bool allowForce )
{
  typedef void ( __thiscall* OriginalFn )( PVOID, int, unsigned int, bool, bool);
  panel_hook->GetMethod<OriginalFn>(45)(GlobalInstanceManager::GetPanel(), unknw, vguiPanel, forceRepaint, allowForce);

  static unsigned int vguiMatSystemTopPanel;

  //Init
  if (vguiMatSystemTopPanel == NULL)
  {
    const char* szName = GlobalInstanceManager::GetPanel()->GetName(vguiPanel);
    if( szName[0] == 'M' && szName[3] == 'S' ) //Look for MatSystemTopPanel without using slow operations like strcmp or strstr.
      vguiMatSystemTopPanel = vguiPanel;
    FillOffsets();
    //	g_pEngine->ClientCmd_Unrestricted("clear");
    //	if(!g_pEngine->Con_IsVisible( ))
    //	g_pEngine->ClientCmd_Unrestricted("toggleconsole");		g_pCvar->RegisterConCommand(&set_var_command);
    //	g_pCvar->RegisterConCommand(&rosh_command);
    //	new CRoshListener();
    return;
  }

  //Draw ESP
  if ( vguiMatSystemTopPanel == vguiPanel ) //If we're on MatSystemTopPanel, call our drawing code.
  {
    //if(GetAsyncKeyState(VK_DELETE)&1)
    //{
    //  ConVar* cvr = g_pCvar->FindVar("dota_camera_distance");
    //  cvr->SetValue(1400);
    //}
    if( GlobalInstanceManager::GetEngineClient()->IsDrawingLoadingImage() ||
        !GlobalInstanceManager::GetEngineClient()->IsInGame( ) ||
        !GlobalInstanceManager::GetEngineClient()->IsConnected() ||
        GlobalInstanceManager::GetEngineClient()->Con_IsVisible( ) ||
        ( ( GetAsyncKeyState(VK_F12) || GlobalInstanceManager::GetEngineClient()->IsTakingScreenshot( ) ) ) )
    {
      return; //We don't want to draw at the menu.
    }

      for( int i = 1; i < GlobalInstanceManager::GetClientEntityList()->GetHighestEntityIndex(); i++ )
      {
        CBaseEntity *pBaseEntity = GlobalInstanceManager::GetClientEntityList()->GetClientEntity(i);

        if( pBaseEntity == NULL )
          continue;

        const char* HeroName = pBaseEntity->GetClientClass()->GetName();
                  utils::Log("hook: %s", HeroName);

        if(HeroName[0] != 'C' && HeroName[6] != 'U')
          continue;
        if( HeroName[0] == 'C' && HeroName[6] == 'U' && HeroName[11] == 'H' )
        {
          int health = pBaseEntity->GetHealth();

          if(health <= 0)
            continue;

          float manaMax = pBaseEntity->GetManaMax();
          if(manaMax == 0)
            continue;
          float mana = pBaseEntity->GetMana();

          int barWidth = 100;
          int lifeWidth = (int)(mana*100/manaMax);

          Vector vecScreen;
          if(!pBaseEntity->IsIllusion())
            if ( draw_utils->WorldToScreenNew(pBaseEntity->GetAbsOrigin(), vecScreen) )
            {
              draw_utils->DrawRect(vecScreen.x-45,vecScreen.y-20-25,barWidth,10,0,0,0,255);
              draw_utils->DrawRect(vecScreen.x-45,vecScreen.y-20-25,lifeWidth,10,0,128,255,255);
              draw_utils->OutlineRect(vecScreen.x-45-1,vecScreen.y-20-25-1,barWidth+2,10+2,0,0,0,255);
              draw_utils->DrawString(vecScreen.x-45+(barWidth/2),vecScreen.y-23-25, 255,255,255,255, true, "%d / %d",(int)mana,(int)manaMax);
            }
        }
      }
    
  }
}

HRESULT __stdcall hEndScene(IDirect3DDevice9 *thisPtr) {
	HRESULT rval = S_OK;

	rval = hook_manager->GetMethod<HRESULT (__stdcall*)( void*)>(42)(thisPtr);

  TwDraw();

	return rval;
}

HRESULT __stdcall hReset(IDirect3DDevice9 *thisPtr, D3DPRESENT_PARAMETERS* pPresentationParameters) {
	HRESULT rval = S_OK;

	TwWindowSize(0,0);
	rval = hook_manager->GetMethod<HRESULT (__stdcall*)( void*, void*)>(16)(thisPtr, pPresentationParameters);
	TwWindowSize(pPresentationParameters->BackBufferWidth, pPresentationParameters->BackBufferHeight);

	
	targetWindow = pPresentationParameters->hDeviceWindow;
  if( NULL != OldWindowProc && NULL != targetWindow ) {
	  SetWindowLongPtr(targetWindow,GWLP_WNDPROC,(LONG_PTR)OldWindowProc);
	}
	if( targetWindow != NULL) {
			OldWindowProc = (WNDPROC)SetWindowLongPtr(targetWindow,GWLP_WNDPROC,(LONG_PTR)WindowProc);
	}

	return rval;
}


int WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved) {
	HANDLE thread = nullptr;

	if( DLL_PROCESS_ATTACH == fdwReason ) {
		thread = CreateThread( NULL, 0, 
			InitializeHook, 0, 0, NULL);  
	} else if (DLL_PROCESS_DETACH == fdwReason) {
	 
    FinalizeHook();
    //ReleaseHook();
	}
	return TRUE;
}

extern "C" LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int handled = 0;
	handled = TwEventWin(hWnd,message,wParam,lParam);
	return handled ? 0 : CallWindowProc(OldWindowProc, hWnd, message, wParam, lParam);
}

int StringToWString(std::wstring &ws, const std::string &s) {
  std::wstring wsTmp(s.begin(), s.end());

  ws = wsTmp;

  return 0;
}

IDirect3DDevice9* GetDirect9Device() {
  const uint32_t* device_address = utils::FindPattern(
   _T("shaderapidx9.dll"),
    reinterpret_cast<unsigned char*>("\x74\x14\x8B\x0D\x00\x00\x00\x00\x8B\x11\x6a\x00"),
    "xxxx????xxxx",
    0x4); // shaderapidx9.dll + 0x0103D64;
  if (device_address == nullptr) return nullptr;
  IDirect3DDevice9* device = reinterpret_cast<IDirect3DDevice9*>(*device_address);
  return device;
}

DWORD __stdcall InstallHook(void*) {
	OutputDebugString("hook: InstallHook() called.\n");

  IDirect3DDevice9* device = GetDirect9Device();
  if (device == nullptr) {
    OutputDebugStringW(L"hook: GetDirect9Device() failed.\n");
    return 0;
  }

  hook_manager = new vtable::VMTManager(device);
  hook_manager->HookMethod(&hReset, 16);
  hook_manager->HookMethod(&hEndScene, 42);

	D3DDEVICE_CREATION_PARAMETERS cparams;
  RECT r;

  device->GetCreationParameters(&cparams);
  targetWindow = cparams.hFocusWindow;
  if( targetWindow != NULL) {
    OldWindowProc = (WNDPROC)SetWindowLongPtr(targetWindow,GWLP_WNDPROC,(LONG_PTR)WindowProc);
  }
  GetWindowRect(cparams.hFocusWindow, &r);
  TwWindowSize(r.right - r.left, r.bottom - r.top);

  TwInit(TW_DIRECT3D9, device);
  pBar = TwNewBar("Settings");

	return 0;
}

void ReleaseHook() {
  	OutputDebugStringW(L"hook: ReleaseHook() called.\n");
	if( NULL != OldWindowProc && NULL != targetWindow ) {
	  SetWindowLongPtr(targetWindow,GWLP_WNDPROC,(LONG_PTR)OldWindowProc);
	}
  hook_manager->Unhook();
  delete hook_manager;
  TwTerminate();
}