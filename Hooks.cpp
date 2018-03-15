// Don't take credits for this ;) Joplin / Manhhao are the first uploaders ;)

#include "Hooks.h"
#include "Hacks.h"
#include "Chams.h"
#include "Menu.h"
#include "CBulletListener.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "MiscHacks.h"
#include "CRC32.h"
#include "Resolver.h"
#include <intrin.h>

namespace Global
{
	 Vector LastAngle;
	 bool lbyupdate;
	 float curReal;
	 float curFake;
	 float hitmarkerAlpha;

}
Vector LastAngleAA;
float fakeangle;
QAngle realangle;
Vector LastAngleAAReal;
Vector LBYThirdpersonAngle;
int Globals::missedshots;

#pragma comment(lib, "Winmm.lib")
#define M_PI       3.14159265358979323846   // pi

#define STUDIO_RENDER					0x00000001



std::vector<trace_info> trace_logs;
// Funtion Typedefs
typedef void(__thiscall* DrawModelEx_)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4*);
typedef void(__thiscall* PaintTraverse_)(PVOID, unsigned int, bool, bool);
typedef bool(__thiscall* InPrediction_)(PVOID);
typedef void(__stdcall *FrameStageNotifyFn)(ClientFrameStage_t);
typedef bool(__thiscall *FireEventClientSideFn)(PVOID, IGameEvent*);
typedef void(__thiscall* RenderViewFn)(void*, CViewSetup&, CViewSetup&, int, int);
typedef void(__thiscall *SceneEnd_t)(void *pEcx);
using OverrideViewFn = void(__fastcall*)(void*, void*, CViewSetup*);
typedef float(__stdcall *oGetViewModelFOV)();


// Function Pointers to the originals
PaintTraverse_ oPaintTraverse;
DrawModelEx_ oDrawModelExecute;
FrameStageNotifyFn oFrameStageNotify;
OverrideViewFn oOverrideView;
FireEventClientSideFn oFireEventClientSide;
RenderViewFn oRenderView;
SceneEnd_t pSceneEnd;

// Hook function prototypes
void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
bool __stdcall Hooked_InPrediction();
bool __fastcall Hooked_FireEventClientSide(PVOID ECX, PVOID EDX, IGameEvent *Event);
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld);
bool __stdcall CreateMoveClient_Hooked(/*void* self, int edx,*/ float frametime, CUserCmd* pCmd);
void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage);
void __fastcall Hooked_OverrideView(void* ecx, void* edx, CViewSetup* pSetup);
float __stdcall GGetViewModelFOV();
void __fastcall Hooked_RenderView(void* ecx, void* edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw);
void __fastcall	hkSceneEnd(void *pEcx, void *pEdx);

// VMT Managers
namespace Hooks
{
	// VMT Managers
	Utilities::Memory::VMTManager VMTPanel; // Hooking drawing functions
	Utilities::Memory::VMTManager VMTClient; // Maybe CreateMove
	Utilities::Memory::VMTManager VMTClientMode; // CreateMove for functionality
	Utilities::Memory::VMTManager VMTModelRender; // DrawModelEx for chams
	Utilities::Memory::VMTManager VMTPrediction; // InPrediction for no vis recoil
	Utilities::Memory::VMTManager VMTPlaySound; // Autoaccept 
	Utilities::Memory::VMTManager VMTRenderView;
	Utilities::Memory::VMTManager VMTEventManager;
};

// Undo our hooks
void Hooks::UndoHooks()
{
	VMTPanel.RestoreOriginal();
	VMTPrediction.RestoreOriginal();
	VMTModelRender.RestoreOriginal();
	VMTClientMode.RestoreOriginal();
}

