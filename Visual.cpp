// Don't take credits for this ;) Joplin / Manhhao are the first uploaders ;)

#include "Visuals.h"
#include "Interfaces.h"
#include "RenderManager.h"

void CVisuals::Init() {}

void CVisuals::Move(CUserCmd *pCmd, bool &bSendPacket) {}

void CVisuals::Draw()
{
	IClientEntity* pLocal = hackManager.pLocal();

	if (Menu::Window.VisualsTab.OtherCrosshair.GetState())
		DrawCrosshair();

	if (Menu::Window.VisualsTab.OtherRecoilCrosshair.GetIndex())
		DrawRecoilCrosshair();

	if (Menu::Window.VisualsTab.OtherNoScope.GetState() && pLocal->IsScoped() && pLocal->IsAlive() && Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) {
		RECT View = Render::GetViewport();
		int MidX = View.right / 2;
		int MidY = View.bottom / 2;
		Render::Line(MidX, MidY - 3000, MidX, MidY + 3000, Color(0, 0, 0, 255));
		Render::Line(MidX - 3000, MidY, MidX + 3000, MidY, Color(0, 0, 0, 255));
	}	
}

void CVisuals::DrawCrosshair() { 
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;
	Render::Line(MidX - 8, MidY - 8, MidX + 8, MidY + 8, Color(0, 0, 0, 200));
	Render::Line(MidX + 8, MidY - 8, MidX - 8, MidY + 8, Color(0, 0, 0, 200));
	Render::Line(MidX - 4, MidY - 4, MidX + 4, MidY + 4, Color(0, 255, 0, 255));
	Render::Line(MidX + 4, MidY - 4, MidX - 4, MidY + 4, Color(0, 255, 0, 255));
}

void CVisuals::DrawRecoilCrosshair() {

	if (Menu::Window.RageBotTab.AccuracyRecoil.GetState())
		return;

	IClientEntity *pLocal = hackManager.pLocal();

	Vector ViewAngles;
	Interfaces::Engine->GetViewAngles(ViewAngles);
	ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	Vector fowardVec;
	AngleVectors(ViewAngles, &fowardVec);
	fowardVec *= 10000;

	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = start + fowardVec, endScreen;

	if (Render::WorldToScreen(end, endScreen) && pLocal->IsAlive())
	{
		Render::Line(endScreen.x - 4, endScreen.y - 4, endScreen.x + 4, endScreen.y + 4, Color(0, 255, 0, 255));
		Render::Line(endScreen.x + 4, endScreen.y - 4, endScreen.x - 4, endScreen.y + 4, Color(0, 255, 0, 255));
		Render::Line(endScreen.x - 2, endScreen.y - 2, endScreen.x + 2, endScreen.y + 2, Color(0, 0, 0, 200));
		Render::Line(endScreen.x + 2, endScreen.y - 2, endScreen.x - 2, endScreen.y + 2, Color(0, 0, 0, 200));
	}
}