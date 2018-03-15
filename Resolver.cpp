


#include "Resolver.h"
#include "Ragebot.h"
#include "Hooks.h"



namespace resolvokek
{
	int Shots;
	int missedshots;
	float RealAngle;
	float FakeAngle;
	bool shouldflip;
	bool ySwitch;
	float NextTime;
	int resolvemode;
	float fakeAngle;
	float OldSimulationTime[65];
	bool error;
}
namespace Globals
{
	CUserCmd* UserCmd;
	IClientEntity* Target;
	int Shots;
	bool change;
	int choked_ticks;
	int TargetID;
	bool didhitHS;
	//	 int missedshots;
	std::map<int, QAngle> storedshit;
}
void LowerBodyYawFix(IClientEntity* pEntity)
{
	/*if (Menu::Window.RageBotTab.LowerbodyFix.GetState())
	{
	if (!pEntity) return;
	if (pEntity->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer) return;
	if (!pEntity->IsAlive() || !pEntity->GetActiveWeaponHandle()) return;
	if (Interfaces::Engine->GetLocalPlayer()) return;

	auto EyeAngles = pEntity->GetEyeAnglesXY();
	if (pEntity->GetVelocity().Length() > 1 && (pEntity->GetFlags() & (int)pEntity->GetFlags() & FL_ONGROUND))
	EyeAngles->y = pEntity->GetLowerBodyYaw();
	}*/
}


/*

...
;::::;
;::::; :;
;:::::'   :;
;:::::;     ;.
,:::::'       ;           OOO
::::::;       ;          OOOOO
;:::::;       ;         OOOOOOOO
,;::::::;     ;'         / OOOOOOO
;:::::::::`. ,,,;.        /  / DOOOOOO
.';:::::::::::::::::;,     /  /     DOOOO
,::::::;::::::;;;;::::;,   /  /        DOOO
;`::::::`'::::::;;;::::: ,#/  /          DOOO
:`:::::::`;::::::;;::: ;::#  /            DOOO
::`:::::::`;:::::::: ;::::# /              DOO
`:`:::::::`;:::::: ;::::::#/               DOO
:::`:::::::`;; ;:::::::::##                OO
::::`:::::::`;::::::::;:::#                OO
`:::::`::::::::::::;'`:;::#                O
`:::::`::::::::;' /  / `:#
::::::`:::::;'  /  /   `#

its supposed to be a grim reaper

*/
void PitchCorrection()
{
	CUserCmd* pCmd;
	for (int i = 0; i < Interfaces::Engine->GetMaxClients(); ++i)
	{
		IClientEntity* pLocal = hackManager.pLocal();
		IClientEntity *player = (IClientEntity*)Interfaces::EntList->GetClientEntity(i);

		if (!player || player->IsDormant() || player->GetHealth() < 1 || (DWORD)player == (DWORD)pLocal)
			continue;

		if (!player)
			continue;

		if (pLocal)
			continue;

		if (pLocal && player && pLocal->IsAlive())
		{
			/*	if (Menu::Window.RageBotTab.AdvancedResolver.GetState())
			{
			Vector* eyeAngles = player->GetEyeAnglesXY();
			if (eyeAngles->x < -179.f) eyeAngles->x += 360.f;
			else if (eyeAngles->x > 90.0 || eyeAngles->x < -90.0) eyeAngles->x = 89.f;
			else if (eyeAngles->x > 89.0 && eyeAngles->x < 91.0) eyeAngles->x -= 90.f;
			else if (eyeAngles->x > 179.0 && eyeAngles->x < 181.0) eyeAngles->x -= 180;
			else if (eyeAngles->x > -179.0 && eyeAngles->x < -181.0) eyeAngles->x += 180;
			else if (fabs(eyeAngles->x) == 0) eyeAngles->x = std::copysign(89.0f, eyeAngles->x);
			}*/
		}
	}
}