void Hooks::Initialise()
{
	VMTPanel.Initialise((DWORD*)Interfaces::Panels);
	oPaintTraverse = (PaintTraverse_)VMTPanel.HookMethod((DWORD)&PaintTraverse_Hooked, Offsets::VMT::Panel_PaintTraverse);

	VMTPrediction.Initialise((DWORD*)Interfaces::Prediction);
	VMTPrediction.HookMethod((DWORD)&Hooked_InPrediction, 14);

	VMTModelRender.Initialise((DWORD*)Interfaces::ModelRender);
	oDrawModelExecute = (DrawModelEx_)VMTModelRender.HookMethod((DWORD)&Hooked_DrawModelExecute, Offsets::VMT::ModelRender_DrawModelExecute);

	VMTClientMode.Initialise((DWORD*)Interfaces::ClientMode);
	VMTClientMode.HookMethod((DWORD)CreateMoveClient_Hooked, 24);

	oOverrideView = (OverrideViewFn)VMTClientMode.HookMethod((DWORD)&Hooked_OverrideView, 18);
	VMTClientMode.HookMethod((DWORD)&GGetViewModelFOV, 35);

	VMTClient.Initialise((DWORD*)Interfaces::Client);
	oFrameStageNotify = (FrameStageNotifyFn)VMTClient.HookMethod((DWORD)&Hooked_FrameStageNotify, 36);

	VMTEventManager.Initialise((DWORD*)Interfaces::EventManager);
	oFireEventClientSide = (FireEventClientSideFn)VMTEventManager.HookMethod((DWORD)&Hooked_FireEventClientSide, 9);

	VMTRenderView.Initialise((DWORD*)Interfaces::RenderView);
	pSceneEnd = (SceneEnd_t)VMTRenderView.HookMethod((DWORD)&hkSceneEnd, 9);

}

