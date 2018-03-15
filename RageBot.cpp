// Don't take credits for this ;) Joplin / Manhhao are the first uploaders ;)

#include "RageBot.h"
#include "RenderManager.h"
#include "Resolver.h"
#include "Autowall.h"
#include <iostream>
#include "UTIL Functions.h"
#include "GameFunctions.h"
#include "Hacks.h"

#define TICK_INTERVAL			( Interfaces::Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )

void CRageBot::Init()
{
	IsAimStepping = false;
	IsLocked = false;
	TargetID = -1;
}

void CRageBot::Draw()
{

}

bool IsAbleToShoot(IClientEntity* pLocal)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!pLocal)
		return false;

	if (!pWeapon)
		return false;

	float flServerTime = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;

	return (!(pWeapon->GetNextPrimaryAttack() > flServerTime));
}

float hitchance(IClientEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	//	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	float hitchance = 101;
	if (!pWeapon) return 0;
	if (Menu::Window.RageBotTab.AccuracyHitchance.GetValue() > 1)
	{//Inaccuracy method
		float inaccuracy = pWeapon->GetInaccuracy();
		if (inaccuracy == 0) inaccuracy = 0.0000001;
		inaccuracy = 1 / inaccuracy;
		hitchance = inaccuracy;

	}
	return hitchance;
}

// (DWORD)g_pNetVars->GetOffset("DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
// You need something like this
bool CanOpenFire() // Creds to untrusted guy
{
	IClientEntity* pLocalEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalEntity)
		return false;

	CBaseCombatWeapon* entwep = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocalEntity->GetActiveWeaponHandle());

	float flServerTime = (float)pLocalEntity->GetTickBase() * Interfaces::Globals->interval_per_tick;
	float flNextPrimaryAttack = entwep->GetNextPrimaryAttack();

	std::cout << flServerTime << " " << flNextPrimaryAttack << std::endl;

	return !(flNextPrimaryAttack > flServerTime);
}

void CRageBot::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocalEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalEntity)
		return;

	// Master switch
	if (!Menu::Window.RageBotTab.Active.GetState())
		return;

	// Aimbot
	if (Menu::Window.RageBotTab.AimbotEnable.GetState())
		DoAimbot(pCmd, bSendPacket);

	// Anti Aim 
	if (Menu::Window.RageBotTab.AntiAimEnable.GetState())
		DoAntiAim(pCmd, bSendPacket);

	// Position Adjustment
	if (Menu::Window.RageBotTab.AccuracyPositionAdjustment.GetState())
		PositionAdjustment(pCmd);

	// Recoil
	if (Menu::Window.RageBotTab.AccuracyRecoil.GetState())
		DoNoRecoil(pCmd);

//	if (Menu::Window.RageBotTab.Active.GetState())
//		Fakelag(pCmd, bSendPacket);

	// Aimstep
	if (Menu::Window.RageBotTab.AimbotAimStep.GetState())
	{
		Vector AddAngs = pCmd->viewangles - LastAngle;
		if (AddAngs.Length2D() > 25.f)
		{
			Normalize(AddAngs, AddAngs);
			AddAngs *= 25;
			pCmd->viewangles = LastAngle + AddAngs;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}
	LastAngle = pCmd->viewangles;
}

/*void CRageBot::Fakelag(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocal = hackManager.pLocal();

	int Factor;

	if (pLocal->GetVelocity().Length2D() >= 0.1)
		Factor = Menu::Window.RageBotTab.Fakelagvalue.GetValue();
	else if (IsLocked && TargetID >= 0 && HitBox >= 0)
		Factor = 0;
	else
		Factor = 0;

	static int iFakeLag = -1;
	iFakeLag++;

	if (iFakeLag <= Factor && iFakeLag > -1)
	{
		bSendPacket = false;
	}
	else
	{
		bSendPacket = true;
		iFakeLag = -1;
	}
}*/