bool bLowerBodyUpdated;
bool IsUsingFakeAngles;
float Differences[65];
inline float RandomFloat(float min, float max)
{
	static auto fn = (decltype(&RandomFloat))(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
	return fn(min, max);
}
bool HasFakeHead(IClientEntity* pEntity) {
	//lby should update if distance from lby to eye angles exceeds 35 degrees
	return abs(pEntity->GetEyeAnglesXY()->y - pEntity->GetLowerBodyYaw()) > 35;
}
bool Lbywithin35(IClientEntity* pEntity) {
	//lby should update if distance from lby to eye angles less than 35 degrees
	return abs(pEntity->GetEyeAnglesXY()->y - pEntity->GetLowerBodyYaw()) < 35;
}
bool IsMovingOnGround(IClientEntity* pEntity) {
	//Check if player has a velocity greater than 0 (moving) and if they are onground.
	return pEntity->GetVelocity().Length2D() > 45.f && pEntity->GetFlags() & FL_ONGROUND;
}
bool IsMovingOnInAir(IClientEntity* pEntity) {
	//Check if player has a velocity greater than 0 (moving) and if they are onground.
	return !(pEntity->GetFlags() & FL_ONGROUND);
}
bool OnGround(IClientEntity* pEntity) {
	//Check if player has a velocity greater than 0 (moving) and if they are onground.
	return pEntity->GetFlags() & FL_ONGROUND;
}
bool IsFakeWalking(IClientEntity* pEntity) {
	//Check if a player is moving, but at below a velocity of 36
	return IsMovingOnGround(pEntity) && pEntity->GetVelocity().Length2D() < 36.0f;
}
bool IsEntityMoving(IClientEntity *player)
{
	return (player->GetVelocity().Length2D() > 0.1f && player->GetFlags() & FL_ONGROUND);
}

bool ResolverSetup::YawResolve2(IClientEntity *player)
{
	float PlayerIsMoving = abs(player->GetVelocity().Length2D());
	bool bLowerBodyUpdated = false;
	bool IsUsingFakeAngles = false;
	float oldlowerbodyyaw;
	int result; // al@1
	float viewangles = player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw();

	int v3; // [sp+0h] [bp-4h]@1

	v3 = rand() % 40 + 160;
	result = PlayerIsMoving;
	if (bLowerBodyUpdated)
	{
		result = IsUsingFakeAngles;
		if (bLowerBodyUpdated = -1)
		{
			oldlowerbodyyaw = (viewangles + 16);
			result = IsUsingFakeAngles;
		}
	}
	*(float *)&bLowerBodyUpdated = (float)v3 + *(float *)&oldlowerbodyyaw;
	bLowerBodyUpdated = oldlowerbodyyaw;
	return result;
}
bool ResolverSetup::YawResolve3(IClientEntity *player)
{
	float PlayerIsMoving = abs(player->GetVelocity().Length2D());
	bool bLowerBodyUpdated = false;
	bool IsUsingFakeAngles = false;
	float oldlowerbodyyaw;
	int result; // al@1
	float viewangles = player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw();


	result = (unsigned __int8)PlayerIsMoving;
	if (bLowerBodyUpdated)
	{
		result = (unsigned __int8)oldlowerbodyyaw;
		if (bLowerBodyUpdated)
		{
			result = player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw();

			if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) + 90.0);
		}
		else
		{
			if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16) > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 90.0;
		}
	}
	else if (++IsUsingFakeAngles == 1)
	{
		oldlowerbodyyaw = 0;
		result = player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw();
		if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 >  player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 90.0);
	}
	else
	{
		oldlowerbodyyaw = 1;
		if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) + 90.0);
		bLowerBodyUpdated = 0;
	}
	return result;
}
bool ResolverSetup::YawResolve4(IClientEntity *player)
{
	float PlayerIsMoving = abs(player->GetVelocity().Length2D());
	bool bLowerBodyUpdated = false;
	bool IsUsingFakeAngles = false;
	float oldlowerbodyyaw;
	int result; // al@1



	result = PlayerIsMoving;
	if (bLowerBodyUpdated)
	{
		result = IsUsingFakeAngles;
		if (oldlowerbodyyaw)
		{
			result = player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw();
			if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 90.0);
		}
		else
		{
			if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 180.0);
		}
	}
	else if (++oldlowerbodyyaw == 1)
	{
		bLowerBodyUpdated = 0;
		result = player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw();
		if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 180.0);
	}
	else
	{
		IsUsingFakeAngles = 1;
		if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 90.0);
		bLowerBodyUpdated = 0;
	}
	return result;
}
bool ResolverSetup::YawResolve5(IClientEntity *player)
{
	int v1; // eax@1
	int result; // eax@1
	signed __int64 v3; // rtt@1
	float v4; // xmm0_4@4
	float v5; // xmm0_4@6
	signed int v6; // [sp+8h] [bp-4h]@1

	v1 = rand();
	v3 = v1;
	result = v1 / 100;
	v6 = v3 % 100;
	if (v6 < 98)
	{
		result = player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw();
		if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 180.0);
	}
	if (v6 < 15)
	{
		v4 = (player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16) + (float)(rand() % 141 - 70);
		result = player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw();
		if (player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 == v4);
	}
	if (v6 == 69)
	{
		v5 = (player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16) + (float)(rand() % 181 - 90);
		result = player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw();
		if (player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 == v5);
	}
	return result;
}
bool ResolverSetup::YawResolve6(IClientEntity *player)
{
	bool bLowerBodyUpdated = false;
	bool IsUsingFakeAngles = false;
	float oldlowerbodyyaw;
	char result; // al@1


	if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 91.0);
	result = bLowerBodyUpdated == 0;
	oldlowerbodyyaw = result;
	if (result)
	{
		result = player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw();
		if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 180.0);
	}
	return result;
}
bool ResolverSetup::YawResolve7(IClientEntity *player)
{
	int result; // eax@1
	float PlayerIsMoving = abs(player->GetVelocity().Length2D());
	bool bLowerBodyUpdated = false;
	bool IsUsingFakeAngles = false;
	float oldlowerbodyyaw;


	result = oldlowerbodyyaw;
	if (PlayerIsMoving)
	{
		result = IsUsingFakeAngles;
		if (bLowerBodyUpdated)
		{
			result = oldlowerbodyyaw;
			if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) + 0.0);
		}
		else
		{
			if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) + 180.0);
		}
	}
	else if (++PlayerIsMoving == 1)
	{
		bLowerBodyUpdated = 0;
		result = player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw();
		if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) + 180.0);
	}
	else
	{
		oldlowerbodyyaw = 1;
		if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 >  player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) + 0.0);
		bLowerBodyUpdated = 0;
	}
	return result;
}
bool ResolverSetup::YawResolve8(IClientEntity *player)
{
	float PlayerIsMoving = abs(player->GetVelocity().Length2D());
	bool bLowerBodyUpdated = false;
	bool IsUsingFakeAngles = false;
	float oldlowerbodyyaw;

	double v2; // [sp+0h] [bp-Ch]@4

	if (PlayerIsMoving)
	{
		bLowerBodyUpdated;
		if (oldlowerbodyyaw == -1)
		{
			bLowerBodyUpdated = 0;
			(&PlayerIsMoving);
		}
	}
	v2 = (double)bLowerBodyUpdated;
	if (oldlowerbodyyaw)
	{
		if (IsUsingFakeAngles)
		{
			if (oldlowerbodyyaw)
				if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 90.0);
				else
					if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) + 90.0);
		}
		else
		{
			if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 180.0);
		}
	}
	else if (++PlayerIsMoving == 1)
	{
		bLowerBodyUpdated = 0;
		if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 180.0);
	}
	else
	{
		oldlowerbodyyaw = 1;
		if (bLowerBodyUpdated)
			if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 90.0);
			else
				if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) + 90.0);
		oldlowerbodyyaw = 0;
	}
	if (v2 >= 0.35)
	{
		IsUsingFakeAngles = bLowerBodyUpdated == 0;
		oldlowerbodyyaw = IsUsingFakeAngles;
	}
	return bLowerBodyUpdated;
}
bool ResolverSetup::YawResolve9(IClientEntity *player)
{
	int idx = player->GetIndex();
	float new_yaw = player->GetLowerBodyYaw();



	//AnimationLayer curBalanceLayer, prevBalanceLayer;

	//SResolveInfo &player_recs = arr_infos[idx];

	float PlayerIsMoving = abs(player->GetVelocity().Length2D());
	bool bLowerBodyUpdated = false;
	bool IsUsingFakeAngles = false;
	float oldlowerbodyyaw;
	bool result; // al@4


	if (IsEntityMoving(player))
	{
		if (oldlowerbodyyaw)
		{
			if (bLowerBodyUpdated)
				if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 226.0);
				else
					if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 134.0);
			result = oldlowerbodyyaw == 0;
			PlayerIsMoving = oldlowerbodyyaw == 0;
		}
		else
		{
			if (oldlowerbodyyaw)
			{
				result = player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw();
				if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 134.0);
			}
			else
			{
				result = player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw();
				if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 226.0);
			}
			bLowerBodyUpdated = PlayerIsMoving == 0;
		}
	}
	else if (++bLowerBodyUpdated == 1)
	{
		oldlowerbodyyaw = 0;
		if (bLowerBodyUpdated == true)
		{
			result = player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw();
			if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 134.0);
		}
		else
		{
			result = player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw();
			if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 226.0);
		}
		PlayerIsMoving = oldlowerbodyyaw == 0;
	}
	else
	{
		PlayerIsMoving = 1;
		if (IsUsingFakeAngles)
			if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 226.0);
			else
				if ((player->GetEyeAnglesXY()->y = player->GetLowerBodyYaw() + 16 > player->GetEyeAnglesXY()->y + player->GetLowerBodyYaw() + 16) - 134.0);
		result = oldlowerbodyyaw == 0;
		bLowerBodyUpdated = PlayerIsMoving == 0;
		IsUsingFakeAngles = 0;
	}
	return result;
}