void SetClanTag(const char* tag, const char* name)//190% paste
{
	static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(((DWORD)Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x30", "xxxxxxxxx????xxxxxx")));
	pSetClanTag(tag, name);
}

void NoClantag()
{
	SetClanTag("", "");
}

void ClanTag()
{
	static int counter = 0;
	switch (Menu::Window.MiscTab.OtherClantag.GetIndex())
	{
	case 0:
		// No 
		break;
	case 1:
	{
		static int motion = 0;
		int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 2.5;

		if (counter % 48 == 0)
			motion++;
		int value = ServerTime % 19;
		switch (value) {
		case 0:SetClanTag("          ", "pasteware"); break;
		case 1:SetClanTag("         p", "pasteware"); break;
		case 2:SetClanTag("        pa", "pasteware"); break;
		case 3:SetClanTag("       pas", "pasteware"); break;
		case 4:SetClanTag("      past", "pasteware"); break;
		case 5:SetClanTag("     paste", "pasteware"); break;
		case 6:SetClanTag("    pastew", "pasteware"); break;
		case 7:SetClanTag("   pastewa", "pasteware"); break;
		case 8:SetClanTag("  pastewar", "pasteware"); break;
		case 9:SetClanTag(" pasteware", "pasteware"); break;
		case 10:SetClanTag("pasteware ", "pasteware"); break;
		case 11:SetClanTag("asteware  ", "pasteware"); break;
		case 12:SetClanTag("steware   ", "pasteware"); break;
		case 13:SetClanTag("teware    ", "pasteware"); break;
		case 14:SetClanTag("eware     ", "pasteware"); break;
		case 15:SetClanTag("ware      ", "pasteware"); break;
		case 16:SetClanTag("are       ", "pasteware"); break;
		case 17:SetClanTag("re        ", "pasteware"); break;
		case 18:SetClanTag("e         ", "pasteware"); break;
		case 19:SetClanTag("          ", "pasteware"); break;
		}
		counter++;
	}
	break;
	case 2:
	{
		static int motion = 0;
		int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 3;

		if (counter % 48 == 0)
			motion++;
		int value = ServerTime % 17;
		switch (value) {
		case 0:SetClanTag("          ", "skeet.cc"); break;
		case 1:SetClanTag("         s", "skeet.cc"); break;
		case 2:SetClanTag("        sk", "skeet.cc"); break;
		case 3:SetClanTag("       ske", "skeet.cc"); break;
		case 4:SetClanTag("      skee", "skeet.cc"); break;
		case 5:SetClanTag("     skeet", "skeet.cc"); break;
		case 6:SetClanTag("    skeet.", "skeet.cc"); break;
		case 7:SetClanTag("   skeet.c", "skeet.cc"); break;
		case 8:SetClanTag(" skeet.cc", "skeet.cc"); break;
		case 9:SetClanTag("skeet.cc ", "skeet.cc"); break;
		case 10:SetClanTag("keet.cc  ", "skeet.cc"); break;
		case 11:SetClanTag("eet.cc   ", "skeet.cc"); break;
		case 12:SetClanTag("et.cc    ", "skeet.cc"); break;
		case 13:SetClanTag("t.cc     ", "skeet.cc"); break;
		case 14:SetClanTag(".cc      ", "skeet.cc"); break;
		case 15:SetClanTag("cc       ", "skeet.cc"); break;
		case 16:SetClanTag("c        ", "skeet.cc"); break;
		case 17:SetClanTag("         ", "skeet.cc"); break;
		}
		counter++;
	}
	break;
	case 3:
		// stainless
		SetClanTag("\r", "\r");
		break;
	case 4:
		SetClanTag("[VALV\xE1\xB4\xB1]", "Valve");
		break;
	}
}


bool __stdcall CreateMoveClient_Hooked(/*void* self, int edx,*/ float frametime, CUserCmd* pCmd)
{
	if (!pCmd->command_number)
		return true;

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{

		PVOID pebp;
		__asm mov pebp, ebp;
		bool* pbSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);
		bool& bSendPacket = *pbSendPacket;

		if (Menu::Window.MiscTab.OtherClantag.GetIndex() > 0)
			ClanTag();

			// Backup for safety
		Vector origView = pCmd->viewangles;
		Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
		Vector qAimAngles;
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);

		// Do da hacks
		IClientEntity *pEntity;
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && pLocal && pLocal->IsAlive())
		{
			Hacks::MoveHacks(pCmd, bSendPacket);
			ResolverSetup::GetInst().CM(pEntity);
		}
			

		//Movement Fix
		//GameUtils::CL_FixMove(pCmd, origView);
		qAimAngles.Init(0.0f, GetAutostrafeView().y, 0.0f); // if pCmd->viewangles.x > 89, set pCmd->viewangles.x instead of 0.0f on first
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &aimforward, &aimright, &aimup);
		Vector vForwardNorm;		Normalize(viewforward, vForwardNorm);
		Vector vRightNorm;			Normalize(viewright, vRightNorm);
		Vector vUpNorm;				Normalize(viewup, vUpNorm);

		// Original shit for movement correction
		float forward = pCmd->forwardmove;
		float right = pCmd->sidemove;
		float up = pCmd->upmove;
		if (forward > 450) forward = 450;
		if (right > 450) right = 450;
		if (up > 450) up = 450;
		if (forward < -450) forward = -450;
		if (right < -450) right = -450;
		if (up < -450) up = -450;
		pCmd->forwardmove = DotProduct(forward * vForwardNorm, aimforward) + DotProduct(right * vRightNorm, aimforward) + DotProduct(up * vUpNorm, aimforward);
		pCmd->sidemove = DotProduct(forward * vForwardNorm, aimright) + DotProduct(right * vRightNorm, aimright) + DotProduct(up * vUpNorm, aimright);
		pCmd->upmove = DotProduct(forward * vForwardNorm, aimup) + DotProduct(right * vRightNorm, aimup) + DotProduct(up * vUpNorm, aimup);

		// Angle normalisation
		if (Menu::Window.MiscTab.OtherSafeMode.GetState())
		{
			GameUtils::NormaliseViewAngle(pCmd->viewangles);

			if (pCmd->viewangles.z != 0.0f)
			{
				pCmd->viewangles.z = 0.00;
			}

			if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
			{
				Utilities::Log("Having to re-normalise!");
				GameUtils::NormaliseViewAngle(pCmd->viewangles);
				Beep(750, 800); // Why does it do this
				if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
				{
					pCmd->viewangles = origView;
					pCmd->sidemove = right;
					pCmd->forwardmove = forward;
				}
			}
		}

		if (pCmd->viewangles.x > 90)
		{
			pCmd->forwardmove = -pCmd->forwardmove;
		}

		if (pCmd->viewangles.x < -90)
		{
			pCmd->forwardmove = -pCmd->forwardmove;
		}

		switch (Menu::Window.MiscTab.OtherThirdpersonType.GetIndex())
		{
		case 0:
			if (bSendPacket)
				fakeangle = pCmd->viewangles.y;
			break;
		case 1:
			if (!bSendPacket)
				fakeangle = pCmd->viewangles.y;
			break;
		}

		switch (Menu::Window.MiscTab.OtherThirdpersonType.GetIndex())
		{
		case 0:
			if (!bSendPacket)
				LastAngleAA = pCmd->viewangles;
			break;
		case 1:
			if (bSendPacket)
				LastAngleAA = pCmd->viewangles;
			break;
		}

		lineLBY = pLocal->GetLowerBodyYaw();
		if (bSendPacket == true) {
			lineFakeAngle = pCmd->viewangles.y;
		}
		else {
			lineRealAngle = pCmd->viewangles.y;
		}
	}

	return false;
}