Vector BestPoint(IClientEntity *targetPlayer, Vector &final)
{
	IClientEntity* pLocal = hackManager.pLocal();

	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = targetPlayer;
	ray.Init(final + Vector(0, 0, 10), final);
	Interfaces::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	final = tr.endpos;
	return final;
}

// Functionality
void CRageBot::DoAimbot(CUserCmd *pCmd,bool &bSendPacket) // Creds to encore1337 for getting it to work
{
	IClientEntity* pTarget = nullptr;
	IClientEntity* pLocal = hackManager.pLocal();
	Vector Start = pLocal->GetViewOffset() + pLocal->GetOrigin();
	bool FindNewTarget = true;
	//IsLocked = false;

	CSWeaponInfo* weapInfo = ((CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle()))->GetCSWpnData();

	// Don't aimbot with the knife..
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (Menu::Window.RageBotTab.AutoRevolver.GetState())
		if (GameUtils::IsRevolver(pWeapon))
		{
			static int delay = 0;
			delay++;
			if (delay <= 15)pCmd->buttons |= IN_ATTACK;
			else delay = 0;
		}
	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || !GameUtils::IsBallisticWeapon(pWeapon))
		{
			//TargetID = 0;
			//pTarget = nullptr;
			//HitBox = -1;
			return;
		}
	}
	else
		return;

	// Make sure we have a good target
	if (IsLocked && TargetID >= 0 && HitBox >= 0)
	{
		pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		if (pTarget  && TargetMeetsRequirements(pTarget))
		{
			HitBox = HitScan(pTarget);
			if (HitBox >= 0)
			{
				Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
				Vector View; 
				Interfaces::Engine->GetViewAngles(View);
				float FoV = FovToPlayer(ViewOffset, View, pTarget, HitBox);
				if (FoV < Menu::Window.RageBotTab.AimbotFov.GetValue())
					FindNewTarget = false;
			}
		}
	}

	// Find a new target, apparently we need to
	if (FindNewTarget)
	{
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;

		// Target selection type
		switch (Menu::Window.RageBotTab.TargetSelection.GetIndex())
		{
		case 0:
			TargetID = GetTargetCrosshair();
			break;
		case 1:
			TargetID = GetTargetDistance();
			break;
		case 2:
			TargetID = GetTargetHealth();
			break;
		}

		// Memes
		if (TargetID >= 0)
		{
			pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		}
		else
		{
			pTarget = nullptr;
			HitBox = -1;
		}
	} 

	Globals::Target = pTarget;
	Globals::TargetID = TargetID;

	// If we finally have a good target
	if (TargetID >= 0 && pTarget)
	{
		// Get the hitbox to shoot at
		HitBox = HitScan(pTarget);

		if (!CanOpenFire())
			return;

		// Key
		if (Menu::Window.RageBotTab.AimbotKeyPress.GetState())
		{
			int Key = Menu::Window.RageBotTab.AimbotKeyBind.GetKey();
			if (Key >= 0 && !GUI.GetKeyState(Key))
			{
				TargetID = -1;
				pTarget = nullptr;
				HitBox = -1;
				return;
			}
		}

		// Stop key
	//	int StopKey = Menu::Window.RageBotTab.AimbotStopKey.GetKey();
	//	if (StopKey >= 0 && GUI.GetKeyState(StopKey))
	//	{
	//		TargetID = -1;
	//		pTarget = nullptr;
	//		HitBox = -1;
	//		return;
	//	}

		float pointscale = Menu::Window.RageBotTab.TargetPointscale.GetValue() - 0.5f; // Aim height
//		float value = Menu::Window.RageBotTab.AccuracyHitchance.GetValue(); // Hitchance

		Vector Point;
		Vector AimPoint = GetHitboxPosition(pTarget, HitBox) + Vector(0, 0, pointscale);

		if (Menu::Window.RageBotTab.TargetMultipoint.GetState())
		{
			Point = BestPoint(pTarget, AimPoint);
		}
		else
		{
			Point = AimPoint;
		}

		// Lets open fire
		if (GameUtils::IsScopedWeapon(pWeapon) && !pWeapon->IsScoped() && Menu::Window.RageBotTab.AccuracyAutoScope.GetState()) // Autoscope
		{
			pCmd->buttons |= IN_ATTACK2;
		}
		else
		{
			if ((Menu::Window.RageBotTab.AccuracyHitchance.GetValue() * 1.5 <= hitchance(pLocal, pWeapon)) || Menu::Window.RageBotTab.AccuracyHitchance.GetValue() == 0 || *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() == 64)
			{
				if (AimAtPoint(pLocal, Point, pCmd, bSendPacket))
				{
					if (Menu::Window.RageBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
					{
						pCmd->buttons |= IN_ATTACK;
					}
					else
					{
						return;
					}
				}
				else if (Menu::Window.RageBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
				{
					pCmd->buttons |= IN_ATTACK;
				}
			}
		}

		if (IsAbleToShoot(pLocal) && pCmd->buttons & IN_ATTACK)
			Globals::Shots += 1;

		// Stop and Crouch
		if (TargetID >= 0 && pTarget)
		{
			if (Menu::Window.RageBotTab.AccuracyAutoStop.GetState())
			{
				pCmd->forwardmove = 0.f;
				pCmd->sidemove = 0.f;
				pCmd->buttons |= IN_DUCK;
			}
		}
	}

	// Auto Pistol
	if (GameUtils::IsPistol(pWeapon) && Menu::Window.RageBotTab.AimbotAutoPistol.GetState())
	{
		if (pCmd->buttons & IN_ATTACK)
		{
			static bool WasFiring = false;
			WasFiring = !WasFiring;
			
			if (WasFiring)
			{
				pCmd->buttons &= ~IN_ATTACK;
			}
		}
	}
}

bool CRageBot::TargetMeetsRequirements(IClientEntity* pEntity)
{
	// Is a valid player
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{
		// Entity Type checks
		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && Interfaces::Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			// Team Check
			if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || Menu::Window.RageBotTab.TargetFriendlyFire.GetState())
			{
				// Spawn Check
				if (!pEntity->HasGunGameImmunity())
				{
					return true;
				}
			}
		}
	}

	// They must have failed a requirement
	return false;
}