void ResolverSetup::Resolve(IClientEntity* pEntity)
{
	float PlayerIsMoving = abs(pEntity->GetVelocity().Length2D());
	bool bLowerBodyUpdated = false;
	bool IsUsingFakeAngles = false;
	float oldlowerbodyyaw;
	bool result; // al@4
	double v2; // [sp+0h] [bp-Ch]@4
	int idx = pEntity->GetIndex();
	float new_yaw = pEntity->GetLowerBodyYaw();
	int v1; // eax@1
	signed __int64 v3; // rtt@1
	float v4; // xmm0_4@4
	float v5; // xmm0_4@6
	signed int v6; // [sp+8h] [bp-4h]@1
	/*if (Menu::Window.RageBotTab.AccuracyResolver.GetState())
	{

	float movinglby[64];
	float lbytomovinglbydelta[64]; // long name idk what else to put
	bool onground = pEntity->GetFlags() & FL_ONGROUND;
	lbytomovinglbydelta[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw() - lbytomovinglbydelta[pEntity->GetIndex()];

	if (pEntity->GetVelocity().Length2D() > 6 && pEntity->GetVelocity().Length2D() < 42) { // shitty ayyware fakewalk check better than nothing.
	pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 180;
	}
	else if (pEntity->GetVelocity().Length2D() < 6 || pEntity->GetVelocity().Length2D() > 42) { // they are moving
	pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw();
	movinglby[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
	}
	else if (lbytomovinglbydelta[pEntity->GetIndex()] > 50 && lbytomovinglbydelta[pEntity->GetIndex()] < -50) {// the 50 will allow you to have a 30 degree margin of error (do the math :))
	pEntity->GetEyeAnglesXY()->y = movinglby[pEntity->GetIndex()];
	}
	else
	pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw();
	}*/

	if (Menu::Window.RageBotTab.AccuracyResolver.GetState())
	{

		if (IsUsingFakeAngles)
		{
			pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + YawResolve9(pEntity) + YawResolve4(pEntity) + YawResolve5(pEntity) + YawResolve6(pEntity) + YawResolve7(pEntity) + YawResolve8(pEntity);
		}
		else if (++bLowerBodyUpdated == 1)
		{
			pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + YawResolve9(pEntity) + YawResolve4(pEntity) + YawResolve5(pEntity) + YawResolve6(pEntity) + YawResolve7(pEntity) + YawResolve8(pEntity);
		}
		else if (OnGround)
		{
			pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + YawResolve2(pEntity) + YawResolve9(pEntity) + YawResolve4(pEntity) + YawResolve5(pEntity) + YawResolve6(pEntity) + YawResolve7(pEntity) + YawResolve8(pEntity);
		}
		else if(IsMovingOnInAir)
		{
			pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + YawResolve2(pEntity) + YawResolve9(pEntity) + YawResolve4(pEntity) + YawResolve5(pEntity) + YawResolve6(pEntity) + YawResolve7(pEntity) + YawResolve8(pEntity);
		}
		else if(IsFakeWalking)
		{
			pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + YawResolve2(pEntity) + YawResolve9(pEntity) + YawResolve4(pEntity) + YawResolve5(pEntity) + YawResolve6(pEntity) + YawResolve7(pEntity) + YawResolve8(pEntity);
		}
		else if (Lbywithin35)
		{
		
			pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + YawResolve2(pEntity) + YawResolve9(pEntity) + YawResolve4(pEntity) + YawResolve5(pEntity) + YawResolve6(pEntity) + YawResolve7(pEntity) + YawResolve8(pEntity);
		}
		else if (HasFakeHead)
		{
			pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + YawResolve2(pEntity) + YawResolve9(pEntity) + YawResolve4(pEntity) + YawResolve5(pEntity) + YawResolve6(pEntity) + YawResolve7(pEntity) + YawResolve8(pEntity);
		}
		else if (IsMovingOnGround)
		{
			pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + YawResolve2(pEntity) + YawResolve9(pEntity) + YawResolve4(pEntity) + YawResolve5(pEntity) + YawResolve6(pEntity) + YawResolve7(pEntity) + YawResolve8(pEntity);
		}
		else if (IsUsingFakeAngles + IsMovingOnGround(pEntity) + Lbywithin35(pEntity) + HasFakeHead(pEntity) + IsMovingOnInAir(pEntity) + OnGround(pEntity))
		{
			pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + YawResolve2(pEntity) + YawResolve9(pEntity) + YawResolve4(pEntity) + YawResolve5(pEntity) + YawResolve6(pEntity) + YawResolve7(pEntity) + YawResolve8(pEntity);
		}


	}


}


