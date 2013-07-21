#include <windows.h>
#include <stdio.h>
#include <stdexcept>

#include "dota\DotaPlayerResource.h"
#include "dota\DotaChat.h"

#include "utils\utils.h"
#include "utils\vmthooks.h"

#include "source-sdk\SDK.h"
#include "source-sdk\Panels.h"
#include "source-sdk\draw_utils.h"
#include "source-sdk\netvar.h"
#include "source-sdk\global_instance_manager.h"

DWORD dwm_iHealth;
DWORD dwm_flMana;
DWORD dwm_flMaxMana;
DWORD dwm_hReplicatingOtherHeroModel;
DWORD dwm_fGameTime;
DWORD dwGameRules;

static sourcesdk::NetVarManager* netvar_manager = nullptr;
static sourcesdk::DrawUtils* draw_utils = nullptr;
static utils::VMTManager* panel_hook;

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

		CreateInterface_t VStdFactory = ( CreateInterfaceFn ) GetProcAddress(
      utils::GetModuleHandleSafe( "vstdlib.dll" ), "CreateInterface" );
		CEngineCVar* engine_cvar = ( CEngineCVar* ) VStdFactory( "VEngineCvar007", NULL );


    draw_utils = new sourcesdk::DrawUtils(
      GlobalInstanceManager::GetSurface(),
      GlobalInstanceManager::GetSurfaceNew(),
      GlobalInstanceManager::GetEngineClient());

		panel_hook = new utils::VMTManager(GlobalInstanceManager::GetPanel());
		panel_hook->HookMethod(Hooked_PaintTraverseNew, 45 );
			
	
	return 0; 
}

void __stdcall Hooked_PaintTraverseNew(int unknw, unsigned int vguiPanel, bool forceRepaint, bool allowForce ) {
  typedef void ( __thiscall* OriginalFn )( PVOID, int, unsigned int, bool, bool);
  panel_hook->GetMethod<OriginalFn>(45)(GlobalInstanceManager::GetPanel(), unknw, vguiPanel, forceRepaint, allowForce);

  static unsigned int vguiMatSystemTopPanel;

  if (vguiMatSystemTopPanel == NULL) {
    const char* szName = GlobalInstanceManager::GetPanel()->GetName(vguiPanel);
    if( szName[0] == 'M' && szName[3] == 'S' ) vguiMatSystemTopPanel = vguiPanel;
    FillOffsets();
    return;
  }


  if ( vguiMatSystemTopPanel == vguiPanel ) {

    if( GlobalInstanceManager::GetEngineClient()->IsDrawingLoadingImage() ||
        !GlobalInstanceManager::GetEngineClient()->IsInGame( ) ||
        !GlobalInstanceManager::GetEngineClient()->IsConnected() ||
        GlobalInstanceManager::GetEngineClient()->Con_IsVisible( ) ||
        ( ( GetAsyncKeyState(VK_F12) || GlobalInstanceManager::GetEngineClient()->IsTakingScreenshot( ) ) ) )
    {
      return; //We don't want to draw at the menu.
    }

      for( int i = 1; i < GlobalInstanceManager::GetClientEntityList()->GetHighestEntityIndex(); i++ ) {
        CBaseEntity *pBaseEntity = GlobalInstanceManager::GetClientEntityList()->GetClientEntity(i);

        if( pBaseEntity == NULL )
          continue;

        const char* HeroName = pBaseEntity->GetClientClass()->GetName();

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

void FinalizeHook() {
  if (panel_hook != nullptr) {
    panel_hook->Unhook();
    delete panel_hook;
  }
  if (draw_utils != nullptr) {
    delete draw_utils;
  }
  if (netvar_manager != nullptr) {
    delete netvar_manager;
  }
}

int WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved) {
	HANDLE thread = nullptr;

	if( DLL_PROCESS_ATTACH == fdwReason ) {
		CreateThread( NULL, 0, InitializeHook, 0, 0, NULL);  
	} else if (DLL_PROCESS_DETACH == fdwReason) {
    FinalizeHook();
	}
	return TRUE;
}