// Paint Traverse Hooked function
void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	if (Menu::Window.VisualsTab.OtherNoScope.GetState() && !strcmp("HudZoom", Interfaces::Panels->GetName(vguiPanel)))
		return;

	oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);

	static unsigned int FocusOverlayPanel = 0;
	static bool FoundPanel = false;

	if (!FoundPanel)
	{
		PCHAR szPanelName = (PCHAR)Interfaces::Panels->GetName(vguiPanel);
		if (strstr(szPanelName, "MatSystemTopPanel"))
		{
			FocusOverlayPanel = vguiPanel;
			FoundPanel = true;
		}
	}
	else if (FocusOverlayPanel == vguiPanel)
	{
		//Render::GradientV(8, 8, 160, 18, Color(0, 0, 0, 0), Color(7, 39, 17, 255));
		Render::Text(10, 10, Color(255, 255, 255, 220), Render::Fonts::Menu, "Sloww.xyz");
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
			Hacks::DrawHacks();

		// Update and draw the menu
		Menu::DoUIFrame();
	}
}

// InPrediction Hooked Function
bool __stdcall Hooked_InPrediction()
{
	bool result;
	static InPrediction_ origFunc = (InPrediction_)Hooks::VMTPrediction.GetOriginalFunction(14);
	static DWORD *ecxVal = Interfaces::Prediction;
	result = origFunc(ecxVal);

	// If we are in the right place where the player view is calculated
	// Calculate the change in the view and get rid of it
	if (Menu::Window.VisualsTab.OtherNoVisualRecoil.GetState() && (DWORD)(_ReturnAddress()) == Offsets::Functions::dwCalcPlayerView)
	{
		IClientEntity* pLocalEntity = NULL;

		float* m_LocalViewAngles = NULL;

		__asm
		{
			MOV pLocalEntity, ESI
			MOV m_LocalViewAngles, EBX
		}

		Vector viewPunch = pLocalEntity->localPlayerExclusive()->GetViewPunchAngle();
		Vector aimPunch = pLocalEntity->localPlayerExclusive()->GetAimPunchAngle();

		m_LocalViewAngles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
		m_LocalViewAngles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
		m_LocalViewAngles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));
		return true;
	}

	return result;
}

bool __fastcall Hooked_FireEventClientSide(PVOID ECX, PVOID EDX, IGameEvent *Event)
{
	CBulletListener::singleton()->OnStudioRender();
	return oFireEventClientSide(ECX, Event);
}

void Hooks::DrawBeamd(Vector src, Vector end, Color color)
{
	BeamInfo_t beamInfo;
	beamInfo.m_nType = TE_BEAMPOINTS;
	beamInfo.m_pszModelName = "sprites/physbeam.vmt";
	beamInfo.m_nModelIndex = -1;
	beamInfo.m_flHaloScale = 0.0f;
	beamInfo.m_flLife = 3.0f;
	beamInfo.m_flWidth = 7.0f;
	beamInfo.m_flEndWidth = 7.0f;
	beamInfo.m_flFadeLength = 0.0f;
	beamInfo.m_flAmplitude = 2.0f;
	beamInfo.m_flBrightness = color.a();
	beamInfo.m_flSpeed = 0.2f;
	beamInfo.m_nStartFrame = 0.f;
	beamInfo.m_flFrameRate = 0.f;
	beamInfo.m_flRed = color.r();
	beamInfo.m_flGreen = color.g();
	beamInfo.m_flBlue = color.b();
	beamInfo.m_nSegments = 2;
	beamInfo.m_bRenderable = true;
	beamInfo.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;
	beamInfo.m_vecStart = src;
	beamInfo.m_vecEnd = end;

	Beam_t* myBeam = Interfaces::g_pViewRenderBeams->CreateBeamPoints(beamInfo);

	if (myBeam)
		Interfaces::g_pViewRenderBeams->DrawBeam(myBeam);
}