float CRageBot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
{
	// Anything past 180 degrees is just going to wrap around
	CONST FLOAT MaxDegrees = 180.0f;

	// Get local angles
	Vector Angles = View;

	// Get local view / eye position
	Vector Origin = ViewOffSet;

	// Create and intiialize vectors for calculations below
	Vector Delta(0, 0, 0);
	//Vector Origin(0, 0, 0);
	Vector Forward(0, 0, 0);

	// Convert angles to normalized directional forward vector
	AngleVectors(Angles, &Forward);
	Vector AimPos = GetHitboxPosition(pEntity, aHitBox);
	// Get delta vector between our local eye position and passed vector
	VectorSubtract(AimPos, Origin, Delta);
	//Delta = AimPos - Origin;

	// Normalize our delta vector
	Normalize(Delta, Delta);

	// Get dot product between delta position and directional forward vectors
	FLOAT DotProduct = Forward.Dot(Delta);

	// Time to calculate the field of view
	return (acos(DotProduct) * (MaxDegrees / PI));
}

int CRageBot::GetTargetCrosshair()
{
	// Target selection
	int target = -1;
	float minFoV = Menu::Window.RageBotTab.AimbotFov.GetValue();

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++) //GetHighestEntityIndex()
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (fov < minFoV)
				{
					minFoV = fov;
					target = i;
				}
			}
		}
	}

	return target;
}

