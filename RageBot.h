/*
Syn's AyyWare Framework 2015
*/

#pragma once

#include "Hacks.h"

class CRageBot : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
private:
	// Targetting
	int GetTargetCrosshair();
	int GetTargetDistance();
	int GetTargetHealth();
	bool TargetMeetsRequirements(IClientEntity* pEntity);
	float FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int HitBox);
	int HitScan(IClientEntity* pEntity);
	bool AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket);
	void Fakelag(CUserCmd *pCmd, bool &bSendPacket);

	// Functionality
	void DoAimbot(CUserCmd *pCmd, bool &bSendPacket);
	void DoNoRecoil(CUserCmd *pCmd);
	void PositionAdjustment(CUserCmd* pCmd);

	// AntiAim
	void DoAntiAim(CUserCmd *pCmd, bool&bSendPacket);
	bool freestanding(CUserCmd* m_pcmd, bool packet);

	// AimStep
	bool IsAimStepping;
	Vector LastAimstepAngle;
	Vector LastAngle;

	// Aimbot
	bool IsLocked;
	int TargetID;
	int HitBox;
	Vector AimPoint;
};