// DrawModelExec for chams and shit
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
	Color color;
	float flColor[3] = { 0.f };
	static IMaterial* CoveredLit = CreateMaterial(true);
	static IMaterial* OpenLit = CreateMaterial(false);
	static IMaterial* CoveredFlat = CreateMaterial(true, false);
	static IMaterial* OpenFlat = CreateMaterial(false, false);
	bool DontDraw = false;

	const char* ModelName = Interfaces::ModelInfo->GetModelName((model_t*)pInfo.pModel);
	IClientEntity* pModelEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(pInfo.entity_index);
	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Menu::Window.VisualsTab.Active.GetState())
	{
		// Player Chams
		int ChamsStyle = Menu::Window.VisualsTab.OptionsChams.GetIndex();
		int HandsStyle = Menu::Window.VisualsTab.OtherNoHands.GetIndex();
		if (ChamsStyle != 0 && Menu::Window.VisualsTab.FiltersPlayers.GetState() && strstr(ModelName, "models/player"))
		{
			if (pModelEntity == nullptr)
				return; //
			{
				if (pLocal && (!Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() ||
					pModelEntity->GetTeamNum() != pLocal->GetTeamNum()))
				{
					IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
					IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;

					IClientEntity* pModelEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(pInfo.entity_index);
					if (pModelEntity)
					{
						IClientEntity *local = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
						if (local)
						{
							if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0)
							{
								float alpha = 1.f;

								if (pModelEntity->HasGunGameImmunity())
									alpha = 0.5f;


								if (pModelEntity->GetTeamNum() == 2)
								{
									flColor[0] = 240.f / 255.f;
									flColor[1] = 30.f / 255.f;
									flColor[2] = 35.f / 255.f;
								}
								else
								{
									flColor[0] = 63.f / 255.f;
									flColor[1] = 72.f / 255.f;
									flColor[2] = 205.f / 255.f;
								}

							//	if (Menu::Window.VisualsTab.ChamsXqz.GetState()) {
									covered->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
							//	}
							//	else {
							//		covered->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
							//	}
							
								Interfaces::RenderView->SetColorModulation(flColor);
								Interfaces::RenderView->SetBlend(alpha);
								Interfaces::ModelRender->ForcedMaterialOverride(covered);
								oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

								if (pModelEntity->GetTeamNum() == 2)
								{
									flColor[0] = 247.f / 255.f;
									flColor[1] = 180.f / 255.f;
									flColor[2] = 20.f / 255.f;
								}
								else
								{
									flColor[0] = 32.f / 255.f;
									flColor[1] = 180.f / 255.f;
									flColor[2] = 57.f / 255.f;
								}
							
								Interfaces::RenderView->SetColorModulation(flColor);
								Interfaces::RenderView->SetBlend(alpha);
								Interfaces::ModelRender->ForcedMaterialOverride(open);
							}
						}
					}
				}
			}
		}
		else if (HandsStyle != 0 && strstr(ModelName, "arms"))
		{
			if (HandsStyle == 1)
			{
				DontDraw = true;
			}
			else if (HandsStyle == 2)
			{
				Interfaces::RenderView->SetBlend(0.3);
			}
			else if (HandsStyle == 3)
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
				if (pLocal)
				{
					if (pLocal->IsAlive())
					{
						int alpha = pLocal->HasGunGameImmunity() ? 150 : 255;

						if (pLocal->GetTeamNum() == 2)
							color.SetColor(240, 30, 35, alpha);
						else
							color.SetColor(63, 72, 205, alpha);

						ForceMaterial(color, covered);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

						if (pLocal->GetTeamNum() == 2)
							color.SetColor(247, 180, 20, alpha);
						else
							color.SetColor(32, 180, 57, alpha);
					}
					else
					{
						color.SetColor(255, 255, 255, 255);
					}

					ForceMaterial(color, open);
				}
			}
			else
			{
				static int counter = 0;
				static float colors[3] = { 1.f, 0.f, 0.f };

				if (colors[counter] >= 1.0f)
				{
					colors[counter] = 1.0f;
					counter += 1;
					if (counter > 2)
						counter = 0;
				}
				else
				{
					int prev = counter - 1;
					if (prev < 0) prev = 2;
					colors[prev] -= 0.05f;
					colors[counter] += 0.05f;
				}

				Interfaces::RenderView->SetColorModulation(colors);
				Interfaces::RenderView->SetBlend(0.3);
				Interfaces::ModelRender->ForcedMaterialOverride(OpenLit);
			}
		}
	}

	if (!DontDraw)
		oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	Interfaces::ModelRender->ForcedMaterialOverride(NULL);
}