int CRageBot::GetTargetDistance()
{
	// Target selection
	int target = -1;
	int minDist = 99999;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				Vector Difference = pLocal->GetOrigin() - pEntity->GetOrigin();
				int Distance = Difference.Length();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Distance < minDist && fov < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					minDist = Distance;
					target = i;
				}
			}
		}
	}

	return target;
}

int CRageBot::GetTargetHealth()
{
	// Target selection
	int target = -1;
	int minHealth = 101;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				int Health = pEntity->GetHealth();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Health < minHealth && fov < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					minHealth = Health;
					target = i;
				}
			}
		}
	}

	return target;
}

int CRageBot::HitScan(IClientEntity* pEntity)
{
	IClientEntity* pLocal = hackManager.pLocal();
	std::vector<int> HitBoxesToScan;

	// Get the hitboxes to scan
#pragma region GetHitboxesToScan
	int HitScanMode = Menu::Window.RageBotTab.TargetHitscan.GetIndex();
	int iSmart = Menu::Window.RageBotTab.AccuracySmart.GetValue();
	bool AWall = Menu::Window.RageBotTab.AccuracyAutoWall.GetState();
	bool Multipoint = Menu::Window.RageBotTab.TargetMultipoint.GetState();

	if (iSmart > 0 && pLocal->GetShotsFired() + 1 > iSmart)
	{
		HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
	}
	else
	{
		if (HitScanMode == 0)
		{
			// No Hitscan, just a single hitbox
			switch (Menu::Window.RageBotTab.TargetHitbox.GetIndex())
			{
			case 0:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				break;
			case 1:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				break;
			case 2:
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				break;
			case 3:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				break;
			case 4:
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
				break;
			}
		}
		else
		{
			switch (HitScanMode)
			{
			case 1:
				// Low
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				break;
			case 2:
				// Normal
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
				break;
			case 3:
				// High
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
			case 4:
				// Extreme
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
			}
		}
	}

#pragma endregion Get the list of shit to scan

	// check hits
	// check hits
	for (auto HitBoxID : HitBoxesToScan)
	{
		if (AWall)
		{
			Vector Point = GetHitboxPosition(pEntity, HitBoxID);
			float Damage = 0.f;
			Color c = Color(255, 255, 255, 255);
			if (CanHit(Point, &Damage))
			{
				c = Color(0, 255, 0, 255);
				if (Damage >= Menu::Window.RageBotTab.AccuracyMinimumDamage.GetValue())
				{
					return HitBoxID;
				}
			}
		}
		else
		{
			if (GameUtils::IsVisible(hackManager.pLocal(), pEntity, HitBoxID))
				return HitBoxID;
		}
	}

	return -1;
}

void CRageBot::PositionAdjustment(CUserCmd *pCmd)
{
	static ConVar* cvar_cl_interp = Interfaces::CVar->FindVar(("cl_interp"));
	static ConVar* cvar_cl_updaterate = Interfaces::CVar->FindVar(("cl_updaterate"));
	static ConVar* cvar_sv_maxupdaterate = Interfaces::CVar->FindVar(("sv_maxupdaterate"));
	static ConVar* cvar_sv_minupdaterate = Interfaces::CVar->FindVar(("sv_minupdaterate"));
	static ConVar* cvar_cl_interp_ratio = Interfaces::CVar->FindVar(("cl_interp_ratio"));

	IClientEntity* pLocal = hackManager.pLocal();

	if (!pLocal)
		return;

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (!pWeapon)
		return;

	float cl_interp = cvar_cl_interp->GetFloat();
	int cl_updaterate = cvar_cl_updaterate->GetInt();
	int sv_maxupdaterate = cvar_sv_maxupdaterate->GetInt();
	int sv_minupdaterate = cvar_sv_minupdaterate->GetInt();
	int cl_interp_ratio = cvar_cl_interp_ratio->GetInt();

	if (sv_maxupdaterate <= cl_updaterate)
		cl_updaterate = sv_maxupdaterate;

	if (sv_minupdaterate > cl_updaterate)
		cl_updaterate = sv_minupdaterate;

	float new_interp = (float)cl_interp_ratio / (float)cl_updaterate;

	if (new_interp > cl_interp)
		cl_interp = new_interp;

	float flSimTime = pLocal->GetSimulationTime();
	float flOldAnimTime = pLocal->GetAnimTime();

	int iTargetTickDiff = (int)(0.5f + (flSimTime - flOldAnimTime) / Interfaces::Globals->interval_per_tick);
}

