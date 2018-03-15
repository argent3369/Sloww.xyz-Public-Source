/*
Syn's AyyWare Framework 2015
*/

#pragma once

#include "GUI.h"
#include "Controls.h"

class CRageBotTab : public CTab
{
public:
	void Setup();

	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;

	// Aimbot Settings
	CSilentGroupBox AimbotGroup;
	CCheckBox AimbotEnable;
	CCheckBox AimbotAutoFire;
	CSlider	  AimbotFov;
	CCheckBox AimbotSilentAim;
	CCheckBox AimbotPerfectSilentAim;
	CCheckBox AimbotAutoPistol;
	CCheckBox AutoRevolver;
	CCheckBox claupizdaresolver;
	CCheckBox AimbotAimStep;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CKeyBind  AimbotStopKey;

	// Target Selection Settings
	CSilentGroupBox TargetGroup;
	CComboBox TargetSelection;
	CCheckBox TargetFriendlyFire;
	CComboBox TargetHitbox;
	CComboBox TargetHitscan;
	CCheckBox TargetMultipoint;
	CSlider   TargetPointscale;

	// Accuracy Settings
	CSilentGroupBox AccuracyGroup;
	CCheckBox AccuracyRecoil;
	CCheckBox AccuracyAutoWall;
	CSlider	  AccuracyMinimumDamage;
	CCheckBox AccuracyAutoStop;
	CCheckBox AccuracyAutoCrouch;
	CCheckBox AccuracyAutoScope;
	CSlider   AccuracyHitchance;
	CCheckBox AccuracyResolver;
	CKeyBind ResolverOverrideKey;
	CCheckBox AccuracyPositionAdjustment;
	CSlider   AccuracySmart;

	// Anti-Aim Settings
	CSilentGroupBox AntiAimGroup;
	CCheckBox AntiAimEnable;
	CComboBox AntiAimPitch;
	CComboBox AntiAimYaw;
	CComboBox AntiAimFakeYaw;
	CKeyBind SWSwitchKey;
	CComboBox AntiAimEdge;
	CSlider	  AntiAimOffset;
	CCheckBox AntiAimKnife;
	CCheckBox AntiAimTarget;
	CSlider	  Fakelagvalue;
	CCheckBox Freestanding;
};

class CLegitBotTab : public CTab
{
public:
	void Setup();

	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;

	// Aimbot Settings
	CSilentGroupBox AimbotGroup;
	CCheckBox AimbotEnable;
	CCheckBox AimbotAutoFire;
	CCheckBox AimbotFriendlyFire;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CCheckBox AimbotAutoPistol;
	CSlider   AimbotInaccuracy;
	CKeyBind  AimbotDisableVis;

	// Main
	CSilentGroupBox TriggerGroup;
	CCheckBox TriggerEnable;
	CCheckBox TriggerKeyPress;
	CKeyBind  TriggerKeyBind;
	CSlider   TriggerDelay;

	// Main
	CSilentGroupBox WeaponMainGroup;
	CSlider   WeaponMainSpeed;
	CSlider   WeaponMainFoV;
	CCheckBox WeaponMainRecoil;
	CCheckBox WeaponMainPSilent;
	CSlider   WeaponMainInacc;
	CComboBox WeaponMainHitbox;

	// Pistol
	CSilentGroupBox WeaponPistGroup;
	CSlider   WeaponPistSpeed;
	CSlider   WeaponPistFoV;
	CCheckBox WeaponPistRecoil;
	CCheckBox WeaponPistPSilent;
	CSlider   WeaponPistInacc;
	CComboBox WeaponPistHitbox;

	// Sniper
	CSilentGroupBox WeaponSnipGroup;
	CSlider   WeaponSnipSpeed;
	CSlider   WeaponSnipFoV;
	CCheckBox WeaponSnipRecoil;
	CCheckBox WeaponSnipPSilent;
	CSlider   WeaponSnipInacc;
	CComboBox WeaponSnipHitbox;
};

class CVisualTab : public CTab
{
public:
	void Setup();

	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;

	CCheckBox Armoresp;

	// Options Settings
	CSilentGroupBox OptionsGroup;
	CCheckBox OptionsBox;
	CCheckBox OptionsName;
	CCheckBox OptionsHealth;
	CCheckBox OptionsWeapon;
	CCheckBox OptionsInfo;
	CCheckBox OptionsHelmet;
	CCheckBox OptionsKit;
	CCheckBox OptionsDefuse;
	//CCheckBox OptionsGlow;
	CComboBox OptionsChams;
	CCheckBox OptionsSkeleton;
	CCheckBox OptionsAimSpot;
	CCheckBox OptionsCompRank;

	// Filters Settings
	CSilentGroupBox FiltersGroup;
	CCheckBox FiltersAll;
	CCheckBox FiltersPlayers;
	CCheckBox FiltersEnemiesOnly;
	CCheckBox FiltersWeapons;
	CCheckBox FiltersChickens;
	CCheckBox FiltersC4;