// Hooked FrameStageNotify for removing visual recoil
void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage)
{
	DWORD eyeangles = NetVar.GetNetVar(0xBFEA4E7B);
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		if (curStage == FRAME_RENDER_START) {
			if (*(bool*)((DWORD)Interfaces::pInput + 0xA5))
				*(Vector*)((DWORD)pLocal + 0x31C8) = LastAngleAA;

			if ((Menu::Window.MiscTab.OtherThirdperson.GetState())) {
				static bool rekt = false;
				if (!rekt) {
					ConVar* sv_cheats = Interfaces::CVar->FindVar("sv_cheats");
					SpoofedConvar* sv_cheats_spoofed = new SpoofedConvar(sv_cheats);
					sv_cheats_spoofed->SetInt(1);
					rekt = true;
				}
			}

			static bool rekt1 = false;
			if (Menu::Window.MiscTab.OtherThirdperson.GetState()) {
				if (Menu::Window.MiscTab.ThirdPersonKeyBind.GetKey()) {
					*(bool*)((DWORD)Interfaces::pInput + 0xA5) = false;
					*(float*)((DWORD)Interfaces::pInput + 0xA8 + 0x8) = 0;

					if (GetKeyState(Menu::Window.MiscTab.ThirdPersonKeyBind.GetKey()) && pLocal->IsAlive()) {
						*(bool*)((DWORD)Interfaces::pInput + 0xA5) = true;
						*(float*)((DWORD)Interfaces::pInput + 0xA8 + 0x8) = Menu::Window.MiscTab.OtherThirdpersonRange.GetValue();
					}
				}
			}

			if (!Menu::Window.MiscTab.OtherThirdperson.GetState()) {
				*(bool*)((DWORD)Interfaces::pInput + 0xA5) = false;
				*(float*)((DWORD)Interfaces::pInput + 0xA8 + 0x8) = 0;
			}

			static auto linegoesthrusmoke = Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx");
			static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);

			static std::vector< const char* > smoke_materials = {
				"particle/vistasmokev1/vistasmokev1_smokegrenade",
				"particle/vistasmokev1/vistasmokev1_emods",
				"particle/vistasmokev1/vistasmokev1_emods_impactdust",
				"particle/vistasmokev1/vistasmokev1_fire",
			};
			for (auto matName : smoke_materials) {
				IMaterial* mat = Interfaces::MaterialSystem->FindMaterial(matName, "Other textures");
				mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, Menu::Window.VisualsTab.OtherNoSmoke.GetState());
			}

			if (Menu::Window.VisualsTab.OtherNoSmoke.GetState()) {
				*(int*)(smokecout) = 0;
			}
		}
	}
	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{

		IClientEntity *pEntity;
		ResolverSetup::GetInst().FSN(pEntity, curStage);


	}

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	
	
	/*	for (int i = 1; i < 65; i++)
		{
			IClientEntity* pEnt = Interfaces::EntList->GetClientEntity(i);
			if (!pEnt) continue;
			if (pEnt->IsDormant()) continue;
			if (pEnt->GetHealth() < 1) continue;
			if (pEnt->GetLifeState() != 0) continue;

			*(float*)((DWORD)pEnt + eyeangles) = pEnt->GetTargetYaw();
			//Msg("%f\n", *(float*)((DWORD)pEnt + m_angEyeAnglesYaw));
		} */

		if (Menu::Window.MiscTab.KnifeEnable.GetState() && pLocal)
		{
		
			IClientEntity* WeaponEnt = Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
			CBaseCombatWeapon* Weapon = (CBaseCombatWeapon*)WeaponEnt;
		
			int iBayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
			int iButterfly = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
			int iFlip = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
			int iGut = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
			int iKarambit = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
			int iM9Bayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
			int iHuntsman = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
			int iFalchion = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
			int iDagger = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
			int iBowie = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");

			int Model = Menu::Window.MiscTab.KnifeModel.GetIndex();
			int Skin = Menu::Window.MiscTab.KnifeSkin.GetIndex();

			if (Weapon)
			{
				if (WeaponEnt->GetClientClass()->m_ClassID == (int)CSGOClassID::CKnife)
				{
					if (Model == 0) // Karambit
					{
						*Weapon->ModelIndex() = iKarambit; // m_nModelIndex
						*Weapon->ViewModelIndex() = iKarambit;
						*Weapon->WorldModelIndex() = iKarambit + 1;
						*Weapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 507;

						if (Skin == 0)
							*Weapon->FallbackPaintKit() = 416; // Doppler Sapphire
						else if (Skin == 1)
							*Weapon->FallbackPaintKit() = 415; // Doppler Ruby
						else if (Skin == 2)
							*Weapon->FallbackPaintKit() = 409; // Tiger Tooth
						else if (Skin == 3)
							*Weapon->FallbackPaintKit() = 558; // Lore
					}
					else if (Model == 1) // Karambit
					{
						*Weapon->ModelIndex() = iBayonet; // m_nModelIndex
						*Weapon->ViewModelIndex() = iBayonet;
						*Weapon->WorldModelIndex() = iBayonet + 1;
						*Weapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 500;

						if (Skin == 0)
							*Weapon->FallbackPaintKit() = 416; // Doppler Sapphire
						else if (Skin == 1)
							*Weapon->FallbackPaintKit() = 415; // Doppler Ruby
						else if (Skin == 2)
							*Weapon->FallbackPaintKit() = 409; // Tiger Tooth
						else if (Skin == 3)
							*Weapon->FallbackPaintKit() = 558; // Lore
					}

					*Weapon->OwnerXuidLow() = 0;
					*Weapon->OwnerXuidHigh() = 0;
					*Weapon->FallbackWear() = 0.001f;
					*Weapon->m_AttributeManager()->m_Item()->ItemIDHigh() = 1;
				}
			}
		}
	//	if (pLocal->IsAlive())
	//		R::Resolve();
	}

	oFrameStageNotify(curStage);
}