void CRageBot::DoNoRecoil(CUserCmd *pCmd)
{
	// Ghetto rcs shit, implement properly later
	IClientEntity* pLocal = hackManager.pLocal();
	if (pLocal)
	{
		Vector AimPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			pCmd->viewangles -= AimPunch * 2;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}
}

bool CRageBot::AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket)
{
	bool ReturnValue = false;
	// Get the full angles
	if (point.Length() == 0) return ReturnValue;

	Vector angles;
	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();

	CalcAngle(src, point, angles);
	GameUtils::NormaliseViewAngle(angles);

	if (angles[0] != angles[0] || angles[1] != angles[1])
	{
		return ReturnValue;
	}


	IsLocked = true;
	//-----------------------------------------------

	// Aim Step Calcs
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	if (!IsAimStepping)
		LastAimstepAngle = LastAngle; // Don't just use the viewangs because you need to consider aa

	float fovLeft = FovToPlayer(ViewOffset, LastAimstepAngle, Interfaces::EntList->GetClientEntity(TargetID), 0);

	if (fovLeft > 25.0f && Menu::Window.RageBotTab.AimbotAimStep.GetState())
	{
		Vector AddAngs = angles - LastAimstepAngle;
		Normalize(AddAngs, AddAngs);
		AddAngs *= 25;
		LastAimstepAngle += AddAngs;
		GameUtils::NormaliseViewAngle(LastAimstepAngle);
		angles = LastAimstepAngle;
	}
	else
	{
		ReturnValue = true;
	}

	// Silent Aim
	if (Menu::Window.RageBotTab.AimbotSilentAim.GetState() && !Menu::Window.RageBotTab.AimbotPerfectSilentAim.GetState())
	{
		pCmd->viewangles = angles;
	}

	// Normal Aim
	if (!Menu::Window.RageBotTab.AimbotSilentAim.GetState() && !Menu::Window.RageBotTab.AimbotPerfectSilentAim.GetState())
	{
		Interfaces::Engine->SetViewAngles(angles);
	}

	// pSilent Aim 
	Vector Oldview = pCmd->viewangles;

	if (Menu::Window.RageBotTab.AimbotPerfectSilentAim.GetState())
	{
		static int ChokedPackets = -1;
		ChokedPackets++;

		if (ChokedPackets < 6)
		{
			bSendPacket = false;
			pCmd->viewangles = angles;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles = Oldview;
			ChokedPackets = -1;
			ReturnValue = false;
		}
		
		//pCmd->viewangles.z = 0;
	}

	return ReturnValue;
}

namespace AntiAims // CanOpenFire checks for fake anti aims?
{
	void b1gJoplinSwitch(CUserCmd *pCmd, bool &bSendPacket)
	{
		IClientEntity* pLocal = hackManager.pLocal();

		if (!bSendPacket) {
			if (Menu::Window.RageBotTab.SWSwitchKey.GetKey()) {
				pCmd->viewangles.y -= 90;
				if (GetKeyState(Menu::Window.RageBotTab.SWSwitchKey.GetKey()) && pLocal->IsAlive())	{
					pCmd->viewangles.y += 180;
				}
			}
		}
		else {
			if (Menu::Window.RageBotTab.SWSwitchKey.GetKey()) {
				pCmd->viewangles.y += 90;
				if (GetKeyState(Menu::Window.RageBotTab.SWSwitchKey.GetKey()) && pLocal->IsAlive())	{
					pCmd->viewangles.y -= 180;
				}
			}
		}
	}