bool ResolverSetup::lby_storage(IClientEntity* pEntity)

{

	Vector* eye_angles = pEntity->GetEyeAnglesPointer();
	float lower_body_yaw = pEntity->GetLowerBodyYaw();
	float lby_storage[65] = { 0 };
	bool player_is_moving;
	bool lby_meets_moving_status;
	int merge_in_pula_mea = pEntity->GetVelocity().Length2D();
	void carton();
	{
		//player is walking
		if (merge_in_pula_mea > 1) {
			player_is_moving = true;

		}
		else {
			player_is_moving = false;
		}


		//gotta have some requrements for the lby i guess
		if (player_is_moving = true && lower_body_yaw >= 35.f && lower_body_yaw <= 180.f) { /// meme value
			lby_meets_moving_status = true;
		}
		else {
			lby_meets_moving_status = false;
		}


		if (lby_meets_moving_status = true) { // logger
			lby_storage[pEntity->GetIndex()]++;
		}
	}
	//if theres no lby stored player doesnt meet the requirements anymore
	if (lby_storage[pEntity->GetIndex()] = 0) {
		lby_meets_moving_status = false;
	}
	//confirmation that the lby value is stored
	bool is_storing_lby;
	if (lby_storage[pEntity->GetIndex()] > 1)
	{
		is_storing_lby = true;

	}
	else {
		is_storing_lby = false;

	}

	{
		return lower_body_yaw && lby_storage;

	}
	{
		if (Globals::Shots > 1 && is_storing_lby)
			switch (Globals::Shots % 2) {
				//this the bruteforce 
			case 0:eye_angles -= 90; break;
			case 1:eye_angles -= 45; break;
			case 2:eye_angles += 90; break;
			case 3:eye_angles -= 90; break;
			case 4:eye_angles += 180; break;
			case 5:eye_angles += 35; break;
			case 6:eye_angles -= 35; break;
			case 7:eye_angles += 85; break;
			case 8:eye_angles -= 85; break;
			case 9:eye_angles -= 180; break;
			case 10:eye_angles += 10; break;
			case 11:eye_angles -= 10; break;
			case 12:eye_angles += 115; break;
			case 13:eye_angles -= 115; break;
			case 14:eye_angles += 175; break;
			case 15:eye_angles += 75; break;
			case 16:eye_angles -= 75; break;
			case 17:eye_angles += 99; break;
			case 18:eye_angles -= 99; break;
			case 19:eye_angles -= 179; break;
			}
		//lby pred
		float lby_timer; {
			bool lby_has_updated;
			float lby = pEntity->GetLowerBodyYaw();

			if (lby >= 1.1f); {
				lby_has_updated = true;
			}
			if (lby_has_updated = true) {
				{
					lby_timer = 1.f;


				}
			}
			return lby / lby_timer;
		}
	}
}

