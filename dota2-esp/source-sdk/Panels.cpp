#include "SDK.h"
#include "Panels.h"
#include "draw_utils.h"
#include "../utils/vmthooks.h"

//CON_COMMAND( set_var, "Change cvar value" )
//{
//	if ( args.ArgC() < 3 )
//	{
//		return;
//	}
//	ConVar* temp = g_pCvar->FindVar(args.Arg(1));
//	temp->SetValue(atoi(args.Arg(2)));
//}
//
//CON_COMMAND( rosh, "" )
//{
//	char temp[160];
//	float t = roshSpawnTime-g_pEngine->GetGameTime();
//  int t2 = 0;
//	switch (t2)
//	{
//	case 1:
//		if(roshKilled) sprintf(temp,"say_team \"rosha cherez %.0f min\"",floor(fmod(t,3600.0f)/60.0f));
//		else sprintf(temp,"say_team \"rosha jiv!\"");
//		break;
//	case 2:
//		if(roshKilled) sprintf(temp,"say_team \"roshan after %.0f min\"",floor(fmod(t,3600.0f)/60.0f));
//		else sprintf(temp,"say_team \"roshan is alive!\"");
//		break;
//	}
//	g_pEngine->ClientCmd_Unrestricted(temp);
//}

//===================================================================================