	void fakeSlowSpin(CUserCmd *pCmd, bool &bSendPacket)
	{
		if (bSendPacket) {
			int random = rand() % 100;
			int random2 = rand() % 1000;

			static bool dir;
			static float current_y = pCmd->viewangles.y;

			if (random == 1) dir = !dir;

			if (dir)
				current_y += 10;
			else
				current_y -= 10;

			pCmd->viewangles.y = current_y;

			if (random == random2)
				pCmd->viewangles.y += random;
		}
		else {
			pCmd->viewangles.y += 180;
		}
	}

	void Backward(CUserCmd* pCmd) {
		Vector OriginalY = pCmd->viewangles;
		pCmd->viewangles.y = OriginalY.y + 180.f;
	}

	void fakeside(float& angle, bool &bSendPacket) {
		if (bSendPacket) {
			angle += 180.f;
		}
	}

	void down(float& angle) {
		angle = 89.f;
	}
}

bool is_viable_target(IClientEntity* pEntity) {
	IClientEntity* m_local = hackManager.localplayer();
	if (!pEntity) return false;
	if (pEntity->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer) return false;
	if (pEntity == m_local) return false;
	if (pEntity->GetTeamNum() == m_local->GetTeamNum()) return false;
	if (pEntity->HasGunGameImmunity()) return false;
	if (!pEntity->IsAlive() || pEntity->IsDormant()) return false;
	return true;
}

bool CRageBot::freestanding(CUserCmd* m_pcmd, bool packet) {
	IClientEntity* m_local = hackManager.localplayer();

	if (Menu::Window.RageBotTab.Freestanding.GetState() && m_local->GetVelocity().Length() < 300) {
		auto fov_to_player = [](Vector view_offset, Vector view, IClientEntity* m_entity, int hitbox)
		{
			CONST FLOAT MaxDegrees = 180.0f;
			Vector Angles = view;
			Vector Origin = view_offset;
			Vector Delta(0, 0, 0);
			Vector Forward(0, 0, 0);
			game::functions.angle_vectors(Angles, Forward);
			Vector AimPos = game::functions.get_hitbox_location(m_entity, hitbox);
			game::functions.vector_subtract(AimPos, Origin, Delta);
			game::functions.normalize(Delta, Delta);
			FLOAT DotProduct = Forward.Dot(Delta);
			return (acos(DotProduct) * (MaxDegrees / PI));
		};

		int target = -1;
		float mfov = 50;
		Vector viewoffset = m_local->GetOrigin() + m_local->GetViewOffset();
		Vector view; Interfaces::Engine->GetViewAngles(view);

		for (int i = 0; i < Interfaces::Globals->maxClients; i++) {
			IClientEntity* m_entity = Interfaces::EntList->GetClientEntity(i);

			if (is_viable_target(m_entity)) {

				float fov = fov_to_player(viewoffset, view, m_entity, 0);
				if (fov < mfov) {
					mfov = fov;
					target = i;
				}
			}
		}

		Vector at_target_angle;

		if (target) {
			auto m_entity = Interfaces::EntList->GetClientEntity(target);

			if (is_viable_target(m_entity)) {
				Vector head_pos_screen;

				if (game::functions.world_to_screen(m_entity->GetHeadPos(), head_pos_screen)) {

					float pitch = m_pcmd->viewangles.x;
					AntiAims::down(pitch);
					m_pcmd->viewangles.x = pitch;

					float yaw = m_pcmd->viewangles.y;
					AntiAims::fakeside(yaw, packet);

					m_pcmd->viewangles.y = yaw;

					game::functions.calculate_angle(m_local->GetOrigin(), m_entity->GetOrigin(), at_target_angle);
					at_target_angle.x = 0;

					Vector src3D, dst3D, forward, right, up, src, dst;
					float back_two, right_two, left_two;
					trace_t tr;
					Ray_t ray, ray2, ray3, ray4, ray5;
					CTraceFilter filter;

					const Vector to_convert = at_target_angle;
					game::functions.angle_vectors(to_convert, &forward, &right, &up);

					filter.pSkip = m_local;
					src3D = m_local->GetEyePosition();
					dst3D = src3D + (forward * 384); //Might want to experiment with other numbers, incase you don't know what the number does, its how far the trace will go. Lower = shorter.

					ray.Init(src3D, dst3D);
					Interfaces::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);
					back_two = (tr.endpos - tr.startpos).Length();

					ray2.Init(src3D + right * 35, dst3D + right * 35);
					Interfaces::Trace->TraceRay(ray2, MASK_SHOT, &filter, &tr);

					right_two = (tr.endpos - tr.startpos).Length();
					ray3.Init(src3D - right * 35, dst3D - right * 35);

					Interfaces::Trace->TraceRay(ray3, MASK_SHOT, &filter, &tr);
					left_two = (tr.endpos - tr.startpos).Length();

					if (left_two > right_two) {
						if (packet)
							m_pcmd->viewangles.y += 90;
						else
							m_pcmd->viewangles.y -= 90;
					}
					else if (right_two > left_two) {
						if (packet)
							m_pcmd->viewangles.y -= 90;
						else
							m_pcmd->viewangles.y += 90;
					}
					else {
						if (packet) {
							int jitterrange = 90;

							static bool Turbo = false;
							if (Turbo)
							{
								m_pcmd->viewangles.y -= jitterrange;
								Turbo = !Turbo;
							}
							else
							{
								m_pcmd->viewangles.y += jitterrange;
								Turbo = !Turbo;
							}
						}
						else
							m_pcmd->viewangles.y -= 180;
					}			

					return true;
				}
			}
		}
	}
	return false;
}