void ResolverSetup::StoreFGE(IClientEntity* pEntity)
{
	ResolverSetup::storedanglesFGE = pEntity->GetEyeAnglesXY()->y;
	ResolverSetup::storedlbyFGE = pEntity->GetLowerBodyYaw();
	ResolverSetup::storedsimtimeFGE = pEntity->GetSimulationTime();
}

void ResolverSetup::StoreThings(IClientEntity* pEntity)
{
	ResolverSetup::StoredAngles[pEntity->GetIndex()] = *pEntity->GetEyeAnglesXY();
	ResolverSetup::storedlby[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
	ResolverSetup::storedsimtime = pEntity->GetSimulationTime();
	ResolverSetup::storeddelta[pEntity->GetIndex()] = pEntity->GetEyeAnglesXY()->y;
	ResolverSetup::storedlby[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
}

void ResolverSetup::CM(IClientEntity* pEntity)
{
	for (int x = 1; x < Interfaces::Engine->GetMaxClients(); x++)
	{

		pEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(x);

		if (!pEntity
			|| pEntity == hackManager.pLocal()
			|| pEntity->IsDormant()
			|| !pEntity->IsAlive())
			continue;

		ResolverSetup::StoreThings(pEntity);
	}
}

void AngleCorrect(IClientEntity* pEntity)
{
	/*if (Menu::Window.RageBotTab.AccuracyResolver2.GetState())
	{
	auto delta = pEntity->GetLowerBodyYaw();
	switch (Globals::Shots % 0)

	if (fabsf(pEntity->GetVelocity().Length2D() < 18)) // its not a full fakewalk fix. its just a baim overdrive WOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
	{
	pEntity->GetEyeAnglesPointer()->y = pEntity->GetLowerBodyYaw() - rand() % 15;
	}
	else
	{
	pEntity->GetEyeAnglesPointer()->y = Globals::Shots * 50;
	}
	}*/
}


/*

I was asked to  put this here:


Name : justin garofolo

Age : 17

Gender : male



*/

void ResolverSetup::FSN(IClientEntity* pEntity, ClientFrameStage_t stage)
{
	if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		for (int i = 1; i < Interfaces::Engine->GetMaxClients(); i++)
		{

			pEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(i);

			if (!pEntity
				|| pEntity == hackManager.pLocal()
				|| pEntity->IsDormant()
				|| !pEntity->IsAlive())
				continue;

			ResolverSetup::Resolve(pEntity);
		}
	}
}