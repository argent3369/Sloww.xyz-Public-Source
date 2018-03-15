#include <array>
#include <string>
#include <deque>
#include <algorithm>
#include "Entities.h"
#include "CommonIncludes.h"
#include "Entities.h"
#include "Vector.h"
#include <map>
#include "Interfaces.h"
#include "Hacks.h"
#include "Hooks.h"


namespace resolvokek
{
	extern int Shots;
	extern int missedshots;
	extern float RealAngle;
	extern float FakeAngle;
	extern bool shouldflip;
	extern bool ySwitch;
	extern float NextTime;
	extern int resolvemode;
	extern float fakeAngle;
	extern float OldSimulationTime[65];
	extern bool error;
}



class ResolverSetup
{
public:






	bool didhit;
	bool lbyDeltaOver120(int plID);
	void FSN(IClientEntity* pEntity, ClientFrameStage_t stage); //used to get the fresh THINGS and the resolve
	void CM(IClientEntity* pEntity); //cause this is slower than FSN so we are going to get older info and not updated at all ez to compare between etc.
	std::map<int, float>storedhealth;
	void StoreThings(IClientEntity* pEntity);
	void StoreThings2(IClientEntity* pEntity);
	bool AngleResolver(IClientEntity * player);
	bool YawResolve2(IClientEntity * player);
	bool YawResolve3(IClientEntity * player);
	bool YawResolve4(IClientEntity * player);
	bool YawResolve5(IClientEntity * player);
	bool YawResolve6(IClientEntity * player);
	bool YawResolve7(IClientEntity * player);
	bool YawResolve8(IClientEntity * player);
	bool YawResolve9(IClientEntity * player);
	void Resolve(IClientEntity * pEntity);
	std::map<int, QAngle>StoredAngles; //y and x lol (stored)
	std::map<int, QAngle>NewANgles; //y and x lol (new)
	std::map<int, float>storedlby;
	std::map<int, float>newlby;
	std::map<int, float>storeddelta;
	std::map<int, float>newdelta;
	std::map<int, float>finaldelta;
	std::map<int, float>storedlbydelta;
	std::map<int, float>newlbydelta;
	std::map<int, float>finallbydelta;
	float newsimtime;
	float storedsimtime;
	bool lbyupdated;
	float storedlbyFGE;
	float storedanglesFGE;
	float storedsimtimeFGE;
	bool didhitHS;
	void Resolve(IClientEntity * pEntity, int CurrentTarget);
	bool lby_storage(IClientEntity * pEntity);
	void StoreFGE(IClientEntity* pEntity);
	static ResolverSetup GetInst()
	{
		static ResolverSetup instance;
		return instance;
	}
};
namespace Globals
{
	extern CUserCmd* UserCmd;
	extern IClientEntity* Target;
	extern int Shots;
	extern bool change;
	extern int choked_ticks;
	extern int TargetID;
	extern bool didhitHS;
	extern int missedshots;
	extern std::map<int, QAngle> storedshit;
}