// AntiAim
void CRageBot::DoAntiAim(CUserCmd *pCmd, bool &bSendPacket) // pCmd->viewangles.y = 0xFFFFF INT_MAX or idk
{
	IClientEntity* pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (!pWeapon)
		return;

	if (pCmd->buttons & IN_ATTACK && game::functions.CanAttack())
		return;
	if (pCmd->buttons & IN_USE)
		return;
	if (pLocal->GetMoveType() == MOVETYPE_LADDER || pLocal->GetMoveType() == MOVETYPE_NOCLIP)
		return;

	if (pWeapon) {
		CSWeaponInfo* pWeaponInfo = pWeapon->GetCSWpnData();
		CCSGrenade* csGrenade = (CCSGrenade*)pWeapon;

		if (!GameUtils::IsBallisticWeapon(pWeapon)) {
			if (Menu::Window.RageBotTab.AntiAimKnife.GetState()) {
				if (!CanOpenFire() || pCmd->buttons & IN_ATTACK2)
					return;
			}
			else {
				return;
			}
		}

		if (GameUtils::IsG(pWeapon)) {
			if (csGrenade->GetThrowTime() > 0.f)
				return;
		}
	}

	if (!freestanding(pCmd, bSendPacket)) {

		switch (Menu::Window.RageBotTab.AntiAimPitch.GetIndex()) // Magic pitch is 69.69?
		{
		case 0:
			break;
		case 1:
			pCmd->viewangles.x = 89.0f;
			break;
		case 2:
			pCmd->viewangles.x = 51.f;
			break;
		}

		switch (Menu::Window.RageBotTab.AntiAimYaw.GetIndex())
		{
		case 0:
			break;
		case 1:
			AntiAims::Backward(pCmd);
			break;
		case 2:
			AntiAims::b1gJoplinSwitch(pCmd, bSendPacket);
			break;
		case 3:
			AntiAims::fakeSlowSpin(pCmd, bSendPacket);
			break;
		}
	}
}

