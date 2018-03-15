// Don't take credits for this ;) Joplin / Manhhao are the first uploaders ;)

#define _CRT_SECURE_NO_WARNINGS

#include "Hacks.h"
#include "Interfaces.h"
#include "RenderManager.h"

#include "ESP.h"
#include "Visuals.h"
#include "RageBot.h"
#include "MiscHacks.h"
#include "LegitBot.h"
#include "GameFunctions.h"

CEsp Esp;
CVisuals Visuals;
CMiscHacks MiscHacks;
CRageBot RageBot;
CLegitBot LegitBot;

// Initialise and register ALL hackmanager hacks in here nigga
void Hacks::SetupHacks()
{
	Esp.Init();
	Visuals.Init();
	MiscHacks.Init();
	RageBot.Init();
	LegitBot.Init();

	hackManager.RegisterHack(&Esp);
	hackManager.RegisterHack(&Visuals);
	hackManager.RegisterHack(&MiscHacks);
	hackManager.RegisterHack(&RageBot);
	hackManager.RegisterHack(&LegitBot);

	hackManager.Ready();
}

namespace game
{
	CGameFunctions functions;
}

void Hacks::DrawHacks()
{
	IClientEntity *pLocal = hackManager.pLocal();

	if (!Menu::Window.VisualsTab.Active.GetState())
		return;

	if (Menu::Window.VisualsTab.OptionsCompRank.GetState() && GUI.GetKeyState(VK_TAB))
	{
		GameUtils::ServerRankRevealAll();
	}

	hackManager.Draw();
	//--------------------------------
		
}

// Game Cmd Changes
void Hacks::MoveHacks(CUserCmd *pCmd,bool &bSendPacket)
{
	Vector origView = pCmd->viewangles;
	IClientEntity *pLocal = hackManager.pLocal();
	hackManager.Move(pCmd, bSendPacket);
	// ------------------------------
}

//---------------------------------------------------------------------//
HackManager hackManager;

// Register a new hack
void HackManager::RegisterHack(CHack* hake)
{
	Hacks.push_back(hake);
	hake->Init();
}

// Draw all the hakes
void HackManager::Draw()
{
	if (!IsReady)
		return;

	// Grab the local player for drawing related hacks
	pLocalInstance = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalInstance) return;

	for (auto &hack : Hacks)
	{
		hack->Draw();
	}
}

// Handle all the move hakes
void HackManager::Move(CUserCmd *pCmd,bool &bSendPacket)
{
	if (!IsReady)
		return;

	// Grab the local player for move related hacks
	pLocalInstance = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalInstance) return;

	for (auto &hack : Hacks)
	{
		hack->Move(pCmd,bSendPacket); // 
	}
}

//---------------------------------------------------------------------//
// Other Utils and shit

// Saves hacks needing to call a bunch of virtuals to get the instance
// Saves on cycles and file size. Recycle your plastic kids
IClientEntity* HackManager::pLocal()
{
	return pLocalInstance;
}

IClientEntity* HackManager::localplayer() {
	return Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
}

// Makes sure none of the hacks are called in their 
// hooks until they are completely ready for use
void HackManager::Ready()
{
	IsReady = true;
}