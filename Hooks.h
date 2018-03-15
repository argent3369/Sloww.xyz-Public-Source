/*
Syn's AyyWare Framework 2015
*/

#pragma once

// It's actually in DLLMain but w/e
extern bool DoUnload;


extern float lineLBY;
extern float lineRealAngle;
extern float lineFakeAngle;

#include "Utilities.h"
#include "Interfaces.h"
namespace Hooks
{
	void Initialise();
	void DrawBeamd(Vector src, Vector end, Color color);
	void UndoHooks();

	// VMT Managers
	extern Utilities::Memory::VMTManager VMTPanel; // Hooking drawing functions
	extern Utilities::Memory::VMTManager VMTClient; // Maybe CreateMove
	extern Utilities::Memory::VMTManager VMTClientMode; // CreateMove for functionality
	extern Utilities::Memory::VMTManager VMTModelRender; // DrawModelEx for chams
	extern Utilities::Memory::VMTManager VMTPrediction; // InPrediction for no vis recoil
	extern Utilities::Memory::VMTManager VMTPlaySound; // Autoaccept shit
	extern Utilities::Memory::VMTManager VMTEventManager;
	extern Utilities::Memory::VMTManager VMTRenderView;
};

namespace Global
{
	extern Vector LastAngle;
	extern bool lbyupdate;
	extern float curReal;
	extern float curFake;
	extern float hitmarkerAlpha;

}