// Don't take credits for this ;) Joplin / Manhhao are the first uploaders ;)

// Credits to Valve and Shad0w
#include "Interfaces.h"
#include "Menu.h"

// Shad0ws Yaw fix
// (FIX ME UP LATER)
void FixY(const CRecvProxyData *pData, void *pStruct, void *pOut)
{
	static Vector vLast[65];
	static bool bShotLastTime[65];
	static bool bJitterFix[65];

	float *flPitch = (float*)((DWORD)pOut - 4);
	float flYaw = pData->m_Value.m_Float;
	bool bHasAA;
	bool bSpinbot;
	static bool last[128];
	bool tmp = last[((IClientEntity*)(pStruct))->GetIndex()];
	float yaw = pData->m_Value.m_Float;
	yaw -= 0.087929;



	*(float*)(pOut) = flYaw;
}

// Simple fix for some Fake-Downs
void FixX(const CRecvProxyData* pData, void* pStruct, void* pOut) // Clamp other player angles to fix fakedown or lisp
{
	float* ang = (float*)pOut;
	*ang = pData->m_Value.m_Float;
	DWORD hex = *(DWORD*)(&ang);

	
}

RecvVarProxyFn oRecvnModelIndex;

void Hooked_RecvProxy_Viewmodel(CRecvProxyData *pData, void *pStruct, void *pOut)
{
	// Get the knife view model id's
	static int default_t = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
	static int default_ct = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
	static int bayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
	static int karam = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");

	// Get local player (just to stop replacing spectators knifes)
	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (Menu::Window.MiscTab.KnifeEnable.GetState() && pLocal)
	{
		// If we are alive and holding a default knife(if we already have a knife don't worry about changing)
		if (pLocal->IsAlive() && (pData->m_Value.m_Int == default_t || pData->m_Value.m_Int == default_ct))
		{
			// Set whatever knife we want
			if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 0)
				pData->m_Value.m_Int = karam;
			else if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 1)
				pData->m_Value.m_Int = bayonet;
		}
	}

	// Carry on the to original proxy
	oRecvnModelIndex(pData, pStruct, pOut);
}

void ApplyAAAHooks()
{
	ClientClass *pClass = Interfaces::Client->GetAllClasses();
	while (pClass)
	{
		const char *pszName = pClass->m_pRecvTable->m_pNetTableName;
		if (!strcmp(pszName, "DT_CSPlayer"))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;

				// Pitch Fix
				if (!strcmp(name, "m_angEyeAngles[0]"))
				{
					pProp->m_ProxyFn = FixX;
				}

				// Yaw Fix
				if (!strcmp(name, "m_angEyeAngles[1]"))
				{
					Utilities::Log("Yaw Fix Applied");
					pProp->m_ProxyFn = FixY;
				}
			}
		}
		else if (!strcmp(pszName, "DT_BaseViewModel"))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;

				// Knives
				if (!strcmp(name, "m_nModelIndex"))
				{
					oRecvnModelIndex = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = Hooked_RecvProxy_Viewmodel;
				}
			}
		}
		pClass = pClass->m_pNext;
	}
}