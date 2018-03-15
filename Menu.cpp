/*
Syn's AyyWare Framework 2015
*/

#include "Menu.h"
#include "Controls.h"
#include "Hooks.h" // for the unload meme
#include "Interfaces.h"
#include "CRC32.h"

#define WINDOW_WIDTH 420
#define WINDOW_HEIGHT 545

AyyWareWindow Menu::Window;

void SaveLegitCallbk()
{
	switch (Menu::Window.MiscTab.lstPlayers.GetIndex())
	{
	case 0:
		GUI.SaveWindowState(&Menu::Window, "SlowwLegit1.cfg");
		break;
	case 1:
		GUI.SaveWindowState(&Menu::Window, "SlowwLegit2.cfg");
		break;
	case 2:
		GUI.SaveWindowState(&Menu::Window, "SlowwLegit3.cfg");
		break;
	case 3:
		GUI.SaveWindowState(&Menu::Window, "SlowwRage1.cfg");
		break;
	case 4:
		GUI.SaveWindowState(&Menu::Window, "SlowwRage2.cfg");
		break;
	case 5:
		GUI.SaveWindowState(&Menu::Window, "SlowwRage3.cfg");
		break;
	case 6:
		GUI.SaveWindowState(&Menu::Window, "SlowwMMHvH1.cfg");
		break;
	case 7:
		GUI.SaveWindowState(&Menu::Window, "SlowwMMHvH2.cfg");
		break;
	case 8:
		GUI.SaveWindowState(&Menu::Window, "SlowwMMHvH3.cfg");
		break;
	}
}

void LoadLegitCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar("cl_fullupdate");
	Interfaces::Engine->ClientCmd_Unrestricted("cl_fullupdate");

	switch (Menu::Window.MiscTab.lstPlayers.GetIndex())
	{
	case 0:
		GUI.LoadWindowState(&Menu::Window, "SlowwLegit1.cfg");
	
		break;
	case 1:
		GUI.LoadWindowState(&Menu::Window, "SlowwLegit2.cfg");
	
		break;
	case 2:
		GUI.LoadWindowState(&Menu::Window, "SlowwLegit3.cfg");
	
		break;
	case 3:
		GUI.LoadWindowState(&Menu::Window, "SlowwRage1.cfg");
		
		break;
	case 4:
		GUI.LoadWindowState(&Menu::Window, "SlowwRage2.cfg");
	
		break;
	case 5:
		GUI.LoadWindowState(&Menu::Window, "SlowwRage3.cfg");
	
		break;
	case 6:
		GUI.LoadWindowState(&Menu::Window, "SlowwMMHvH1.cfg");
		
		break;
	case 7:
		GUI.LoadWindowState(&Menu::Window, "SlowwMMHvH2.cfg");
	
		break;
	case 8:
		GUI.LoadWindowState(&Menu::Window, "SlowwMMHvH3.cfg");
		
		break;
	}
}

void UnLoadCallbk()
{
	DoUnload = true;
}

void KnifeApplyCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	Interfaces::Engine->ClientCmd_Unrestricted("cl_fullupdate");
}

void AyyWareWindow::Setup()
{
	SetPosition(300, 50);
	SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
//	SetTitle("                                                 Sloww.xyz");
	SetTitle("Sloww.xyz");

	RegisterTab(&RageBotTab);
	RegisterTab(&LegitBotTab);
	RegisterTab(&VisualsTab);
//	RegisterTab(&SkinchangerTab);
	RegisterTab(&MiscTab);
	RegisterTab(&ColorsTab);

	RECT Client = GetClientArea();
	Client.bottom -= 29;

	RageBotTab.Setup();
	LegitBotTab.Setup();
	VisualsTab.Setup();
//	SkinchangerTab.Setup();
	MiscTab.Setup();
	ColorsTab.Setup();

#pragma region Bottom Buttons
	SaveButton3.SetText("Save");
	SaveButton3.SetCallback(SaveLegitCallbk);
	SaveButton3.SetPosition(230, 220);

	LoadButton3.SetText("Load");
	LoadButton3.SetCallback(LoadLegitCallbk);
	LoadButton3.SetPosition(270, 220);

	MiscTab.RegisterControl(&SaveButton3);
	MiscTab.RegisterControl(&LoadButton3);
#pragma endregion Setting up the settings buttons
}

