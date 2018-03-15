/*
Syn's AyyWare Framework 2015
*/

#pragma once

#include "Hacks.h"

extern float lineLBY;
extern float lineRealAngle;
extern float lineFakeAngle;

class CEsp : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
private:

	IClientEntity *BombCarrier;

	struct ESPBox
	{
		int x, y, w, h;
	};

	void DrawPlayer(IClientEntity* pEntity, player_info_t pinfo);

	Color GetPlayerColor(IClientEntity* pEntity);
	bool GetBox(IClientEntity* pEntity, ESPBox &result);

	void SpecList();
	//void DrawBox(ESPBox size, Color color);
	void DrawBox(IClientEntity* pEntity, CEsp::ESPBox size, Color color);
	//void DrawName(player_info_t pinfo, ESPBox size);
	void DrawName(IClientEntity* pEntity, player_info_t pinfo, CEsp::ESPBox size);
	void DrawHealth(IClientEntity* pEntity, ESPBox size);
	void DrawInfo(IClientEntity* pEntity, ESPBox size);
	void DrawCross(IClientEntity* pEntity);
	void DrawSkeleton(IClientEntity* pEntity);
	void DrawLinesAA(Color color);

	void DrawChicken(IClientEntity* pEntity, ClientClass* cClass);
	void DrawDrop(IClientEntity* pEntity, ClientClass* cClass);
	void DrawArmor(IClientEntity * pEntity, CEsp::ESPBox size);
	void DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass);
	void DrawBomb(IClientEntity* pEntity, ClientClass* cClass);
};