	CCheckBox ResolverInfo;

	// anglechams
	CCheckBox FakeAngleChams;
	CCheckBox FakeLines;

	CSilentGroupBox OtherGroup;
	CCheckBox OtherCrosshair;
	CComboBox OtherRecoilCrosshair;
	CCheckBox OtherHitmarker;
	CCheckBox OtherRadar;
	CCheckBox OtherNoVisualRecoil;
	CCheckBox OtherNoSky; 
	CCheckBox OtherNoFlash; 
	CCheckBox OtherNoSmoke;
	CCheckBox OtherNoScope;
	CCheckBox OtherAsusWalls;
	CComboBox OtherNoHands;
	CCheckBox BulletTracers;
	CSlider OtherViewmodelFOV;
	CSlider OtherFOV;
};

class CMiscTab : public CTab
{
public:
	void Setup();



	// Knife Changer
	CSilentGroupBox KnifeGroup;
	CCheckBox KnifeEnable;
	CComboBox KnifeModel;
	CComboBox KnifeSkin;
	CButton   KnifeApply;

	//configs
	CComboBox lstPlayers;
	// Other Settings
	CSilentGroupBox OtherGroup;
	CComboBox OtherAutoJump;
	CCheckBox OtherEdgeJump;
	CComboBox OtherAutoStrafe;
	CCheckBox OtherSafeMode;
	CComboBox OtherChatSpam;
	CCheckBox OtherTeamChat;
	CSlider	  OtherChatDelay;
	CKeyBind  OtherAirStuck;
	CKeyBind  OtherLagSwitch;
	CCheckBox OtherSpectators;
	CCheckBox OtherThirdperson;
	CKeyBind ThirdPersonKeyBind;
	CSlider OtherThirdpersonRange;
	//CCheckBox OtherAutoAccept;
	CCheckBox OtherWalkbot;
	CComboBox OtherClantag;
	CComboBox OtherThirdpersonType;
	// Fake Lag Settings
	CSilentGroupBox FakeLagGroup;
	CCheckBox FakeLagEnable;
	CSlider   FakeLagChoke;
	CSlider	  FakeLagSend;
	CCheckBox ChokeRandomize;
	CCheckBox SendRandomize;
	//CCheckBox FakeLagWhileShooting;

	// Teleport shit cause we're cool
	CSilentGroupBox TeleportGroup;
	CCheckBox TeleportEnable;
	CKeyBind  TeleportKey;
};

class CColorsTab : public CTab
{
public:
	void Setup();
	CSlider CharmsA;
	CSilentGroupBox ColorsGroup;
	CSilentGroupBox CTNVisGroup;
	CSilentGroupBox CTVisGroup;
	CSilentGroupBox TNVisGroup;
	CSilentGroupBox TVisGroup;
	//Menu Groups
	CSilentGroupBox MenuGroup;
	CSilentGroupBox MenuBar2;
	CSilentGroupBox MenuInside;
	CSilentGroupBox MenuBackround;
	//CT Not Visible
	CSlider  BULLETCT_R;
	CSlider  BULLETCT_G;
	CSlider  BULLETCT_B;
	//CT Visible
	CSlider  BULLET_T_R;
	CSlider  BULLET_T_G;
	CSlider  BULLET_T_B;
	//T Not Visible
	CSlider  TNVisColorR;
	CSlider  TNVisColorG;
	CSlider  TNVisColorB;
	//T Visible
	CSlider  TVisColorR;
	CSlider  TVisColorG;
	CSlider  TVisColorB;

	//Menu Colors


	CSlider  MenuBar1R;
	CSlider  MenuBar1G;
	CSlider  MenuBar1B;
	CSlider  MenuBar2R;
	CSlider  MenuBar2G;
	CSlider  MenuBar2B;
	//Inner
	CSlider  MenuInnerR;
	CSlider  MenuInnerG;
	CSlider  MenuInnerB;

	CSlider  MenuBGR;
	CSlider  MenuBGG;
	CSlider  MenuBGB;
	CSlider  MenuOpacity;
};

class CSkinChanger : public CTab
{
public:
	void Setup();

};

class AyyWareWindow : public CWindow
{
public:
	void Setup();

	CRageBotTab RageBotTab;
	CLegitBotTab LegitBotTab;
	CVisualTab VisualsTab;
	CMiscTab MiscTab;

	CButton LoadButton;
	CButton SaveButton2;
	CButton LoadButton2;
	CButton SaveButton3;
	CButton LoadButton3;

	CSkinChanger SkinchangerTab;
	CColorsTab ColorsTab;


	CButton UnloadButton;
};

namespace Menu
{
	void SetupMenu();
	void DoUIFrame();

	extern AyyWareWindow Window;
};