void CRageBotTab::Setup()
{
	SetTitle("Rage");
	
	ActiveLabel.SetPosition(320, 1);
	ActiveLabel.SetText("Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(370, 1);
	RegisterControl(&Active);

#pragma region Aimbot
	AimbotGroup.SetPosition(0, 8);
//	AimbotGroup.SetText("Aimbot");	
	AimbotGroup.SetSize(200, 380);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl("Enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl("Auto Fire", this, &AimbotAutoFire);

	AimbotFov.SetFileId("aim_fov");
	AimbotFov.SetBoundaries(0.f, 180.f);
	AimbotFov.SetValue(39.f);
	AimbotGroup.PlaceLabledControl("FOV Range", this, &AimbotFov);

	AimbotSilentAim.SetFileId("aim_silent");
	AimbotGroup.PlaceLabledControl("Silent Aim", this, &AimbotSilentAim);

	AimbotPerfectSilentAim.SetFileId("aim_psilent");
	AimbotGroup.PlaceLabledControl("Perfect Silent", this, &AimbotPerfectSilentAim);

	AimbotAutoPistol.SetFileId("aim_autopistol");
	AimbotGroup.PlaceLabledControl("Auto Pistol", this, &AimbotAutoPistol);

	AutoRevolver.SetFileId("aim_autor8");
	AimbotGroup.PlaceLabledControl("Auto Revolver", this, &AutoRevolver);

	AimbotKeyPress.SetFileId("aim_usekey");
	AimbotGroup.PlaceLabledControl("On Key Press", this, &AimbotKeyPress);

	AimbotKeyBind.SetFileId("aim_key");
	AimbotGroup.PlaceLabledControl("Key", this, &AimbotKeyBind);

//	AimbotStopKey.SetFileId("aim_stop");
//	AimbotGroup.PlaceLabledControl("Stop Aim", this, &AimbotStopKey);
#pragma endregion Aimbot Controls Get Setup in here

#pragma region Target

	// 	AimbotGroup.SetPosition(0, 10);

	TargetGroup.SetPosition(205, 8);
//	TargetGroup.SetText("Target");
	TargetGroup.SetSize(200, 380);
	RegisterControl(&TargetGroup);

	TargetSelection.SetFileId("tgt_selection");
	TargetSelection.AddItem("Crosshair");
	TargetSelection.AddItem("Distance");
	TargetSelection.AddItem("Health");
	TargetGroup.PlaceLabledControl("Selection", this, &TargetSelection);

	TargetFriendlyFire.SetFileId("tgt_friendlyfire");
	TargetGroup.PlaceLabledControl("Friendly Fire", this, &TargetFriendlyFire);

	TargetHitbox.SetFileId("tgt_hitbox");
	TargetHitbox.AddItem("Head");
	TargetHitbox.AddItem("Neck");
	TargetHitbox.AddItem("Chest");
	TargetHitbox.AddItem("Stomach");
	TargetHitbox.AddItem("Shins");
	TargetGroup.PlaceLabledControl("Hitbox", this, &TargetHitbox);

	TargetHitscan.SetFileId("tgt_hitscan");
	TargetHitscan.AddItem("Off"); //0
	TargetHitscan.AddItem("25%"); // 1
	TargetHitscan.AddItem("50%"); // 2
	TargetHitscan.AddItem("75%"); // 3
	TargetHitscan.AddItem("100%"); // 4
	TargetGroup.PlaceLabledControl("Hitscan", this, &TargetHitscan);

	TargetMultipoint.SetFileId("tgt_multipoint");
	TargetGroup.PlaceLabledControl("Multipoint", this, &TargetMultipoint);

	TargetPointscale.SetFileId("tgt_pointscale");
	TargetPointscale.SetBoundaries(0.f, 1.f);
	TargetPointscale.SetValue(0.5f);
	TargetGroup.PlaceLabledControl("Pointscale", this, &TargetPointscale);

	AccuracyResolver.SetFileId("acc_aaa");
	TargetGroup.PlaceLabledControl("Resolver", this, &AccuracyResolver);





#pragma endregion Targetting controls 

#pragma region Accuracy
	AccuracyGroup.SetPosition(0, 190);
	AccuracyGroup.SetText("Accuracy");
	AccuracyGroup.SetSize(200, 380);
	RegisterControl(&AccuracyGroup);

	AccuracyRecoil.SetFileId("acc_norecoil");
	AccuracyGroup.PlaceLabledControl("Anti Recoil", this, &AccuracyRecoil);

	AccuracyAutoWall.SetFileId("acc_awall");
	AccuracyGroup.PlaceLabledControl("Auto Wall", this, &AccuracyAutoWall);

	AccuracyMinimumDamage.SetFileId("acc_mindmg");
	AccuracyMinimumDamage.SetBoundaries(1.f, 99.f);
	AccuracyMinimumDamage.SetValue(1.f);
	AccuracyGroup.PlaceLabledControl("Autowall Damage", this, &AccuracyMinimumDamage);

	AccuracyAutoScope.SetFileId("acc_scope");
	AccuracyGroup.PlaceLabledControl("Auto Scope", this, &AccuracyAutoScope);

	AccuracyHitchance.SetFileId("acc_chance");
	AccuracyHitchance.SetBoundaries(0, 100);
	AccuracyHitchance.SetValue(0);
	AccuracyGroup.PlaceLabledControl("Hit Chance", this, &AccuracyHitchance);

	Fakelagvalue.SetFileId("acc_fakelag");
	Fakelagvalue.SetBoundaries(0, 13.9);
	AccuracyGroup.PlaceLabledControl("Fake Lag", this, &Fakelagvalue);

#pragma endregion  Accuracy controls get Setup in here

#pragma region AntiAim
	AntiAimGroup.SetPosition(205, 190);
	AntiAimGroup.SetText("Anti Aim");
	AntiAimGroup.SetSize(200, 380);
	RegisterControl(&AntiAimGroup);

	AntiAimEnable.SetFileId("aa_enable");
	AntiAimGroup.PlaceLabledControl("Enable", this, &AntiAimEnable);

	AntiAimPitch.SetFileId("aa_x");
	AntiAimPitch.AddItem("None");
	AntiAimPitch.AddItem("Down");
	AntiAimPitch.AddItem("Half Down");
	AntiAimGroup.PlaceLabledControl("Pitch", this, &AntiAimPitch);

	AntiAimYaw.SetFileId("antiaim_yaw");
	AntiAimYaw.AddItem("None");
	AntiAimYaw.AddItem("Backward");
	AntiAimYaw.AddItem("SW Switch");
	AntiAimYaw.AddItem("Fake Spin");
	AntiAimGroup.PlaceLabledControl("Yaw", this, &AntiAimYaw);

	SWSwitchKey.SetFileId("swswitchkey");
	SWSwitchKey.SetKey(88);
	AntiAimGroup.PlaceLabledControl("SW Switch", this, &SWSwitchKey);

	Freestanding.SetFileId("aa_freestand");
	AntiAimGroup.PlaceLabledControl("Free Standing", this, &Freestanding);

	AntiAimKnife.SetFileId("antiaim_onknife");
	AntiAimGroup.PlaceLabledControl("On Knife", this, &AntiAimKnife);

#pragma endregion  AntiAim controls get setup in here
}

void CLegitBotTab::Setup()
{
	SetTitle("Legit");

	ActiveLabel.SetPosition(320, 1);
	ActiveLabel.SetText("Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(370, 1);
	RegisterControl(&Active);

#pragma region Aimbot
	AimbotGroup.SetPosition(0, 8);
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetSize(240, 210);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl("Enable", this, &AimbotEnable);

	AimbotKeyPress.SetFileId("aim_usekey");
	AimbotGroup.PlaceLabledControl("On Key Press", this, &AimbotKeyPress);

	AimbotKeyBind.SetFileId("aim_key");
	AimbotGroup.PlaceLabledControl("Key Bind", this, &AimbotKeyBind);
	
	AimbotAutoPistol.SetFileId("aim_apistol");
	AimbotGroup.PlaceLabledControl("Auto Pistol", this, &AimbotAutoPistol);

#pragma endregion Aimbot shit



#pragma region Main Weapon
	WeaponMainGroup.SetPosition(0, 100);
	WeaponMainGroup.SetText("Rifles/Other");
	WeaponMainGroup.SetSize(240, 210);
	RegisterControl(&WeaponMainGroup);

	WeaponMainSpeed.SetFileId("main_speed");
	WeaponMainSpeed.SetBoundaries(0.1f, 100.f); 
	WeaponMainSpeed.SetValue(1.0f);
	WeaponMainGroup.PlaceLabledControl("Main AimTime", this, &WeaponMainSpeed);

	WeaponMainFoV.SetFileId("main_fov");
	WeaponMainFoV.SetBoundaries(0.1f, 30.f);
	WeaponMainFoV.SetValue(5.f);
	WeaponMainGroup.PlaceLabledControl("Main FoV", this, &WeaponMainFoV);

	WeaponMainRecoil.SetFileId("main_recoil");
	WeaponMainGroup.PlaceLabledControl("Recoil", this, &WeaponMainRecoil);

	WeaponMainPSilent.SetFileId("main_psilent");
	WeaponMainGroup.PlaceLabledControl("Silent", this, &WeaponMainPSilent);

	WeaponMainHitbox.SetFileId("main_hitbox");
	WeaponMainHitbox.AddItem("Head");
	WeaponMainHitbox.AddItem("Neck");
	WeaponMainHitbox.AddItem("Chest");
	WeaponMainHitbox.AddItem("Stomach");
	WeaponMainGroup.PlaceLabledControl("Hitbox", this, &WeaponMainHitbox);
#pragma endregion

#pragma region Pistols
	WeaponPistGroup.SetPosition(0, 200);
	WeaponPistGroup.SetText("Pistols");
	WeaponPistGroup.SetSize(240, 210);
	RegisterControl(&WeaponPistGroup);

	WeaponPistSpeed.SetFileId("pist_speed");
	WeaponPistSpeed.SetBoundaries(0.1f, 100.f);
	WeaponPistSpeed.SetValue(1.0f);
	WeaponPistGroup.PlaceLabledControl("Pistol AimTime", this, &WeaponPistSpeed);

	WeaponPistFoV.SetFileId("pist_fov");
	WeaponPistFoV.SetBoundaries(0.1f, 30.f);
	WeaponPistFoV.SetValue(5.f);
	WeaponPistGroup.PlaceLabledControl("Pistol FoV", this, &WeaponPistFoV);

	WeaponPistRecoil.SetFileId("pist_recoil");
	WeaponPistGroup.PlaceLabledControl("Recoil", this, &WeaponPistRecoil);

	WeaponPistPSilent.SetFileId("pist_psilent");
	WeaponPistGroup.PlaceLabledControl("Silent", this, &WeaponPistPSilent);


	WeaponPistHitbox.SetFileId("pist_hitbox");
	WeaponPistHitbox.AddItem("Head");
	WeaponPistHitbox.AddItem("Neck");
	WeaponPistHitbox.AddItem("Chest");
	WeaponPistHitbox.AddItem("Stomach");
	WeaponPistGroup.PlaceLabledControl("Hitbox", this, &WeaponPistHitbox);
#pragma endregion

#pragma region Snipers
	WeaponSnipGroup.SetPosition(0, 300);
	WeaponSnipGroup.SetText("Snipers");
	WeaponSnipGroup.SetSize(240, 210);
	RegisterControl(&WeaponSnipGroup);

	WeaponSnipSpeed.SetFileId("snip_speed");
	WeaponSnipSpeed.SetBoundaries(0.1f, 100.f);
	WeaponSnipSpeed.SetValue(1.0f);
	WeaponSnipGroup.PlaceLabledControl("Snipers AimTime", this, &WeaponSnipSpeed);

	WeaponSnipFoV.SetFileId("snip_fov");
	WeaponSnipFoV.SetBoundaries(0.1f, 30.f);
	WeaponSnipFoV.SetValue(5.f);
	WeaponSnipGroup.PlaceLabledControl("Snipers FoV", this, &WeaponSnipFoV);

	WeaponSnipRecoil.SetFileId("snip_recoil");
	WeaponSnipGroup.PlaceLabledControl("Recoil", this, &WeaponSnipRecoil);

	WeaponSnipPSilent.SetFileId("snip_psilent");
	WeaponSnipGroup.PlaceLabledControl("Silent", this, &WeaponSnipPSilent);

	WeaponSnipHitbox.SetFileId("snip_hitbox");
	WeaponSnipHitbox.AddItem("Head");
	WeaponSnipHitbox.AddItem("Neck");
	WeaponSnipHitbox.AddItem("Chest");
	WeaponSnipHitbox.AddItem("Stomach");
	WeaponSnipGroup.PlaceLabledControl("Hitbox", this, &WeaponSnipHitbox);
#pragma endregion*/
}

void CColorsTab::Setup()
{
	SetTitle("Colors");
#pragma region Colors
	CTNVisGroup.SetPosition(16, 16);
	CTNVisGroup.SetSize(360, 100);
	CTNVisGroup.SetText("BULLETTRACERS CT");
	RegisterControl(&CTNVisGroup);

	// CT NOT VISIBLE


	BULLETCT_R.SetFileId("ct_not_vis_r");
	BULLETCT_R.SetBoundaries(1, 255);
	BULLETCT_R.SetValue(15);
	CTNVisGroup.PlaceLabledControl("BULLETTRACER_CT_R", this, &BULLETCT_R);

	BULLETCT_G.SetFileId("ct_not_vis_g");
	BULLETCT_G.SetBoundaries(1, 255);
	BULLETCT_G.SetValue(1);
	CTNVisGroup.PlaceLabledControl("BULLETTRACER_CT_G", this, &BULLETCT_G);

	BULLETCT_B.SetFileId("ct_not_vis_b");
	BULLETCT_B.SetBoundaries(1, 255);
	BULLETCT_B.SetValue(220);
	CTNVisGroup.PlaceLabledControl("BULLETTRACER_CT_B", this, &BULLETCT_B);



	//CT VISIBLE


	CTVisGroup.SetPosition(16, 137);
	CTVisGroup.SetSize(360, 100);
	CTVisGroup.SetText("BULLETTRACERS T");
	RegisterControl(&CTVisGroup);



	BULLET_T_R.SetFileId("ct_vis_r");
	BULLET_T_R.SetBoundaries(1, 255);
	BULLET_T_R.SetValue(187);
	CTVisGroup.PlaceLabledControl("BULLETTRACER_T_R", this, &BULLET_T_R);

	BULLET_T_G.SetFileId("ct_vis_g");
	BULLET_T_G.SetBoundaries(1, 255);
	BULLET_T_G.SetValue(1);
	CTVisGroup.PlaceLabledControl("BULLETTRACER_T_G", this, &BULLET_T_G);

	BULLET_T_B.SetFileId("ct_vis_b");
	BULLET_T_B.SetBoundaries(1, 255);
	BULLET_T_B.SetValue(255);
	CTVisGroup.PlaceLabledControl("BULLETTRACER_T_B", this, &BULLET_T_B);

	

	// T Vis Color


#pragma endregion
	/**/
#pragma MenuGroup
	MenuGroup.SetPosition(16, 258);
	MenuGroup.SetSize(360, 100);
	MenuGroup.SetText("COOMNGSOON");
	RegisterControl(&MenuGroup);

	//Menu Gradient Color 1
	MenuBar1R.SetFileId("bar_grad_1R");
	MenuBar1R.SetBoundaries(0, 255);
	MenuBar1R.SetValue(0);
	MenuGroup.PlaceLabledControl("Red", this, &MenuBar1R);

	MenuBar1G.SetFileId("bar_grad_1G");
	MenuBar1G.SetBoundaries(0, 255);
	MenuBar1G.SetValue(150);
	MenuGroup.PlaceLabledControl("Green", this, &MenuBar1G);

	MenuBar1B.SetFileId("bar_grad_1B");
	MenuBar1B.SetBoundaries(0, 255);
	MenuBar1B.SetValue(0);
	MenuGroup.PlaceLabledControl("Blue", this, &MenuBar1B);


	


	//MenuOpacity.SetFileId("men_oapacity");
	//MenuOpacity.SetBoundaries(0, 255);
	//MenuOpacity.SetValue(0);
	//MenuGroup.PlaceLabledControl("Menu Opacity", this, &MenuOpacity);

	//Inner
	MenuInside.SetPosition(16, 379);
	MenuInside.SetSize(360, 100);
	MenuInside.SetText("COOMNGSOON");
	RegisterControl(&MenuInside);

	MenuInnerR.SetFileId("in_R");
	MenuInnerR.SetBoundaries(0, 255);
	MenuInnerR.SetValue(255);
	MenuInside.PlaceLabledControl("Red", this, &MenuInnerR);

	MenuInnerG.SetFileId("in_G");
	MenuInnerG.SetBoundaries(0, 255);
	MenuInnerG.SetValue(255);
	MenuInside.PlaceLabledControl("Green", this, &MenuInnerG);

	MenuInnerB.SetFileId("in_B");
	MenuInnerB.SetBoundaries(0, 255);
	MenuInnerB.SetValue(255);
	MenuInside.PlaceLabledControl("Stuff", this, &MenuInnerB);


}

void CVisualTab::Setup()
{
	SetTitle("Visuals");

	ActiveLabel.SetPosition(320, 1);
	ActiveLabel.SetText("Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(370, 1);
	RegisterControl(&Active);

#pragma region Options
//	OptionsGroup.SetText("Options");
//	OptionsGroup.SetPosition(0, 10);
//	OptionsGroup.SetSize(193, 430);

	OptionsGroup.SetPosition(0, 8);
	OptionsGroup.SetSize(200, 380);
	RegisterControl(&OptionsGroup);

	OptionsBox.SetFileId("opt_box");
	OptionsGroup.PlaceLabledControl("Box", this, &OptionsBox);

	OptionsName.SetFileId("opt_name");
	OptionsGroup.PlaceLabledControl("Name", this, &OptionsName);

	OptionsHealth.SetFileId("opt_hp");
	OptionsGroup.PlaceLabledControl("Health", this, &OptionsHealth);

	Armoresp.SetFileId("opt_armor");
	OptionsGroup.PlaceLabledControl("Armor", this, &Armoresp);

	OptionsWeapon.SetFileId("opt_weapon");
	OptionsGroup.PlaceLabledControl("Weapon", this, &OptionsWeapon);

	OptionsInfo.SetFileId("opt_info");
	OptionsGroup.PlaceLabledControl("Info", this, &OptionsInfo);

	OptionsChams.SetFileId("opt_chams");
	OptionsChams.AddItem("Off");
	OptionsChams.AddItem("Normal");
	OptionsChams.AddItem("Flat");
	OptionsGroup.PlaceLabledControl("Chams", this, &OptionsChams);

	OptionsSkeleton.SetFileId("opt_bone");
	OptionsGroup.PlaceLabledControl("Skeleton", this, &OptionsSkeleton);

	OptionsCompRank.SetFileId("opt_comprank");
	OptionsGroup.PlaceLabledControl("Player Ranks", this, &OptionsCompRank);

	FiltersPlayers.SetState(true);

	FiltersEnemiesOnly.SetFileId("ftr_enemyonly");
	OptionsGroup.PlaceLabledControl("Enemies Only", this, &FiltersEnemiesOnly);

	FiltersWeapons.SetFileId("ftr_weaps");
	OptionsGroup.PlaceLabledControl("Weapons", this, &FiltersWeapons);

	FiltersC4.SetFileId("ftr_c4");
	OptionsGroup.PlaceLabledControl("C4", this, &FiltersC4);

#pragma endregion Setting up the Options controls

#pragma region Filters
	FiltersGroup.SetText("Extra");
	FiltersGroup.SetPosition(205, 8);
	FiltersGroup.SetSize(200, 380);
	RegisterControl(&FiltersGroup);

	BulletTracers.SetFileId("bullettracers");
	FiltersGroup.PlaceLabledControl("Bullet Tracers", this, &BulletTracers);


	OtherNoVisualRecoil.SetFileId("otr_visrecoil");
	FiltersGroup.PlaceLabledControl("No Visual Recoil", this, &OtherNoVisualRecoil);

	OtherNoFlash.SetFileId("otr_noflash");
	FiltersGroup.PlaceLabledControl("No Flash", this, &OtherNoFlash);

	OtherNoScope.SetFileId("otr_noscope");
	FiltersGroup.PlaceLabledControl("No Scope", this, &OtherNoScope);

	OtherNoSmoke.SetFileId("otr_nosmoke");
	FiltersGroup.PlaceLabledControl("No Smoke", this, &OtherNoSmoke);

	FakeAngleChams.SetFileId("otr_anglechams");
	FiltersGroup.PlaceLabledControl("FakeAngleChams", this, &FakeAngleChams);

	FakeLines.SetFileId("otr_anglelines");
	FiltersGroup.PlaceLabledControl("Angle Lines", this, &FakeLines);

	ResolverInfo.SetFileId("otr_resolverinfo");
	FiltersGroup.PlaceLabledControl("Resolver Info", this, &ResolverInfo);

	//OtherRadar.SetFileId("otr_radar");
	//FiltersGroup.PlaceLabledControl("Radar", this, &OtherRadar);

	OtherNoHands.SetFileId("otr_hands");
	OtherNoHands.AddItem("Off");
	OtherNoHands.AddItem("None");
	OtherNoHands.AddItem("Transparent");
	OtherNoHands.AddItem("Chams");
	OtherNoHands.AddItem("Rainbow");
	FiltersGroup.PlaceLabledControl("Hands", this, &OtherNoHands);

	OtherViewmodelFOV.SetFileId("otr_viewfov");
	OtherViewmodelFOV.SetBoundaries(0.f, 180.f);
	OtherViewmodelFOV.SetValue(0.f);
	FiltersGroup.PlaceLabledControl("VMDL Changer", this, &OtherViewmodelFOV);

	OtherFOV.SetFileId("otr_fov");
	OtherFOV.SetBoundaries(0.f, 180.f);
	OtherFOV.SetValue(90.f);
	FiltersGroup.PlaceLabledControl("FOV Changer", this, &OtherFOV);
	
#pragma endregion Setting up the Filters controls


}

void CMiscTab::Setup()
{
	SetTitle("Misc");

#pragma region Other
	OtherGroup.SetPosition(16, 16);
	OtherGroup.SetSize(360, 430);
	OtherGroup.SetText("Other");
	RegisterControl(&OtherGroup);

	OtherAutoJump.SetFileId("otr_autojump");
	OtherAutoJump.AddItem("Off");
	OtherAutoJump.AddItem("Normal");
	OtherGroup.PlaceLabledControl("Auto Jump", this, &OtherAutoJump);

	OtherEdgeJump.SetFileId("otr_edgejump");
	OtherGroup.PlaceLabledControl("Edge Jump", this, &OtherEdgeJump);

	OtherAutoStrafe.SetFileId("otr_strafe");
	OtherAutoStrafe.AddItem("Off");
	OtherAutoStrafe.AddItem("Legit");
	OtherAutoStrafe.AddItem("Rage");
	OtherGroup.PlaceLabledControl("Auto Strafer", this, &OtherAutoStrafe);

	OtherClantag.SetFileId("otr_spam");
	OtherClantag.AddItem("Off");
	OtherClantag.AddItem("pasteware");
	OtherClantag.AddItem("skeet.cc");
	OtherClantag.AddItem("Blank");
	OtherClantag.AddItem("Valve");
	OtherGroup.PlaceLabledControl("Custom Clantag", this, &OtherClantag);


	OtherSpectators.SetFileId("otr_speclist");
	OtherGroup.PlaceLabledControl("Spectators List", this, &OtherSpectators);

	OtherThirdperson.SetFileId("thirdperson");
	OtherGroup.PlaceLabledControl("Thirdperson", this, &OtherThirdperson);

	ThirdPersonKeyBind.SetFileId("otr_tpkey");
	OtherGroup.PlaceLabledControl("Thirdp Key:", this, &ThirdPersonKeyBind);

	OtherThirdpersonRange.SetFileId("thirdpersonrange");
	OtherThirdpersonRange.SetBoundaries(1, 1000);
	OtherThirdpersonRange.SetValue(180);
	OtherGroup.PlaceLabledControl("Thirdp Range", this, &OtherThirdpersonRange);

	OtherThirdpersonType.SetFileId("thirdpersonangles");
	OtherThirdpersonType.AddItem("Real");
	OtherThirdpersonType.AddItem("Fake");
	OtherGroup.PlaceLabledControl("Thirdp Angles", this, &OtherThirdpersonType);

	lstPlayers.SetFileId("configs");
	lstPlayers.AddItem("Legit 1");
	lstPlayers.AddItem("Legit 2");
	lstPlayers.AddItem("Legit 3");
	lstPlayers.AddItem("Rage 1");
	lstPlayers.AddItem("Rage 2");
	lstPlayers.AddItem("Rage 3");
	lstPlayers.AddItem("MM HvH 1");
	lstPlayers.AddItem("MM HvH 2");
	lstPlayers.AddItem("MM HvH 3");
	OtherGroup.PlaceLabledControl("Configs", this, &lstPlayers);

#pragma endregion other random options*/

	OtherSafeMode.SetState(true);
}

void Menu::SetupMenu()
{
	Window.Setup();

	GUI.RegisterWindow(&Window);
	GUI.BindWindow(VK_INSERT, &Window);
	GUI.BindWindow(VK_F11, &Window);
}

void Menu::DoUIFrame()
{
	// General Processing

	// If the "all filter is selected tick all the others
	if (Window.VisualsTab.FiltersAll.GetState())
	{
		Window.VisualsTab.FiltersAll.SetState(false);
		Window.VisualsTab.FiltersC4.SetState(true);
		Window.VisualsTab.FiltersChickens.SetState(true);
		Window.VisualsTab.FiltersPlayers.SetState(true);
		Window.VisualsTab.FiltersWeapons.SetState(true);
	}

	GUI.Update();
	GUI.Draw();

	
}