void __fastcall Hooked_OverrideView(void* ecx, void* edx, CViewSetup* pSetup)
{
	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		if (Menu::Window.VisualsTab.Active.GetState() && pLocal->IsAlive() && !pLocal->IsScoped())
		{
			if (pSetup->fov = 90)
				pSetup->fov = Menu::Window.VisualsTab.OtherFOV.GetValue();
		}

		oOverrideView(ecx, edx, pSetup);
	}

}

void __fastcall  hkSceneEnd(void *pEcx, void *pEdx) {
	float flColor[3] = { 0.f };

	if (Menu::Window.VisualsTab.FakeAngleChams.GetState() && Menu::Window.VisualsTab.Active.GetState())
	{
		IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		if (pLocal)
		{
			static  IMaterial* CoveredLit = CreateMaterial(true);
			if (CoveredLit)
			{
				Vector OrigAng;
				OrigAng = pLocal->GetEyeAngles();
				pLocal->SetAngle2(Vector(0, fakeangle, 0));

				flColor[0] = 255 / 255.f;
				flColor[1] = 255 / 255.f;
				flColor[2] = 255 / 255.f;

				Interfaces::RenderView->SetColorModulation(flColor);
				Interfaces::RenderView->SetBlend(0.5);
				Interfaces::ModelRender->ForcedMaterialOverride(CoveredLit);
				pLocal->draw_model(0x00000001, 255);
				Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
				pLocal->SetAngle2(OrigAng);
			}
		}
	}

	pSceneEnd(pEcx);
}
void GetViewModelFOV(float& fov)
{
	IClientEntity* localplayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{

		if (!localplayer)
			return;


		if (Menu::Window.VisualsTab.Active.GetState())
		fov += Menu::Window.VisualsTab.OtherViewmodelFOV.GetValue();
	}
}

float __stdcall GGetViewModelFOV()
{
	float fov = Hooks::VMTClientMode.GetMethod<oGetViewModelFOV>(35)();

	GetViewModelFOV(fov);

	return fov;
}

void __fastcall Hooked_RenderView(void* ecx, void* edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw)
{
	static DWORD oRenderView = Hooks::VMTRenderView.GetOriginalFunction(6);

	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	__asm
	{
		PUSH whatToDraw
		PUSH nClearFlags
		PUSH hudViewSetup
		PUSH setup
		MOV ECX, ecx
		CALL oRenderView
	}
} //hooked for no reason yay