// Don't take credits for this ;) Joplin / Manhhao are the first uploaders ;)

//#define AYY_DEBUG

// General shit
#include "DLLMain.h"
#include "Utilities.h"
#include "CBulletListener.h"
// Injection stuff
#include "INJ/ReflectiveLoader.h"

// Stuff to initialise
#include "Offsets.h"
#include "Interfaces.h"
#include "Hooks.h"
#include "RenderManager.h"
#include "Hacks.h"
#include "Menu.h"
#include "AntiAntiAim.h"

#include "Dumping.h"

// Used as part of the reflective DLL injection
extern HINSTANCE hAppInstance;

// Our DLL Instance
HINSTANCE HThisModule;
bool DoUnload;

float lineLBY;
float lineRealAngle;
float lineFakeAngle;


// Our thread we use to setup everything we need
// Everything appart from code in hooks get's called from inside 
// here.

int InitialThread()
{
	Offsets::Initialise(); // Set our VMT offsets and do any pattern scans
	Interfaces::Initialise(); // Get pointers to the valve classes
	NetVar.RetrieveClasses(); // Setup our NetVar manager (thanks shad0w bby)
	NetvarManager::Instance()->CreateDatabase();
	Render::Initialise();
	Hacks::SetupHacks();
	Menu::SetupMenu();
	Hooks::Initialise();
	ApplyAAAHooks();
	CBulletListener::singleton()->init();

	//---------------------------------------------------------
	Utilities::Log("Ready");

	// While our cheat is running
	while (DoUnload == false)
	{
		Sleep(1000);
	}

	Hooks::UndoHooks();
	Sleep(2000); // Make sure none of our hooks are running
	FreeLibraryAndExitThread(HThisModule, 0);

	return 0;
}

// DllMain
// Entry point for our module
BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
	)
{
	switch (fdwReason)
	{
	case DLL_QUERY_HMODULE:
		if (lpvReserved != NULL)
			*(HMODULE *)lpvReserved = hAppInstance;
		break;
	case DLL_PROCESS_ATTACH:
		HThisModule = hinstDLL;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitialThread, NULL, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}