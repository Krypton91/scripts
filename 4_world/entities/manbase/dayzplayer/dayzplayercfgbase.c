

//-----------------------------------------------------------------------------------------------
// DayZPlayerTypeCreate - main dayz type config
//-----------------------------------------------------------------------------------------------

void DayZPlayerTypeCreate(DayZPlayerType pType)
{
//	Print("DayZPlayerTypeCreate called");

	//! register all item types
	DayZPlayerTypeRegisterItems(pType);

	//! register all sounds
	DayZPlayerTypeRegisterSounds(pType);
	
	//! register camera creators
	DayZPlayerCameras.RegisterCameras(pType);
 
	//! other configs 

	//! look limits 
	//! look limits 
	pType.SetLookLimits(-85, 85, -160, 160);	
	
	//! aiming limits 	
	pType.SetAimLimits(-85, 85, -180, 180);		

	//! camera shoot params
	pType.SetCameraShootParams(250, 0.01, 0.866);
	
	//! register hit components for AI melee
	pType.RegisterHitComponentsForAI();
	

	//--------------------------------------------------------------
    // VIKTOR, JOHNNY - nastaveni Movu !!!

	//! set move settings: 
	SHumanCommandMoveSettings hmcs = pType.CommandMoveSettingsW();

	//! run sprint (SHIFT HOLD) filter 
	hmcs.m_fRunSpringTimeout	= 0.1;							//!< filter span value		[s]
	hmcs.m_fRunSpringMaxChange	= 4.0;							//!< filter speed value		[val/s]

	//! WSAD direction filter 
	hmcs.m_fDirFilterTimeout	= 0.1;						//!< direction filter timeout [s]
	hmcs.m_fDirFilterSpeed		= Math.DEG2RAD * 360; 		//!< direction filter max rot speed [rad/s]
	hmcs.m_fMaxSprintAngle		= Math.DEG2RAD * 45.01;		//!< max sprint angle [rad]

	hmcs.m_fTurnAngle 			= 45;						//!< this is when char starts tu turn
	hmcs.m_fTurnEndUpdateNTime	= 0.7;						//!< this is end norm time (turn amount is not updated after this time)
	hmcs.m_fTurnNextNTime		= 0.9;						//!< this is norm time when new turn from old turn is evaluated again (turn->turn)


	hmcs.m_fHeadingChangeLimiterIdle 	= 500000; 			//!<
	hmcs.m_fHeadingChangeLimiterWalk 	= 2000;				//!<
	hmcs.m_fHeadingChangeLimiterRun		= 1500;				//!<
	
	hmcs.m_fLeaningSpeed				= 4.5;				//!<


	// swim settings
		

	SHumanCommandClimbSettings hcls = pType.CommandClimbSettingsW();


	//--------------------------------------------------------------
    // VIKTOR, JOHNNY, PETERN - nastaveni speedu animaci

	// DayZPlayerUtils.DebugOverrideAnimationSpeed("dz/anims/anm/player/moves/rifles/p_rfl_erc_runF_low.anm", 2.0);
	//DayZPlayerUtils.DebugOverrideAnimationTranslation("dz/anims/anm/player/moves/rifles/p_rfl_erc_runF_low.anm", "0 0 10" );
	
	//Melee evades
	//LEFT
	//DayZPlayerUtils.DebugOverrideAnimationSpeed("dz/anims/anm/player/moves/unarmed/p_erc_evadeL_ras.anm", 2.0);
	//DayZPlayerUtils.DebugOverrideAnimationTranslation("dz/anims/anm/player/moves/unarmed/p_erc_evadeL_ras.anm", "0 0 10");
	
	//RIGHT
	//DayZPlayerUtils.DebugOverrideAnimationSpeed("dz/anims/anm/player/moves/unarmed/p_erc_evadeR_ras.anm", 1);
	//DayZPlayerUtils.DebugOverrideAnimationTranslation("dz/anims/anm/player/moves/unarmed/p_erc_evadeR_ras.anm", "2 0 0");
	
	//BACK
	//DayZPlayerUtils.DebugOverrideAnimationSpeed("dz/anims/anm/player/moves/unarmed/p_erc_evadeB_ras.anm", 2.0);
	//DayZPlayerUtils.DebugOverrideAnimationTranslation("dz/anims/anm/player/moves/unarmed/p_erc_evadeB_ras.anm", "0 0 10");

}


//-----------------------------------------------------------------------------------------------
// ITEMS 
//-----------------------------------------------------------------------------------------------
enum ItemBehaviorType
{
	EMPTYHANDED,
	ONEHANDED,
	TWOHANDED,
	PISTOLS,
	FIREARMS,
	HEAVY
};

class DayzPlayerItemBehaviorCfg extends HumanItemBehaviorCfg
{
	void DayzPlayerItemBehaviorCfg()
	{
		m_iStanceMask = DayZPlayerConstants.STANCEMASK_ALL;

		m_StanceMovements[DayZPlayerConstants.STANCEIDX_ERECT] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_RUN | DayZPlayerConstants.MOVEMENT_SPRINT;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_CROUCH] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_SPRINT;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_PRONE] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_RAISEDERECT] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_RUN | DayZPlayerConstants.MOVEMENT_SPRINT;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_RAISEDCROUCH] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_RUN;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_RAISEDPRONE] = DayZPlayerConstants.MOVEMENT_IDLE;

		m_StanceRotation[DayZPlayerConstants.STANCEIDX_ERECT] = DayZPlayerConstants.ROTATION_ENABLE;
		m_StanceRotation[DayZPlayerConstants.STANCEIDX_CROUCH] = DayZPlayerConstants.ROTATION_ENABLE;
		m_StanceRotation[DayZPlayerConstants.STANCEIDX_PRONE] = DayZPlayerConstants.ROTATION_ENABLE;
		m_StanceRotation[DayZPlayerConstants.STANCEIDX_RAISEDERECT] = DayZPlayerConstants.ROTATION_ENABLE;
		m_StanceRotation[DayZPlayerConstants.STANCEIDX_RAISEDCROUCH] = DayZPlayerConstants.ROTATION_ENABLE;
		m_StanceRotation[DayZPlayerConstants.STANCEIDX_RAISEDPRONE] = DayZPlayerConstants.ROTATION_ENABLE;
		
		m_iPerItemCameraUserData = DayZPlayerCameras.PERITEMUD_INVALID;		//! per item camera user data to be uninitialized

		//! rotational delay to camera direction in move only (filter)
		m_fMoveHeadingFilterSpan 	= 0.15;	                // 
		m_fMoveHeadingFilterSpeed	= Math.DEG2RAD * 300;	// 720 stup / sek

		m_fMeleeEvadeHeadingFilterSpan	= 0.07;	                // 
		m_fMeleeEvadeHeadingFilterSpeed	= Math.DEG2RAD * 1440;	// 720 stup / sek

		m_bAttackLean = false;
		m_bJumpAllowed = true;
	}

	void 	SetPistols()
	{
		m_iType = ItemBehaviorType.PISTOLS;
		
		SetIKStance(DayZPlayerConstants.STANCEIDX_ERECT, false, true, false);
		SetIKStance(DayZPlayerConstants.STANCEIDX_CROUCH, false, true, false);
		SetIKStance(DayZPlayerConstants.STANCEIDX_PRONE, false, true, false);
		SetIKStance(DayZPlayerConstants.STANCEIDX_RAISEDERECT, true, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_RAISEDCROUCH, true, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_RAISEDPRONE, true, true, true);

		SetIK(DayZPlayerConstants.STANCEIDX_RAISEDERECT, DayZPlayerConstants.MOVEMENTIDX_SPRINT, false, true, false);
	}

	void 	SetFirearms()
	{
		m_iType = ItemBehaviorType.FIREARMS;
		
		SetIKStance(DayZPlayerConstants.STANCEIDX_ERECT, false, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_CROUCH, false, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_PRONE, false, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_RAISEDERECT, true, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_RAISEDCROUCH, true, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_RAISEDPRONE, true, true, true);
	}

	void 	SetIKTwoHanded()
	{
		SetIKStance(DayZPlayerConstants.STANCEIDX_ERECT, false, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_CROUCH, false, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_PRONE, false, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_RAISEDERECT, false, true, true);
		SetIKStance(DayZPlayerConstants.STANCEIDX_RAISEDCROUCH, false, false, false);
		SetIKStance(DayZPlayerConstants.STANCEIDX_RAISEDPRONE, false, true, true);
	}

	void	SetEmptyHanded()
	{
		m_iType = ItemBehaviorType.EMPTYHANDED;
		
		m_iStanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_RAISEDERECT | DayZPlayerConstants.STANCEMASK_RAISEDPRONE;
		m_iPerItemCameraUserData = DayZPlayerCameras.PERITEMUD_EMPTYHANDED;

		m_StanceMovements[DayZPlayerConstants.STANCEIDX_RAISEDERECT] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_RUN;
		
		SetIKAll(false, false, false);

		m_bAttackLean = true;
	}
	
	void	SetTwoHanded()
	{
		m_iType = ItemBehaviorType.TWOHANDED;

		m_iStanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_RAISEDERECT | DayZPlayerConstants.STANCEMASK_RAISEDPRONE;
	  	m_iPerItemCameraUserData = DayZPlayerCameras.PERITEMUD_TWOHANDED;

		m_StanceMovements[DayZPlayerConstants.STANCEIDX_RAISEDERECT] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_RUN;

		SetIKTwoHanded();

		m_bAttackLean = true;
	}

	void	SetToolsOneHanded()
	{
		m_iType = ItemBehaviorType.ONEHANDED;

		m_iStanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_RAISEDERECT | DayZPlayerConstants.STANCEMASK_RAISEDPRONE;
		m_iPerItemCameraUserData = DayZPlayerCameras.PERITEMUD_ONEHANDED;

		m_StanceMovements[DayZPlayerConstants.STANCEIDX_RAISEDERECT] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_RUN;
		
		SetIKAll(false, true, false);
		
		m_bAttackLean = true;
	}

	void	SetRestrained()
	{
		m_iType = ItemBehaviorType.EMPTYHANDED;
		
		m_iStanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_PRONE;
		m_iPerItemCameraUserData = DayZPlayerCameras.PERITEMUD_ONEHANDED;

		m_StanceMovements[DayZPlayerConstants.STANCEIDX_ERECT] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_SPRINT;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_CROUCH] = DayZPlayerConstants.MOVEMENT_IDLE  | DayZPlayerConstants.MOVEMENT_WALK;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_PRONE] = DayZPlayerConstants.MOVEMENT_IDLE;
		
		m_StanceRotation[DayZPlayerConstants.STANCEIDX_PRONE] = DayZPlayerConstants.ROTATION_DISABLE;
		
		SetIKAll(false, true, true);
		
		m_bAttackLean = false;
	}

	void	SetSurrender()
	{
		m_iType = ItemBehaviorType.EMPTYHANDED;
		
		m_iStanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_PRONE;
		m_iPerItemCameraUserData = DayZPlayerCameras.PERITEMUD_EMPTYHANDED;

		m_StanceMovements[DayZPlayerConstants.STANCEIDX_ERECT] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_CROUCH] = DayZPlayerConstants.MOVEMENT_IDLE;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_PRONE] = DayZPlayerConstants.MOVEMENT_IDLE;
		
		m_StanceRotation[DayZPlayerConstants.STANCEIDX_PRONE] = DayZPlayerConstants.ROTATION_DISABLE;

		m_bAttackLean = false;
	}
	
	void	SetHeavyItems()
	{
		m_iType = ItemBehaviorType.HEAVY;

		m_iStanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_ERECT] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK;
		m_iPerItemCameraUserData = DayZPlayerCameras.PERITEMUD_TWOHANDED;
	  
		SetIKAll(false, true, true);
		
		m_bAttackLean = true;
		m_bJumpAllowed = false;
	}
}


class ModItemRegisterCallbacks
{
	void RegisterEmptyHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior);

    void RegisterOneHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior);

    void RegisterTwoHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior);

    void RegisterRestrained(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior);

    void RegisterSurrender(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior);

    void RegisterHeavy(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior);

    void RegisterPistol(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior);

    void RegisterFireArms(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior);

    void RegisterArcheryItem(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior);

    void RegisterLauncher(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior);

	void RegisterCustom(DayZPlayerType pType);

	void CustomBoneRemapping(DayZPlayerType pType);
}

void DayZPlayerTypeRegisterItems(DayZPlayerType pType)
{
	GetGame().ProfilerStart("DayZPlayerTypeRegisterItems");
	
	ModItemRegisterCallbacks modCallbacks = new ModItemRegisterCallbacks();
		
	//! behaviour 
	DayzPlayerItemBehaviorCfg 	emptyHanded = new DayzPlayerItemBehaviorCfg;
	emptyHanded.SetEmptyHanded();
	
	DayzPlayerItemBehaviorCfg 	toolsOneHanded = new DayzPlayerItemBehaviorCfg;
	toolsOneHanded.SetToolsOneHanded();
	
	DayzPlayerItemBehaviorCfg 	twoHanded = new DayzPlayerItemBehaviorCfg;
	twoHanded.SetTwoHanded();
	
	DayzPlayerItemBehaviorCfg 	restrained = new DayzPlayerItemBehaviorCfg;
	restrained.SetRestrained();
	
	DayzPlayerItemBehaviorCfg 	surrender = new DayzPlayerItemBehaviorCfg;
	surrender.SetSurrender();
	
	DayzPlayerItemBehaviorCfg 	heavyItemBehaviour = new DayzPlayerItemBehaviorCfg;
	heavyItemBehaviour.SetHeavyItems();
	
	//! set the default - use default instance set + default behaviour - empty hands
	pType.SetDefaultItemInHandsProfile("", emptyHanded);

	//! reset previous setup 
	pType.ResetItemInHandsProfiles();
	
	//! custom emptyhanded - really needed?
	modCallbacks.RegisterEmptyHanded(pType, emptyHanded);

	//! complete root of any item not specified here
	{
		pType.AddItemInHandsProfileIK("Inventory_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, "dz/anims/anm/player/ik/gear/apple.anm");
	}

	//! Pistols (RH,LH IK)
	{
		DayzPlayerItemBehaviorCfg 	pistolItemBehaviour = new DayzPlayerItemBehaviorCfg;

		pistolItemBehaviour.SetPistols();
		
		pType.AddItemInHandsProfileIK("Colt1911_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_1911.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/1911.anm", "dz/anims/anm/player/reloads/1911/w_1911_states.anm");
		pType.AddItemInHandsProfileIK("CZ75_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_cz75.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/CZ75.anm", "dz/anims/anm/player/reloads/CZ75/w_CZ75_states.anm");
		pType.AddItemInHandsProfileIK("DartGun", "dz/anims/workspaces/player/player_main/weapons/player_main_x2dartgun.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/X2.anm");
		pType.AddItemInHandsProfileIK("Derringer_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_derringer.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/derringer.anm");
		pType.AddItemInHandsProfileIK("Flaregun_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_flaregun.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/flaregun.anm");
		pType.AddItemInHandsProfileIK("FNX45_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_fnp45.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/fnp45.anm", "dz/anims/anm/player/reloads/FNP45/w_fnp45_states.anm");
		pType.AddItemInHandsProfileIK("Glock19_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_glock.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/Glock19.anm", "dz/anims/anm/player/reloads/Glock/w_Glock19_states.anm");
		pType.AddItemInHandsProfileIK("LongHorn_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_longhorn.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/longhorn.anm");
		pType.AddItemInHandsProfileIK("Magnum_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_magnum.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/magnum.anm", "dz/anims/anm/player/reloads/magnum/w_magnum_states.anm");
		pType.AddItemInHandsProfileIK("MKII_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_rugermk2.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/MK2.anm", "dz/anims/anm/player/reloads/RugerMK2/w_MK2_states.anm");
		pType.AddItemInHandsProfileIK("pb6p9", "dz/anims/workspaces/player/player_main/weapons/player_main_pb6p9.asi",pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/PB6P9.anm");
		pType.AddItemInHandsProfileIK("MakarovIJ70_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_ij70.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/ij70.anm", "dz/anims/anm/player/reloads/IJ70/w_IJ70_states.anm");
		pType.AddItemInHandsProfileIK("P1_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_p1.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/P1.anm");
		pType.AddItemInHandsProfileIK("Red9_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_mauserred9.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/Red9.anm");
		pType.AddItemInHandsProfileIK("Shockpistol_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_taser.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/Shockpistol.anm");
		pType.AddItemInHandsProfileIK("Deagle_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_deagle.asi", pistolItemBehaviour, "dz/anims/anm/player/ik/weapons/Deagle.anm", "dz/anims/anm/player/reloads/Deagle/w_Deagle_states.anm"); //TODO; placeholder
		
		modCallbacks.RegisterPistol(pType, pistolItemBehaviour);
	}

	//! Firearms (RH,LH IK)		
	{
		DayzPlayerItemBehaviorCfg 	fireArmsItemBehaviour = new DayzPlayerItemBehaviorCfg;
		
		fireArmsItemBehaviour.SetFirearms();
		
		pType.AddItemInHandsProfileIK("AKS74U", "dz/anims/workspaces/player/player_main/weapons/player_main_ak74u.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/AKS74U.anm", "dz/anims/anm/player/reloads/AK74U/w_AKS74U_states.anm");
		pType.AddItemInHandsProfileIK("AK74_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_ak101.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/ak101.anm", "dz/anims/anm/player/reloads/AK101/w_AK101_states.anm");
		pType.AddItemInHandsProfileIK("AK101_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_ak101.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/ak101.anm", "dz/anims/anm/player/reloads/AK101/w_AK101_states.anm");
		pType.AddItemInHandsProfileIK("AKM_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_akm.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/akm.anm", "dz/anims/anm/player/reloads/AKM/w_akm_states.anm");
		pType.AddItemInHandsProfileIK("AugSteyr_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_aug.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/aug.anm");
		pType.AddItemInHandsProfileIK("B95_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_b95.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/b95.anm", "dz/anims/anm/player/reloads/B95/w_B95_states.anm"); 
		pType.AddItemInHandsProfileIK("CZ61_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_cz61.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/CZ61.anm", "dz/anims/anm/player/reloads/cz61/w_cz61_states.anm");
		pType.AddItemInHandsProfileIK("CZ527_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_cz527.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/cz527.anm", "dz/anims/anm/player/reloads/cz527/w_CZ527_states.anm");
		pType.AddItemInHandsProfileIK("FAL_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_fnfal.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/fn_fal.anm",  "dz/anims/anm/player/reloads/FNFal/w_FNFal_states.anm");
		pType.AddItemInHandsProfileIK("Izh18_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_izh18.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/izh18.anm", "dz/anims/anm/player/reloads/IZH18/w_izh18_states.anm");
		pType.AddItemInHandsProfileIK("M4A1_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_m4a1.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/m4a1_ik.anm", "dz/anims/anm/player/reloads/M4A1/w_M4A1_states.anm");
		pType.AddItemInHandsProfileIK("M249_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_m249.asi",fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/M249.anm");
		pType.AddItemInHandsProfileIK("Mosin9130_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_mosin1903.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/mosin9130.anm","dz/anims/anm/player/reloads/Mosin_1903/w_mosin1903_states.anm");
		pType.AddItemInHandsProfileIK("SawedoffMosin9130_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_mosin1903.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/mosin9130.anm","dz/anims/anm/player/reloads/Mosin_1903/w_mosin1903_states.anm");
		pType.AddItemInHandsProfileIK("MP5K_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_mp5.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/mp5k.anm","dz/anims/anm/player/reloads/mp5/w_mp5k_states.anm");
		pType.AddItemInHandsProfileIK("PM73Rak_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_pm73.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/pm63rak.anm");
		pType.AddItemInHandsProfileIK("Repeater_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_repeater.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/repeater.anm", "dz/anims/anm/player/reloads/repeater/w_repeater_states.anm");
		pType.AddItemInHandsProfileIK("Ruger1022_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_ruger1022.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/ruger1022.anm", "dz/anims/anm/player/reloads/ruger1022/w_Ruger1022_states.anm");
		pType.AddItemInHandsProfileIK("SKS_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_sks.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/SKS.anm", "dz/anims/anm/player/reloads/SKS/w_SKS_states.anm");
		pType.AddItemInHandsProfileIK("SVD_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_svd.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/svd.anm","dz/anims/anm/player/reloads/SVD/w_SVD_states.anm");
		pType.AddItemInHandsProfileIK("Trumpet_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_trumpet.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/trumpet.anm");
		pType.AddItemInHandsProfileIK("UMP45_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_ump45.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/ump.anm","dz/anims/anm/player/reloads/UMP45/w_UMP45_states.anm");
		pType.AddItemInHandsProfileIK("VSS_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_vss.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/VSS.anm","dz/anims/anm/player/reloads/VSS/w_VSS_states.anm");
		pType.AddItemInHandsProfileIK("Winchester70_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_winchester70.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/winchester70.anm", "dz/anims/anm/player/reloads/Winchester70/w_winchester70_states.anm");

		//! Shotguns
		pType.AddItemInHandsProfileIK("Izh43Shotgun_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_izh43.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/izh43.anm", "dz/anims/anm/player/reloads/IZH43/w_IZH43_states.anm");
		pType.AddItemInHandsProfileIK("Mp133Shotgun_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_mp133.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/mp133.anm","dz/anims/anm/player/reloads/MP133/w_mp133_states.anm");
		pType.AddItemInHandsProfileIK("Mp133Shotgun_PistolGrip", "dz/anims/workspaces/player/player_main/weapons/player_main_mp133grip.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/mp133_pistol_grip.anm");
		pType.AddItemInHandsProfileIK("Saiga_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_Saiga.asi", fireArmsItemBehaviour, "dz/anims/anm/player/ik/weapons/Saiga.anm", "dz/anims/anm/player/reloads/Saiga/w_Saiga_states.anm");
		
		modCallbacks.RegisterFireArms(pType, fireArmsItemBehaviour);
	}
	
	//! Archery (RH,LH IK)
	{
		DayzPlayerItemBehaviorCfg 	archeryItemBehaviour = new DayzPlayerItemBehaviorCfg;
		//! Bows
		pType.AddItemInHandsProfileIK("PVCBow", "dz/anims/workspaces/player/player_main/player_main_bow.asi", archeryItemBehaviour,						"dz/anims/anm/player/ik/weapons/bow_erc_IK.anm");
		pType.AddItemInHandsProfileIK("QuickieBow", "dz/anims/workspaces/player/player_main/weapons/player_main_bow_quicky.asi", archeryItemBehaviour, 	"dz/anims/anm/player/ik/weapons/bow_erc_IK.anm");
		pType.AddItemInHandsProfileIK("RecurveBow", "dz/anims/workspaces/player/player_main/weapons/player_main_bow_recurve.asi", archeryItemBehaviour,	"dz/anims/anm/player/ik/weapons/bow_erc_recurve_IK.anm");
		pType.AddItemInHandsProfileIK("Crossbow", "dz/anims/workspaces/player/player_main/weapons/player_main_crossbow.asi", archeryItemBehaviour,		"dz/anims/anm/player/ik/weapons/crossbow.anm");
		
		//! Arrows
		pType.AddItemInHandsProfileIK("Ammo_ArrowComposite", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/arrow_hunting.anm");
		pType.AddItemInHandsProfileIK("Ammo_ArrowBolt", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/arrow_bolt.anm");
		pType.AddItemInHandsProfileIK("Ammo_ArrowBoned", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/arrow_hunting.anm");
		pType.AddItemInHandsProfileIK("Ammo_ArrowPrimitive", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/arrow_hunting.anm");
		pType.AddItemInHandsProfileIK("Ammo_SharpStick", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/arrow_hunting.anm");
		
		
		//! Arrows
		pType.AddItemInHandsProfileIK("Mag_Arrows_Quiver", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/ammunition/9mm_25rnd_box.anm");
		pType.AddItemInHandsProfileIK("Mag_Bolts_Quiver", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/ammunition/9mm_25rnd_box.anm");
	
		
		modCallbacks.RegisterArcheryItem(pType, archeryItemBehaviour);
	}
	
	//! Launchers
	{
		DayzPlayerItemBehaviorCfg 	launcherItemBehaviour = new DayzPlayerItemBehaviorCfg;

		//!pType.AddItemInHandsProfile("GP25Base", "dz/anims/workspaces/player/player_main/weapons/player_main_m72law.asi", launcherItemBehaviour);	
		//!pType.AddItemInHandsProfile("LAWBase", "dz/anims/workspaces/player/player_main/weapons/player_main_m72law.asi", launcherItemBehaviour);	
		//!pType.AddItemInHandsProfile("M203Base", "dz/anims/workspaces/player/player_main/weapons/player_main_m72law.asi", launcherItemBehaviour);
		//!pType.AddItemInHandsProfile("RPG7Base", "dz/anims/workspaces/player/player_main/weapons/player_main_m72law.asi", launcherItemBehaviour);
		
		modCallbacks.RegisterLauncher(pType, launcherItemBehaviour);
	}

	//! Ammunition
	{
		pType.AddItemInHandsProfileIK("Ammunition_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		
		
		//! Boxes
		pType.AddItemInHandsProfileIK("AmmoBox_9x19_25rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/9mm_25rnd_box.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_00buck_10rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/00Buck_10RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_762x39_20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/762x39_20RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_762x54_20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/762_20RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_22_50Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 	"dz/anims/anm/player/ik/ammunition/22_50RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_556x45_20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/556x20_20RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_357_20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 		"dz/anims/anm/player/ik/ammunition/357_20RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_545x39_20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/545x39_20RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_380_35rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 		"dz/anims/anm/player/ik/ammunition/380auto_35RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_45ACP_25rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/45acp_25rnd_box.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_308Win_20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/308Win_20RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_9x39_20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/9x39_20RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_9x39AP_20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/9x39_20RoundBox.anm");

		// New ammobox classes
		pType.AddItemInHandsProfileIK("AmmoBox_556x45Tracer_20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/556x20_20RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_308WinTracer_20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/308Win_20RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_762x54Tracer_20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/762_20RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_762x39Tracer_20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/762x39_20RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_12gaSlug_10Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/00Buck_10RoundBox.anm");
		pType.AddItemInHandsProfileIK("AmmoBox_545x39Tracer_20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			 	"dz/anims/anm/player/ik/ammunition/545x39_20RoundBox.anm");
		
		
		
		//! Loose Rounds
		pType.AddItemInHandsProfileIK("Ammo_9x19", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/9mm_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_357", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/ammunition/357_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_22", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/ammunition/22_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_380", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/ammunition/9mm_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_45ACP", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/9mm_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_762x39", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/762x39_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_762x54", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/762x54_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_556x45", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/556x45_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_Flare", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/Flare_SingleRound.anm");
		pType.AddItemInHandsProfileIK("Ammo_545x39", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/545x39_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_9x39", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/9x39_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_9x39AP", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/9x39_LooseRounds.anm");
		
		// New ammo classes
		pType.AddItemInHandsProfileIK("Ammo_308Win", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/9x39_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_308WinTracer", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/9x39_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_556x45Tracer", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/556x45_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_762x54Tracer", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/762x54_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_762x39Tracer", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/762x39_LooseRounds.anm");
		pType.AddItemInHandsProfileIK("Ammo_545x39Tracer", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/545x39_LooseRounds.anm");
		
		pType.AddItemInHandsProfileIK("Ammo_12gaPellets", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/12ga_00buck.anm");
		pType.AddItemInHandsProfileIK("Ammo_12gaSlug", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/ammunition/12ga_00buck.anm");
		
		pType.AddItemInHandsProfileIK("Ammo_LAW_HE", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 		"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("Ammo_LAW_HE", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 		"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured

	}

	//! Attachments
	{	
		//! light
		pType.AddItemInHandsProfileIK("UniversalLight", "dz/anims/workspaces/player/player_main/props/player_main_1h_flashlight.asi", toolsOneHanded,				"dz/anims/anm/player/ik/attachments/light/universallight.anm");
		pType.AddItemInHandsProfileIK("TLRLight", "dz/anims/workspaces/player/player_main/props/player_main_1h_flashlight.asi", toolsOneHanded,						"dz/anims/anm/player/ik/attachments/light/tlrlight.anm");
	
		//! magazine
		pType.AddItemInHandsProfileIK("Magazine_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/banana.anm");
		pType.AddItemInHandsProfileIK("Mag_IJ70_8Rnd", "dz/anims/workspaces/player/player_main/props/player_main_fnp45_mag.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/magazine_ij70.anm");
		pType.AddItemInHandsProfileIK("Mag_FNX45_15Rnd", "dz/anims/workspaces/player/player_main/props/player_main_fnp45_mag.asi", toolsOneHanded,		"dz/anims/anm/player/ik/attachments/magazines/magazine_fnp45.anm");
		pType.AddItemInHandsProfileIK("Mag_MP5_15Rnd", "dz/anims/workspaces/player/player_main/props/player_main_mp5k_mag.asi", toolsOneHanded,			"dz/anims/anm/player/ik/attachments/magazines/magazine_mp5_15rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_MP5_30Rnd", "dz/anims/workspaces/player/player_main/props/player_main_mp5k_mag.asi", toolsOneHanded,			"dz/anims/anm/player/ik/attachments/magazines/magazine_mp5_30rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_MKII_10Rnd", "dz/anims/workspaces/player/player_main/props/player_main_fnp45_mag.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/magazine_mk2.anm");
		pType.AddItemInHandsProfileIK("Mag_AKM_Drum75Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/magazines/magazine_akm_drum.anm");
		pType.AddItemInHandsProfileIK("Mag_CLIP762x39_10Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/magazines/clip_762x39_10rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_CLIP762x54_5Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/magazines/clip_762x54_5Rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_CLIP9x19_10Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/magazines/clip_9x19_10rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_CZ75_15Rnd", "dz/anims/workspaces/player/player_main/props/player_main_fnp45_mag.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/magazine_CZ75.anm");
		pType.AddItemInHandsProfileIK("Mag_P1_8Rnd", "dz/anims/workspaces/player/player_main/props/player_main_fnp45_mag.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/attachments/magazines/magazine_p1.anm");
		pType.AddItemInHandsProfileIK("Mag_PM73_15Rnd", "dz/anims/workspaces/player/player_main/props/player_main_mp5k_mag.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/magazine_pm73_15rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_PM73_25Rnd", "dz/anims/workspaces/player/player_main/props/player_main_mp5k_mag.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/magazine_pm73_15rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_1911_7Rnd", "dz/anims/workspaces/player/player_main/props/player_main_fnp45_mag.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/magazine_1911.anm");
		pType.AddItemInHandsProfileIK("Mag_762x54Snaploader_2Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/snaploader_762.anm");
		pType.AddItemInHandsProfileIK("Mag_308WinSnaploader_2Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/snaploader_762.anm");
		pType.AddItemInHandsProfileIK("Mag_12gaSnaploader_2Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/attachments/magazines/snaploader.anm");
		pType.AddItemInHandsProfileIK("Mag_357Speedloader_6Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/attachments/magazines/357_speedloader.anm");
		pType.AddItemInHandsProfileIK("Mag_SVD_10Rnd", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/magazine_svd.anm");
		pType.AddItemInHandsProfileIK("Mag_UMP_25Rnd", "dz/anims/workspaces/player/player_main/props/player_main_mp5k_mag.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/magazine_ump45.anm");
		pType.AddItemInHandsProfileIK("Mag_CZ61_20Rnd", "dz/anims/workspaces/player/player_main/props/player_main_mp5k_mag.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/magazine_cz61_20rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_CZ527_5rnd", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/magazine_cz527.anm");
		pType.AddItemInHandsProfileIK("Mag_AKM_30Rnd", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/magazine_akm_30rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_AKM_Palm30Rnd", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", toolsOneHanded, 	"dz/anims/anm/player/ik/attachments/magazines/magazine_akm_30rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_STANAG_30Rnd", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", toolsOneHanded, 	"dz/anims/anm/player/ik/attachments/magazines/magazine_stanag30.anm");
		pType.AddItemInHandsProfileIK("Mag_STANAGCoupled_30Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/attachments/magazines/magazine_stanag30_coupled.anm");
		pType.AddItemInHandsProfileIK("Mag_AK74_30Rnd", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/magazine_akm_30rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_AK101_30Rnd", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", toolsOneHanded, 	"dz/anims/anm/player/ik/attachments/magazines/magazine_akm_30rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_CMAG_10Rnd", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/magazine_cmag_10rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_CMAG_20Rnd", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/magazine_cmag_10rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_CMAG_30Rnd", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/magazine_cmag_10rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_CMAG_40Rnd", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/magazine_cmag_10rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_Ruger1022_10Rnd", "dz/anims/workspaces/player/player_main/props/player_main_mp5k_mag.asi", toolsOneHanded, 	"dz/anims/anm/player/ik/attachments/magazines/magazine_ruger1022_10rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_Ruger1022_15Rnd", "dz/anims/workspaces/player/player_main/props/player_main_mp5k_mag.asi", toolsOneHanded, 	"dz/anims/anm/player/ik/attachments/magazines/magazine_ruger1022_30rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_Ruger1022_30Rnd", "dz/anims/workspaces/player/player_main/props/player_main_mp5k_mag.asi", toolsOneHanded, 	"dz/anims/anm/player/ik/attachments/magazines/magazine_ruger1022_30rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_FAL_20Rnd", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/magazine_fal_20rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_VSS_10Rnd", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/magazine_vss_10rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_Glock_15Rnd", "dz/anims/workspaces/player/player_main/props/player_main_fnp45_mag.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/attachments/magazines/magazine_glock19_15rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_Saiga_5Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 		            "dz/anims/anm/player/ik/attachments/magazines/magazine_saiga_5rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_Saiga_8Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 		            "dz/anims/anm/player/ik/attachments/magazines/magazine_saiga_8rnd.anm");
		pType.AddItemInHandsProfileIK("Mag_Saiga_Drum20Rnd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 		        "dz/anims/anm/player/ik/attachments/magazines/magazine_saiga_drum.anm");
		pType.AddItemInHandsProfileIK("Mag_Deagle_9rnd", "dz/anims/workspaces/player/player_main/props/player_main_fnp45_mag.asi", toolsOneHanded,		"dz/anims/anm/player/ik/attachments/magazines/magazine_fnp45.anm"); //TODO; placeholder

		//! muzzle
		pType.AddItemInHandsProfileIK("AK_Bayonet", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/bayonet_ak.anm");
		pType.AddItemInHandsProfileIK("M9A1_Bayonet", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/combatknife.anm");
		pType.AddItemInHandsProfileIK("Mosin_Bayonet", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/bayonet_mosin.anm");
		pType.AddItemInHandsProfileIK("SKS_Bayonet", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/bayonet_sks.anm");
		pType.AddItemInHandsProfileIK("Mosin_Compensator", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/attachments/muzzle/compensator_mosin.anm");
		pType.AddItemInHandsProfileIK("MP5_Compensator", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/attachments/muzzle/compensator_mp5k.anm");
		pType.AddItemInHandsProfileIK("ImprovisedSuppressor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/attachments/muzzle/suppressor_bottle.anm");
		pType.AddItemInHandsProfileIK("M4_Suppressor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,								"dz/anims/anm/player/ik/attachments/muzzle/suppressor_556.anm");
		pType.AddItemInHandsProfileIK("AK_Suppressor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,								"dz/anims/anm/player/ik/attachments/muzzle/suppressor_545.anm");
		pType.AddItemInHandsProfileIK("PistolSuppressor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/attachments/muzzle/suppressor_45acp.anm");
		pType.AddItemInHandsProfileIK("Groza_Barrel_Short", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured	
		pType.AddItemInHandsProfileIK("Groza_Barrel_Grip", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("Groza_Barrel_Suppressor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
	
		//! optic
		pType.AddItemInHandsProfileIK("M4_CarryHandleOptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/optic/optic_m4_carryhandle.anm");
		pType.AddItemInHandsProfileIK("BUISOptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/optic/optic_buis.anm");
		pType.AddItemInHandsProfileIK("M68Optic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/attachments/optic/optic_m68_cco.anm");
		pType.AddItemInHandsProfileIK("FNP45_MRDSOptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope_kobra.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/attachments/optic/optic_fnp45_mrd.anm");
		pType.AddItemInHandsProfileIK("ACOGOptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", toolsOneHanded,							"dz/anims/anm/player/ik/attachments/optic/optic_acog.anm");
		pType.AddItemInHandsProfileIK("PUScopeOptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/optic/optic_puscope.anm");
		pType.AddItemInHandsProfileIK("LongrangeOptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/optic/optic_longrange.anm");
		pType.AddItemInHandsProfileIK("HuntingOptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/optic/optic_hunting_scope_12x.anm");
		pType.AddItemInHandsProfileIK("PSO1Optic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/optic/optic_pso1.anm");
		pType.AddItemInHandsProfileIK("PSO11Optic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/optic/optic_pso1.anm");
		pType.AddItemInHandsProfileIK("M4_T3NRDSOptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/optic/optic_t3n_rds.anm");
		pType.AddItemInHandsProfileIK("PistolOptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/optic/optic_pistol.anm");
		pType.AddItemInHandsProfileIK("KashtanOptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/optic/optic_kashtan.anm");
		pType.AddItemInHandsProfileIK("Crossbow_RedpointOptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/attachments/optic/optic_red_point.anm");
		pType.AddItemInHandsProfileIK("ReflexOptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/optic/optic_reflex.anm");
		pType.AddItemInHandsProfileIK("GrozaOptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/optic/optic_ots14.anm");
		pType.AddItemInHandsProfileIK("KobraOptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope_kobra.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/optic/optic_kobra.anm");
		pType.AddItemInHandsProfileIK("KazuarOptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/optic/optic_1PN51.anm"); 
			
		//! support		
		pType.AddItemInHandsProfileIK("M4_OEBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/support/buttstock_m4_oe.anm");
		pType.AddItemInHandsProfileIK("M4_MPBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/support/buttstock_m4_mp.anm");
		pType.AddItemInHandsProfileIK("M4_CQBBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/buttstock_m4_cqb.anm");
		pType.AddItemInHandsProfileIK("M4_PlasticHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/handguard_m4_plastic.anm");
		pType.AddItemInHandsProfileIK("M4_RISHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/support/handguard_m4_ris.anm");
		pType.AddItemInHandsProfileIK("M4_MPHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/support/handguard_m4_mp.anm");
		pType.AddItemInHandsProfileIK("AK_WoodBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/buttstock_ak_wood.anm");
		pType.AddItemInHandsProfileIK("AK_FoldingBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/support/buttstock_ak_folding.anm");
		pType.AddItemInHandsProfileIK("AK_PlasticBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/support/buttstock_ak_plastic.anm");
		pType.AddItemInHandsProfileIK("AK_WoodHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/handguard_ak_wood.anm");
		pType.AddItemInHandsProfileIK("AK_RailHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/handguard_ak_rail.anm");
		pType.AddItemInHandsProfileIK("AK_PlasticHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/handguard_ak_plastic.anm");
		pType.AddItemInHandsProfileIK("AK74_WoodBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/buttstock_ak_wood.anm");
		pType.AddItemInHandsProfileIK("AK74_Hndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/support/handguard_ak_wood.anm");
		pType.AddItemInHandsProfileIK("AKS74U_Bttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/buttstock_aks.anm");
		pType.AddItemInHandsProfileIK("MP5k_StockBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/support/buttstock_mp5_stock.anm");
		pType.AddItemInHandsProfileIK("MP5_PlasticHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/support/handguard_mp5k_plastic.anm");
		pType.AddItemInHandsProfileIK("MP5_RailHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/handguard_mp5k_rail.anm");
		pType.AddItemInHandsProfileIK("Red9Bttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/support/buttstock_ak_folding.anm");
		pType.AddItemInHandsProfileIK("Fal_OeBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/buttstock_fal_oe.anm");
		pType.AddItemInHandsProfileIK("Fal_FoldingBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/support/buttstock_fal_folding.anm");
		pType.AddItemInHandsProfileIK("Saiga_Bttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/buttstock_saiga_folding.anm");
		pType.AddItemInHandsProfileIK("M249_Hndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/support/handguard_m249.anm");
		pType.AddItemInHandsProfileIK("M249_RisHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/attachments/support/handguard_m249_ris.anm");
		pType.AddItemInHandsProfileIK("M249_Bipod", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/support/bipod_m249.anm");
		pType.AddItemInHandsProfileIK("AtlasBipod", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/attachments/support/bipod_atlas.anm");
		pType.AddItemInHandsProfileIK("GhillieAtt_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/attachments/support/ghillie_wrap.anm");
		pType.AddItemInHandsProfileIK("Groza_LowerReceiver", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/banana.anm");
		pType.AddItemInHandsProfileIK("GrozaGL_LowerReceiver", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/gear/banana.anm");
	}
	
	//! Explosives
	{
		pType.AddItemInHandsProfileIK("RGD5Grenade", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/explosives/rgd5.anm"); // Not configured
		pType.AddItemInHandsProfileIK("M67Grenade", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/explosives/grenade.anm");
		pType.AddItemInHandsProfileIK("M18SmokeGrenade_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/explosives/smokegrenade.anm");
		pType.AddItemInHandsProfileIK("Grenade_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured	
		pType.AddItemInHandsProfileIK("FlashGrenade", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/explosives/flashbang.anm");
		pType.AddItemInHandsProfileIK("RDG2SmokeGrenade_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,		"dz/anims/anm/player/ik/explosives/rdg2.anm"); // Not configured
	}

	//! Tools
	{
		pType.AddItemInHandsProfileIK("Flashlight", "dz/anims/workspaces/player/player_main/props/player_main_1h_flashlight.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/gear/flashlight.anm");
		pType.AddItemInHandsProfileIK("Pen_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/pen.anm");
		pType.AddItemInHandsProfileIK("CanOpener", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/CanOpener.anm");
		pType.AddItemInHandsProfileIK("Heatpack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/Heatpack.anm");
		pType.AddItemInHandsProfileIK("MessTin", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/mess_tin.anm");
	    pType.AddItemInHandsProfileIK("Pliers", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/pliers.anm");
		pType.AddItemInHandsProfileIK("Lockpick", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/lockpick.anm");
		pType.AddItemInHandsProfileIK("WeaponCleaningKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/cleaning_kit_wood.anm");
		pType.AddItemInHandsProfileIK("SewingKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/sewing_kit.anm");
		pType.AddItemInHandsProfileIK("LeatherSewingKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/leather_sewing_kit.anm");
		pType.AddItemInHandsProfileIK("Handcuffs", "dz/anims/workspaces/player/player_main/props/player_main_1h_handcuffs.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/handcuffs.anm");
		pType.AddItemInHandsProfileIK("HandcuffKeys", "dz/anims/workspaces/player/player_main/props/player_main_1h_keys.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/handcuff_keys.anm");
		pType.AddItemInHandsProfileIK("Cassette", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/cassette.anm");
		pType.AddItemInHandsProfileIK("FieldShovel", "dz/anims/workspaces/player/player_main/props/player_main_1h_handshovel.asi", toolsOneHanded,		"dz/anims/anm/player/ik/gear/CSLA_Field_Shovel.anm");
		pType.AddItemInHandsProfileIK("Crowbar", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/crowbar.anm");
		pType.AddItemInHandsProfileIK("Hammer", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/hammer_ik.anm");
		pType.AddItemInHandsProfileIK("MeatTenderizer", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,		"dz/anims/anm/player/ik/gear/meat_tenderizer.anm");
		pType.AddItemInHandsProfileIK("Wrench", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/wrench.anm");
		pType.AddItemInHandsProfileIK("LugWrench", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/Lug_wrench.anm");
		pType.AddItemInHandsProfileIK("Pipe", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/metal_pipe.anm");
		pType.AddItemInHandsProfileIK("Screwdriver", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/screwdriver.anm");
		pType.AddItemInHandsProfileIK("Sickle", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/sickle.anm");
		pType.AddItemInHandsProfileIK("Hacksaw", "dz/anims/workspaces/player/player_main/props/player_main_1h_hacksaw.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/gear/hacksaw.anm");
		pType.AddItemInHandsProfileIK("KitchenKnife", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/gear/kitchenknife.anm");
		pType.AddItemInHandsProfileIK("HuntingKnife", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/gear/kitchenknife.anm"); 
		pType.AddItemInHandsProfileIK("CombatKnife", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", toolsOneHanded, 		"dz/anims/anm/player/ik/gear/combatknife.anm"); 
		pType.AddItemInHandsProfileIK("SteakKnife", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/gear/steak_knife.anm");
		pType.AddItemInHandsProfileIK("HayHook", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/hay_hook.anm");
		pType.AddItemInHandsProfileIK("StoneKnife", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/gear/stone_knife.anm");
		pType.AddItemInHandsProfileIK("Iceaxe", "dz/anims/workspaces/player/player_main/props/player_main_1h_iceaxe.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/Ice_Axe.anm"); // wrong pose
		pType.AddItemInHandsProfileIK("Hatchet", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/Hatchet.anm"); 
		pType.AddItemInHandsProfileIK("Machete", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/machete.anm"); 
		pType.AddItemInHandsProfileIK("BrassKnuckles_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/brassknuckles.anm");
		pType.AddItemInHandsProfileIK("TelescopicBaton", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,		"dz/anims/anm/player/ik/gear/TelescopicBaton.anm");
		pType.AddItemInHandsProfileIK("PoliceBaton", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/TelescopicBaton.anm");
		pType.AddItemInHandsProfileIK("CattleProd", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/banana.anm"); //Needs Config
		pType.AddItemInHandsProfileIK("StunBaton", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/TelescopicBaton.anm"); //Needs Config
		pType.AddItemInHandsProfileIK("Cleaver", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/kitchenknife.anm"); //Needs Config
		pType.AddItemInHandsProfileIK("FishingRod", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/fishingrod.anm");
		pType.AddItemInHandsProfileIK("ImprovisedFishingRod", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/ImprovisedFishingRod.anm");
		pType.AddItemInHandsProfileIK("ObsoleteFishingRod", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_light.asi", twoHanded, 		"dz/anims/anm/player/ik/gear/ImprovisedFishingRod.anm");
		pType.AddItemInHandsProfileIK("Mace", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/flanged_mace.anm");
		pType.AddItemInHandsProfileIK("TireRepairKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 		 			"dz/anims/anm/player/ik/gear/TireRepairKit.anm");
		pType.AddItemInHandsProfileIK("HandSaw", "dz/anims/workspaces/player/player_main/props/player_main_1h_hacksaw.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/gear/handsaw.anm");
		pType.AddItemInHandsProfileIK("ElectronicRepairKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/electronic_repair_kit.anm");
		pType.AddItemInHandsProfileIK("EpoxyPutty", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				        "dz/anims/anm/player/ik/gear/Epoxy_Putty.anm");
		
		//! Tools two handed
		pType.AddItemInHandsProfileIK("PipeWrench", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_pipewrench.asi", twoHanded,				"dz/anims/anm/player/ik/two_handed/pipewrench.anm");
		pType.AddItemInHandsProfileIK("Shovel", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_shovel.asi", twoHanded, 					"dz/anims/anm/player/ik/two_handed/Shovel.anm");
		pType.AddItemInHandsProfileIK("FarmingHoe", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_farminghoe.asi", twoHanded, 			"dz/anims/anm/player/ik/two_handed/farming_hoe.anm");
		pType.AddItemInHandsProfileIK("SledgeHammer", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_sledgehammer.asi", twoHanded, 		"dz/anims/anm/player/ik/two_handed/sledge_hammer.anm");
		pType.AddItemInHandsProfileIK("Broom", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_fireaxe.asi", twoHanded, 					"dz/anims/anm/player/ik/two_handed/broom.anm");
		pType.AddItemInHandsProfileIK("Paddle", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_fireaxe.asi", twoHanded, 				"dz/anims/anm/player/ik/two_handed/paddle.anm"); 
		pType.AddItemInHandsProfileIK("FirefighterAxe", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_fireaxe.asi", twoHanded, 		"dz/anims/anm/player/ik/two_handed/FirefighterAxe.anm");
		pType.AddItemInHandsProfileIK("WoodAxe", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_woodaxe.asi", twoHanded, 				"dz/anims/anm/player/ik/two_handed/woodaxe.anm");
		pType.AddItemInHandsProfileIK("Pitchfork", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_light.asi", twoHanded,				"dz/anims/anm/player/ik/two_handed/pitchfork.anm");		
		pType.AddItemInHandsProfileIK("Pickaxe", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_pickaxe.asi", twoHanded,				"dz/anims/anm/player/ik/two_handed/pickaxe.anm");
		pType.AddItemInHandsProfileIK("Sword", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_sword.asi", twoHanded,					"dz/anims/anm/player/ik/two_handed/medieval_sword.anm");
		pType.AddItemInHandsProfileIK("BaseballBat", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_bat.asi", twoHanded,				"dz/anims/anm/player/ik/two_handed/baseballbat.anm");	
		pType.AddItemInHandsProfileIK("NailedBaseballBat", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_bat.asi", twoHanded,			"dz/anims/anm/player/ik/two_handed/baseballbat.anm");	
		pType.AddItemInHandsProfileIK("HockeyStick", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_fireaxe.asi", twoHanded,			"dz/anims/anm/player/ik/two_handed/hockey_stick.anm");	
		pType.AddItemInHandsProfileIK("Chainsaw", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,								"dz/anims/anm/player/ik/two_handed/chainsaw.anm"); 
		pType.AddItemInHandsProfileIK("FireExtinguisher", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded, 	"dz/anims/anm/player/ik/two_handed/fire_extinguisher.anm");	
	}
	
	//! Camping
	{
		//! Camping 1handed
		pType.AddItemInHandsProfileIK("BarbedWire", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/barbed_wire.anm");
		pType.AddItemInHandsProfileIK("BatteryCharger", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/battery_charger.anm");
		pType.AddItemInHandsProfileIK("XmasLights", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/christmas_lights.anm");
		pType.AddItemInHandsProfileIK("FenceKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/gear/fence_kit_folded.anm");
		pType.AddItemInHandsProfileIK("CombinationLock", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/camping/combinationlock.anm");
		pType.AddItemInHandsProfileIK("Flag_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/camping/flag.anm");
		
		
		//! Camping 2handed
		pType.AddItemInHandsProfileIK("CableReel", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,								"dz/anims/anm/player/ik/two_handed/cable_reel.anm");
		pType.AddItemInHandsProfileIK("Spotlight", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,								"dz/anims/anm/player/ik/two_handed/spotlight.anm");
		pType.AddItemInHandsProfileIK("MediumTent", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded, 							"dz/anims/anm/player/ik/two_handed/tent_medium_packed.anm");
		pType.AddItemInHandsProfileIK("CamoNet", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,								"dz/anims/anm/player/ik/two_handed/camo_net.anm");
	
		//! Camping heavy	
		
		pType.AddItemInHandsProfileIK("LargeTentBackPack", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,						"dz/anims/anm/player/ik/heavy/tent_large.anm");
		pType.AddItemInHandsProfileIK("Barrel_ColorBase", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,				"dz/anims/anm/player/ik/heavy/55galDrum.anm");
		pType.AddItemInHandsProfileIK("BarrelHoles_ColorBase", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			"dz/anims/anm/player/ik/heavy/55galDrum.anm");
		pType.AddItemInHandsProfileIK("CarTent", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,						"dz/anims/anm/player/ik/heavy/car_tent_packed.anm");
		pType.AddItemInHandsProfileIK("LargeTent", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,						"dz/anims/anm/player/ik/heavy/tent_large.anm");
		pType.AddItemInHandsProfileIK("PartyTent", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,						"dz/anims/anm/player/ik/heavy/tent_party_packed.anm");
		//pType.AddItemInHandsProfileIK("CamoNet", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,						"dz/anims/anm/player/ik/heavy/camo_net.anm");
		//pType.AddItemInHandsProfileIK("CamoNetShelter", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,					"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("PowerGenerator", "dz/anims/workspaces/player/player_main/props/player_main_heavy_generator.asi", heavyItemBehaviour, "dz/anims/anm/player/ik/heavy/generator.anm");
		pType.AddItemInHandsProfileIK("HescoBox", "dz/anims/workspaces/player/player_main/props/player_main_heavy_generator.asi", heavyItemBehaviour, 		"dz/anims/anm/player/ik/heavy/hesco_box.anm");
		pType.AddItemInHandsProfileIK("Fence", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour, 							"dz/anims/anm/player/ik/heavy/fence.anm");
		pType.AddItemInHandsProfileIK("WoodenLog", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour, 						"dz/anims/anm/player/ik/heavy/wooden_log.anm");
		pType.AddItemInHandsProfileIK("MetalPlate", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,				 		"dz/anims/anm/player/ik/heavy/metal_sheet.anm");
		pType.AddItemInHandsProfileIK("SeaChest", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,				 		"dz/anims/anm/player/ik/heavy/sea_chest.anm");
		pType.AddItemInHandsProfileIK("WoodenCrate", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			 		"dz/anims/anm/player/ik/heavy/wooden_crate.anm");



	}	
	
	//! Vehicles
	{
		//! Vehicles 1handed
		pType.AddItemInHandsProfileIK("SparkPlug", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/vehicles/sparkplug.anm");
		pType.AddItemInHandsProfileIK("HeadlightH7_Box", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/vehicles/HeadlightH7_Box.anm");		
		pType.AddItemInHandsProfileIK("HeadlightH7", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/vehicles/HeadlightH7.anm");		
		
		
		//! Vehicles 2handed 
		pType.AddItemInHandsProfileIK("CarBattery", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,		                        "dz/anims/anm/player/ik/two_handed/BatteryCar.anm"); 
		pType.AddItemInHandsProfileIK("TruckBattery", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,							"dz/anims/anm/player/ik/vehicles/BatteryTruck.anm"); 
		pType.AddItemInHandsProfileIK("CarRadiator", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,							"dz/anims/anm/player/ik/vehicles/radiator_car.anm"); 
		
		//! Vehicles Heavy
		pType.AddItemInHandsProfileIK("TruckRadiator", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,							"dz/anims/anm/player/ik/vehicles/radiator_truck.anm"); 
	
		//! Vehicles heavy	
		//! Hatchback	
		pType.AddItemInHandsProfileIK("HatchbackWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,				"dz/anims/anm/player/ik/vehicles/hatchback/hatchback_wheel.anm");
		pType.AddItemInHandsProfileIK("HatchbackWheel_Ruined", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,		"dz/anims/anm/player/ik/vehicles/hatchback/hatchback_wheeldestroyed.anm");
		pType.AddItemInHandsProfileIK("HatchbackDoors_Driver", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,		"dz/anims/anm/player/ik/vehicles/hatchback/hatchback_driverdoor.anm");
		pType.AddItemInHandsProfileIK("HatchbackDoors_CoDriver", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,	"dz/anims/anm/player/ik/vehicles/hatchback/hatchback_codriverdoor.anm");
		pType.AddItemInHandsProfileIK("HatchbackHood", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,				"dz/anims/anm/player/ik/vehicles/hatchback/hatchback_hood.anm");
		pType.AddItemInHandsProfileIK("HatchbackTrunk", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,				"dz/anims/anm/player/ik/vehicles/hatchback/hatchback_trunk.anm");

		//! v3s	
		pType.AddItemInHandsProfileIK("V3SWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,					"dz/anims/anm/player/ik/vehicles/v3s/v3s_wheel.anm");
		pType.AddItemInHandsProfileIK("V3SWheel_Ruined", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			"dz/anims/anm/player/ik/vehicles/v3s/v3s_wheeldestroyed.anm");
		pType.AddItemInHandsProfileIK("V3SWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,					"dz/anims/anm/player/ik/vehicles/v3s/v3s_wheeldouble.anm");
		pType.AddItemInHandsProfileIK("V3SWheel_Ruined", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			"dz/anims/anm/player/ik/vehicles/v3s/v3s_wheeldoubledestroyed.anm");
		pType.AddItemInHandsProfileIK("V3SDoors_Driver", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			"dz/anims/anm/player/ik/vehicles/v3s/v3s_driverdoor.anm");
		pType.AddItemInHandsProfileIK("V3SDoors_CoDriver", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			"dz/anims/anm/player/ik/vehicles/v3s/v3s_codriverdoor.anm");
		pType.AddItemInHandsProfileIK("V3SHood", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,					"dz/anims/anm/player/ik/vehicles/v3s/v3s_hood.anm");

		//! sedan	
		pType.AddItemInHandsProfileIK("CivSedanWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,					"dz/anims/anm/player/ik/vehicles/sedan/sedan_wheel.anm");
		pType.AddItemInHandsProfileIK("CivSedanWheel_Ruined", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			"dz/anims/anm/player/ik/vehicles/sedan/sedan_wheeldestroyed.anm");
		pType.AddItemInHandsProfileIK("CivSedanDoors_Driver", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			"dz/anims/anm/player/ik/vehicles/sedan/sedan_driverdoor.anm");
		pType.AddItemInHandsProfileIK("CivSedanDoors_CoDriver", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			"dz/anims/anm/player/ik/vehicles/sedan/sedan_codriverdoor.anm");
		pType.AddItemInHandsProfileIK("CivSedanDoors_BackLeft", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			"dz/anims/anm/player/ik/vehicles/sedan/sedan_passengerL_door.anm");
		pType.AddItemInHandsProfileIK("CivSedanDoors_BackRight", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,		"dz/anims/anm/player/ik/vehicles/sedan/sedan_passengerR_door.anm");
		pType.AddItemInHandsProfileIK("CivSedanHood", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,					"dz/anims/anm/player/ik/vehicles/sedan/sedan_hood.anm");
		pType.AddItemInHandsProfileIK("CivSedanTrunk", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,					"dz/anims/anm/player/ik/vehicles/sedan/sedan_trunk.anm");



		//! Hatchback_02	
		pType.AddItemInHandsProfileIK("Hatchback_02_Wheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,					"dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_Wheel.anm");
		pType.AddItemInHandsProfileIK("Hatchback_02_Wheel_Ruined", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			"dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_Wheel.anm");
		pType.AddItemInHandsProfileIK("Hatchback_02_Door_1_1", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			"dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_Door_driver.anm");
		pType.AddItemInHandsProfileIK("Hatchback_02_Door_2_1", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			"dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_Door_CoDriver.anm");
		pType.AddItemInHandsProfileIK("Hatchback_02_Door_2_2", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			"dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_Door_PassengerR.anm");
		pType.AddItemInHandsProfileIK("Hatchback_02_Door_1_2", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,		"dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_Door_PassengerL.anm");
		pType.AddItemInHandsProfileIK("Hatchback_02_Hood", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,					"dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_Hood.anm");
		pType.AddItemInHandsProfileIK("Hatchback_02_Trunk", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,					"dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_Trunk.anm");

       //! sedan_02 
		pType.AddItemInHandsProfileIK("Sedan_02_Wheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,					"dz/anims/anm/player/ik/vehicles/sedan_02/sedan_02_wheel.anm");
		pType.AddItemInHandsProfileIK("Sedan_02_Wheel_Ruined", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			"dz/anims/anm/player/ik/vehicles/sedan_02/sedan_02_wheel_ruined.anm");
		pType.AddItemInHandsProfileIK("Sedan_02_Door_1_1", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			"dz/anims/anm/player/ik/vehicles/sedan_02/sedan_02_driver_door.anm");
		pType.AddItemInHandsProfileIK("Sedan_02_Door_2_1", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			"dz/anims/anm/player/ik/vehicles/sedan_02/sedan_02_codriver_door.anm");
		pType.AddItemInHandsProfileIK("Sedan_02_Door_1_2", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,			"dz/anims/anm/player/ik/vehicles/sedan_02/sedan_02_passengerL_door.anm");
		pType.AddItemInHandsProfileIK("Sedan_02_Door_2_2", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,		"dz/anims/anm/player/ik/vehicles/sedan_02/sedan_02_passengerR_door.anm");
		pType.AddItemInHandsProfileIK("Sedan_02_Hood", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,					"dz/anims/anm/player/ik/vehicles/sedan_02/sedan_02_Hood.anm");
		pType.AddItemInHandsProfileIK("Sedan_02_Trunk", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,					"dz/anims/anm/player/ik/vehicles/sedan_02/sedan_02_Trunk.anm");


	}	
	
	//! Consumables	
	{	
		pType.AddItemInHandsProfileIK("Paper", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/gear/paper.anm");
		pType.AddItemInHandsProfileIK("DuctTape", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/gear/duct_tape.anm");
		pType.AddItemInHandsProfileIK("Bandana_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/BandanaFH_ground.anm");
		pType.AddItemInHandsProfileIK("Rag", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 								"dz/anims/anm/player/ik/gear/Rags.anm");
		pType.AddItemInHandsProfileIK("BurlapStrip", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/burlap_strips.anm");
		pType.AddItemInHandsProfileIK("SmallStone", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 		"dz/anims/anm/player/ik/gear/SmallStone.anm");
		pType.AddItemInHandsProfileIK("BatteryD", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/gear/battery_d.anm");
		pType.AddItemInHandsProfileIK("Battery9V", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/9v_battery.anm");
		pType.AddItemInHandsProfileIK("SmallGasCanister", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/GasCannisterSmall.anm");
		pType.AddItemInHandsProfileIK("MediumGasCanister", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,		 			"dz/anims/anm/player/ik/gear/GasCannisterLarge.anm");  
		pType.AddItemInHandsProfileIK("LargeGasCanister", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/GasCannisterLarge.anm");	
		pType.AddItemInHandsProfileIK("Spraycan_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/spraycan.anm");
		pType.AddItemInHandsProfileIK("Matchbox", "dz/anims/workspaces/player/player_main/props/player_main_1h_matchbox.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/gear/Matchbox.anm");
		pType.AddItemInHandsProfileIK("NailBox", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/gear/Nails_box.anm");
		pType.AddItemInHandsProfileIK("Roadflare", "dz/anims/workspaces/player/player_main/props/player_main_1h_torch.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/gear/roadflare.anm");
		pType.AddItemInHandsProfileIK("Chemlight_ColorBase", "dz/anims/workspaces/player/player_main/props/player_main_1h_torch.asi", toolsOneHanded,	"dz/anims/anm/player/ik/gear/chemlight.anm");
		pType.AddItemInHandsProfileIK("Hook", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 				"dz/anims/anm/player/ik/gear/bait_worm_pinned.anm");
		pType.AddItemInHandsProfileIK("BoneHook", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/gear/bone_hook.anm");
		pType.AddItemInHandsProfileIK("Nail", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 								"dz/anims/anm/player/ik/gear/Nails_loose_10.anm");
		pType.AddItemInHandsProfileIK("Bone", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 				"dz/anims/anm/player/ik/gear/pile_of_bones.anm"); // Not configured
		pType.AddItemInHandsProfileIK("Bait", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 								"dz/anims/anm/player/ik/gear/bait_worm_pinned.anm");
		pType.AddItemInHandsProfileIK("BoneBait", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 			"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("Netting", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 			"dz/anims/anm/player/ik/gear/netting.anm");
		pType.AddItemInHandsProfileIK("ButaneCanister", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/ButanCanister.anm");	
		pType.AddItemInHandsProfileIK("Bark_Oak", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/gear/Bark_oak.anm");
		pType.AddItemInHandsProfileIK("Bark_Birch", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/Bark_birch.anm");
		pType.AddItemInHandsProfileIK("RabbitPelt", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 		"dz/anims/anm/player/ik/gear/Pelt_Rabbit.anm");
		pType.AddItemInHandsProfileIK("PigPelt", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 			"dz/anims/anm/player/ik/gear/Pelt_Pig.anm");

			
		//! Consumables two handed	
		pType.AddItemInHandsProfileIK("Stone", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded, 									"dz/anims/anm/player/ik/two_handed/stone.anm");
		pType.AddItemInHandsProfileIK("Firewood", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded,			"dz/anims/anm/player/ik/two_handed/firewood.anm");
		pType.AddItemInHandsProfileIK("Pelt_Base", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 				"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured	
		pType.AddItemInHandsProfileIK("TannedLeather", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded, 							"dz/anims/anm/player/ik/two_handed/tanned_leather.anm");	
		pType.AddItemInHandsProfileIK("WoodenPlank", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,							"dz/anims/anm/player/ik/two_handed/wooden_plank.anm"); 
		pType.AddItemInHandsProfileIK("PileOfWoodenPlanks", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,						"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("GardenLime", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded,		"dz/anims/anm/player/ik/two_handed/gardenlime.anm");
		pType.AddItemInHandsProfileIK("CowPelt", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 				"dz/anims/anm/player/ik/two_handed/Pelt_Cow.anm");
		pType.AddItemInHandsProfileIK("DeerPelt", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 				"dz/anims/anm/player/ik/two_handed/Pelt_Deer.anm");
		//pType.AddItemInHandsProfileIK("RabbitPelt", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 				"dz/anims/anm/player/ik/two_handed/Pelt_Rabbit.anm");//
		pType.AddItemInHandsProfileIK("FoxPelt", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 				"dz/anims/anm/player/ik/two_handed/Pelt_Fox.anm");
		pType.AddItemInHandsProfileIK("WildboarPelt", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 			"dz/anims/anm/player/ik/two_handed/Pelt_Wildboar.anm");
		pType.AddItemInHandsProfileIK("GoatPelt", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 				"dz/anims/anm/player/ik/two_handed/Pelt_Goat.anm");
		pType.AddItemInHandsProfileIK("MouflonPelt", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 			"dz/anims/anm/player/ik/two_handed/Pelt_Mouflon.anm");
		pType.AddItemInHandsProfileIK("BearPelt", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 				"dz/anims/anm/player/ik/two_handed/Pelt_Bear.anm");
		pType.AddItemInHandsProfileIK("WolfPelt", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 				"dz/anims/anm/player/ik/two_handed/Pelt_Wolf.anm");
		pType.AddItemInHandsProfileIK("SheepPelt", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 				"dz/anims/anm/player/ik/two_handed/Pelt_Sheep.anm");


	}	
	
	//! Containers	
	{	
		pType.AddItemInHandsProfileIK("FirstAidKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/first_aid_kit.anm");	
		pType.AddItemInHandsProfileIK("SmallProtectorCase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/protector_case.anm");	
		pType.AddItemInHandsProfileIK("AmmoBox", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/AmmoBox.anm"); 
		pType.AddItemInHandsProfileIK("Bear_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/TeddyBear.anm");
		pType.AddItemInHandsProfileIK("ChildBag_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/child_g.anm");
		pType.AddItemInHandsProfileIK("SmershBag", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/smersh_g.anm");
		pType.AddItemInHandsProfileIK("GhillieBushrag_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/GhillieBushrag.anm");
		pType.AddItemInHandsProfileIK("GhillieSuit_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/GhillieBushrag.anm");
		pType.AddItemInHandsProfileIK("GhillieTop_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/GhillieBushrag.anm");


		//pType.AddItemInHandsProfileIK("WaterproofBag_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/WaterproofBag.anm");

	
		//! Containers - 2handed		
		pType.AddItemInHandsProfileIK("WaterproofBag_ColorBase", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded, 				"dz/anims/anm/player/ik/two_handed/WaterproofBag.anm");	
		pType.AddItemInHandsProfileIK("TaloonBag_ColorBase", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded, 					"dz/anims/anm/player/ik/two_handed/taloon_g.anm");	
		pType.AddItemInHandsProfileIK("TortillaBag", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded, 							"dz/anims/anm/player/ik/two_handed/tortilla_g.anm");	
		pType.AddItemInHandsProfileIK("HuntingBag", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded, 							"dz/anims/anm/player/ik/two_handed/hunting_g.anm");
		pType.AddItemInHandsProfileIK("AssaultBag_ColorBase", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded, 					"dz/anims/anm/player/ik/two_handed/assault_g.anm");
		pType.AddItemInHandsProfileIK("MountainBag_ColorBase", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded, 				"dz/anims/anm/player/ik/two_handed/mountain_g.anm");
		pType.AddItemInHandsProfileIK("CoyoteBag_ColorBase", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded, 					"dz/anims/anm/player/ik/two_handed/coyote_g.anm");
		pType.AddItemInHandsProfileIK("AliceBag_ColorBase", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded, 					"dz/anims/anm/player/ik/two_handed/alice_g.anm");
		pType.AddItemInHandsProfileIK("DryBag_ColorBase", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded, 						"dz/anims/anm/player/ik/two_handed/drybag_g.anm");
		pType.AddItemInHandsProfileIK("CanisterGasoline", "dz/anims/workspaces/player/player_main/props/player_main_2h_jerrycan.asi", twoHanded, 							"dz/anims/anm/player/ik/two_handed/jerrycan.anm");
		pType.AddItemInHandsProfileIK("CourierBag", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded, 							"dz/anims/anm/player/ik/two_handed/courier_bag.anm");
		pType.AddItemInHandsProfileIK("ImprovisedBag", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded, 						"dz/anims/anm/player/ik/two_handed/improvised_courier_bag.anm");
		pType.AddItemInHandsProfileIK("FurCourierBag", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded, 						"dz/anims/anm/player/ik/two_handed/fur_courier_bag.anm");
		pType.AddItemInHandsProfileIK("FurImprovisedBag", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded, 						"dz/anims/anm/player/ik/two_handed/improvised_courier_bag.anm");
		pType.AddItemInHandsProfileIK("LeatherSack_ColorBase", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded, 				"dz/anims/anm/player/ik/two_handed/leather_sack.anm");

		
		//! Containers - Heavy	
		pType.AddItemInHandsProfileIK("TrashCan", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour,					"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
	}	
	
	//! Cooking	
	{	
		pType.AddItemInHandsProfileIK("ClutterCutterFireplace", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured	
		pType.AddItemInHandsProfileIK("BarrelHoles_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("Tripod", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/Cooking_Tripod.anm");		
		pType.AddItemInHandsProfileIK("FryingPan", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/frying_pan.anm");		
		pType.AddItemInHandsProfileIK("PortableGasStove", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/gas_cooker.anm");	
		pType.AddItemInHandsProfileIK("PortableGasLamp", "dz/anims/workspaces/player/player_main/props/player_main_1h_torch.asi", toolsOneHanded,		"dz/anims/anm/player/ik/gear/GasLight.anm");
		pType.AddItemInHandsProfileIK("Candle", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/candle.anm");
		pType.AddItemInHandsProfileIK("PetrolLighter", "dz/anims/workspaces/player/player_main/props/player_main_1h_lighter.asi", toolsOneHanded,		"dz/anims/anm/player/ik/gear/petrol_lighter.anm");	
		pType.AddItemInHandsProfileIK("FireplaceIndoor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/FireplaceIndoor.anm");
			
		//! Cooking two handed	
		pType.AddItemInHandsProfileIK("Pot", "dz/anims/workspaces/player/player_main/props/player_main_2h_pot.asi", twoHanded, 							"dz/anims/anm/player/ik/two_handed/CookingPot.anm");
		pType.AddItemInHandsProfileIK("FireplaceBase", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded,		"dz/anims/anm/player/ik/two_handed/Fireplace.anm"); 
		
	}	
	
	//! Crafting	
	{	
		pType.AddItemInHandsProfileIK("Rope", "dz/anims/workspaces/player/player_main/props/player_main_1h_rope.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/Rope.anm"); // Not configured
		pType.AddItemInHandsProfileIK("ImprovisedRope", "dz/anims/workspaces/player/player_main/props/player_main_1h_rope.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("MetalWire", "dz/anims/workspaces/player/player_main/props/player_main_1h_rope.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/metalwire.anm");
		pType.AddItemInHandsProfileIK("Torch", "dz/anims/workspaces/player/player_main/props/player_main_1h_torch.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/torch.anm");
		pType.AddItemInHandsProfileIK("ChickenFeather", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("GorkaHelmetVisor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/Maska_glass_g.anm");
		pType.AddItemInHandsProfileIK("HandDrillKit", "dz/anims/workspaces/player/player_main/props/player_main_1h_handdrillkit.asi", toolsOneHanded, 	"dz/anims/anm/player/ik/gear/hand_drill_kit.anm");
		pType.AddItemInHandsProfileIK("WoodenStick", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/WoodenStick.anm"); 
		pType.AddItemInHandsProfileIK("LongWoodenStick", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,		"dz/anims/anm/player/ik/gear/LongWoodenStick.anm");
		pType.AddItemInHandsProfileIK("BurlapSack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/bp_burlap_sack.anm");
		pType.AddItemInHandsProfileIK("LongTorch", "dz/anims/workspaces/player/player_main/props/player_main_1h_torch.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/longTorch.anm");

		//! Crafting 2hd	
		pType.AddItemInHandsProfileIK("Spear", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,				 					"dz/anims/anm/player/ik/two_handed/advanced_spear.anm"); 
	
	}	
	
	//! Cultivation	
	{	
		pType.AddItemInHandsProfileIK("TomatoSeedsPack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/zucchini_seeds_pack.anm");
		pType.AddItemInHandsProfileIK("SeedBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/gear/Seeds_Tomato.anm");
		pType.AddItemInHandsProfileIK("PepperSeedsPack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/zucchini_seeds_pack.anm");
		pType.AddItemInHandsProfileIK("PumpkinSeedsPack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/zucchini_seeds_pack.anm");
		pType.AddItemInHandsProfileIK("ZucchiniSeedsPack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/zucchini_seeds_pack.anm");
		pType.AddItemInHandsProfileIK("CannabisSeedsPack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/zucchini_seeds_pack.anm");
		pType.AddItemInHandsProfileIK("AntiPestsSpray", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/DesinfectantSpray.anm"); 
		pType.AddItemInHandsProfileIK("ParticleTest", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 		"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("PlantMaterial", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 		"dz/anims/anm/player/ik/gear/plant_material.anm"); 

			
		//! Cultivation two handed	
	
	}	
		
	//! Drinks	
	{	
		pType.AddItemInHandsProfileIK("Bottle_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/water_bottle.anm");
		pType.AddItemInHandsProfileIK("Canteen", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/canteen.anm");	
		pType.AddItemInHandsProfileIK("WaterBottle", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/water_bottle.anm");	
		pType.AddItemInHandsProfileIK("WaterPouch_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/waterpouch.anm");	
		pType.AddItemInHandsProfileIK("SodaCan_ColorBase", "dz/anims/workspaces/player/player_main/props/player_main_1h_sodacan.asi", toolsOneHanded,	"dz/anims/anm/player/ik/gear/soda_can.anm");	
	}	
		
	//! Food	
	{	
		//! Food animal corpses	
		pType.AddItemInHandsProfileIK("Carp", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/carp_live.anm");
		pType.AddItemInHandsProfileIK("Sardines", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/sardines_live.anm");
		pType.AddItemInHandsProfileIK("Mackerel", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/mackerel_fillet.anm");	
		pType.AddItemInHandsProfileIK("Worm", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,								"dz/anims/anm/player/ik/gear/bait_worm.anm");
		
		//! Food cans	
		pType.AddItemInHandsProfileIK("BakedBeansCan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/food_can.anm");
		pType.AddItemInHandsProfileIK("BakedBeansCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/food_can_opened.anm");
		pType.AddItemInHandsProfileIK("PeachesCan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/food_can.anm");
		pType.AddItemInHandsProfileIK("PeachesCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/food_can_opened.anm");
		pType.AddItemInHandsProfileIK("TacticalBaconCan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/food_can.anm");
		pType.AddItemInHandsProfileIK("TacticalBaconCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/food_can_opened.anm");
		pType.AddItemInHandsProfileIK("SpaghettiCan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/food_can.anm");
		pType.AddItemInHandsProfileIK("SpaghettiCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/food_can_opened.anm");
		pType.AddItemInHandsProfileIK("SardinesCan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/sardines.anm");
		pType.AddItemInHandsProfileIK("SardinesCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/sardines.anm");
		pType.AddItemInHandsProfileIK("TunaCan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 							"dz/anims/anm/player/ik/gear/tuna.anm");
		pType.AddItemInHandsProfileIK("TunaCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/tuna.anm");
			
		//! Food fruits		
		pType.AddItemInHandsProfileIK("Apple", "dz/anims/workspaces/player/player_main/props/player_main_1h_fruit.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/apple.anm");
		pType.AddItemInHandsProfileIK("Banana", "dz/anims/workspaces/player/player_main/props/player_main_1h_fruit.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("Orange", "dz/anims/workspaces/player/player_main/props/player_main_1h_fruit.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/orange.anm");	
		pType.AddItemInHandsProfileIK("Tomato", "dz/anims/workspaces/player/player_main/props/player_main_1h_fruit.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/tomato_fresh.anm");
		pType.AddItemInHandsProfileIK("GreenBellPepper", "dz/anims/workspaces/player/player_main/props/player_main_1h_fruit.asi", toolsOneHanded,		"dz/anims/anm/player/ik/gear/pepper_green_fresh.anm");
		pType.AddItemInHandsProfileIK("Zucchini", "dz/anims/workspaces/player/player_main/props/player_main_1h_fruit.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/zucchini_fresh.anm");
		pType.AddItemInHandsProfileIK("Pumpkin", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,								"dz/anims/anm/player/ik/two_handed/pumpkin_fresh.anm");
		pType.AddItemInHandsProfileIK("SlicedPumpkin", "dz/anims/workspaces/player/player_main/props/player_main_1h_fruit.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/pumpkin_sliced.anm"); 
		pType.AddItemInHandsProfileIK("Potato", "dz/anims/workspaces/player/player_main/props/player_main_1h_fruit.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/potato_fresh.anm");
		pType.AddItemInHandsProfileIK("Kiwi", "dz/anims/workspaces/player/player_main/props/player_main_1h_fruit.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/kiwi.anm");
		pType.AddItemInHandsProfileIK("SambucusBerry", "dz/anims/workspaces/player/player_main/props/player_main_1h_fruit.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/berries.anm");
		pType.AddItemInHandsProfileIK("CaninaBerry", "dz/anims/workspaces/player/player_main/props/player_main_1h_fruit.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/poisonousberries.anm");
		pType.AddItemInHandsProfileIK("Cannabis", "dz/anims/workspaces/player/player_main/props/player_main_1h_fruit.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/berries.anm");
		pType.AddItemInHandsProfileIK("Plum", "dz/anims/workspaces/player/player_main/props/player_main_1h_fruit.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/plum.anm");
		pType.AddItemInHandsProfileIK("Pear", "dz/anims/workspaces/player/player_main/props/player_main_1h_fruit.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/pear.anm");  
				
		//! Food meats			
		pType.AddItemInHandsProfileIK("HumanSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/meat_human.anm");
		pType.AddItemInHandsProfileIK("GoatSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/meat_steak.anm");
		pType.AddItemInHandsProfileIK("MouflonSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/meat_steak.anm");	
		pType.AddItemInHandsProfileIK("BoarSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/meat_steak.anm");
		pType.AddItemInHandsProfileIK("PigSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/meat_steak.anm");
		pType.AddItemInHandsProfileIK("DeerSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/meat_steak.anm");
		pType.AddItemInHandsProfileIK("WolfSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/meat_steak.anm");
		pType.AddItemInHandsProfileIK("BearSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/meat_steak.anm");	
		pType.AddItemInHandsProfileIK("CowSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/meat_steak.anm");
		pType.AddItemInHandsProfileIK("SheepSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/meat_steak.anm");
		pType.AddItemInHandsProfileIK("FoxSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/meat_steak.anm");
		pType.AddItemInHandsProfileIK("ChickenBreastMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/meat_breast.anm");
		pType.AddItemInHandsProfileIK("RabbitLegMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/meat_leg.anm");
		pType.AddItemInHandsProfileIK("CarpFilletMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/carp_fillet_raw.anm");
		pType.AddItemInHandsProfileIK("MackerelFilletMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/mackerel_fillet.anm"); 
		pType.AddItemInHandsProfileIK("Lard", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,								"dz/anims/anm/player/ik/gear/lard.anm");
		
		//! Food mushrooms	
		pType.AddItemInHandsProfileIK("AgaricusMushroom", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/mushroom_agaricus.anm"); // Not configured
		pType.AddItemInHandsProfileIK("AuriculariaMushroom", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/mushroom_auricularia.anm"); // Not configured
		pType.AddItemInHandsProfileIK("AmanitaMushroom", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/mushroom_amanita.anm"); // Not configured
		pType.AddItemInHandsProfileIK("MacrolepiotaMushroom", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/mushroom_macrolepiota.anm"); // Not configured
		pType.AddItemInHandsProfileIK("LactariusMushroom", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/mushroom_lactarius.anm"); // Not configured
		pType.AddItemInHandsProfileIK("PsilocybeMushroom", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/mushroom_psilocybe.anm");
		pType.AddItemInHandsProfileIK("BoletusMushroom", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/Mushroom_boletus.anm");		
		//! Food basic	
		pType.AddItemInHandsProfileIK("Edible_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 		"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("BoxCerealCrunchin", "dz/anims/workspaces/player/player_main/props/player_main_1h_food_box.asi", toolsOneHanded,	"dz/anims/anm/player/ik/gear/box_cereal.anm");
		pType.AddItemInHandsProfileIK("PowderedMilk", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/powdered_milk.anm");
		pType.AddItemInHandsProfileIK("SmallGuts", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/guts_animal_small.anm"); 
		pType.AddItemInHandsProfileIK("Guts", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,								"dz/anims/anm/player/ik/gear/guts_animal.anm");
		pType.AddItemInHandsProfileIK("Rice", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,								"dz/anims/anm/player/ik/gear/rice.anm");
		pType.AddItemInHandsProfileIK("Marmalade", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/marmalade.anm");
	}
	
	//! Medical 
	{
		pType.AddItemInHandsProfileIK("Bottle_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("BandageDressing", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/bandage_dressing.anm");
		pType.AddItemInHandsProfileIK("DisinfectantSpray", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/DesinfectantSpray.anm");
		pType.AddItemInHandsProfileIK("DisinfectantAlcohol", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/disinfectant_alcohol.anm");
		pType.AddItemInHandsProfileIK("PurificationTablets", "dz/anims/workspaces/player/player_main/props/player_main_1h_tablets.asi", toolsOneHanded,	"dz/anims/anm/player/ik/gear/Water_purification_tablets.anm");
		pType.AddItemInHandsProfileIK("CharcoalTablets", "dz/anims/workspaces/player/player_main/props/player_main_1h_tablets.asi", toolsOneHanded,		"dz/anims/anm/player/ik/gear/charcoal_tablets.anm");
		pType.AddItemInHandsProfileIK("PainkillerTablets", "dz/anims/workspaces/player/player_main/props/player_main_1h_tablets.asi", toolsOneHanded,	"dz/anims/anm/player/ik/gear/painkillers2.anm");
		pType.AddItemInHandsProfileIK("VitaminBottle", "dz/anims/workspaces/player/player_main/props/player_main_1h_pills.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/VitaminBottle.anm");
		pType.AddItemInHandsProfileIK("TetracyclineAntibiotics", "dz/anims/workspaces/player/player_main/props/player_main_1h_tablets.asi", toolsOneHanded,	"dz/anims/anm/player/ik/gear/tetracycline_tablets.anm");
		pType.AddItemInHandsProfileIK("Epinephrine", "dz/anims/workspaces/player/player_main/props/player_main_1h_morphine.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/morphine.anm");
		pType.AddItemInHandsProfileIK("Morphine", "dz/anims/workspaces/player/player_main/props/player_main_1h_morphine.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/morphine.anm");
		pType.AddItemInHandsProfileIK("Syringe", "dz/anims/workspaces/player/player_main/props/player_main_1h_syringe.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/Syringe_Empty.anm");
		pType.AddItemInHandsProfileIK("ClearSyringe", "dz/anims/workspaces/player/player_main/props/player_main_1h_syringe.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/Syringe_Full.anm");
		pType.AddItemInHandsProfileIK("BloodSyringe", "dz/anims/workspaces/player/player_main/props/player_main_1h_syringe.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/Syringe_Full.anm");
		pType.AddItemInHandsProfileIK("InjectionVial", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/InjectionVial.anm");
		pType.AddItemInHandsProfileIK("SalineBag", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/SalineBag.anm");
		pType.AddItemInHandsProfileIK("StartKitIV", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/StartKit.anm");
		pType.AddItemInHandsProfileIK("SalineBagIV", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/TransfusionKit.anm");
		pType.AddItemInHandsProfileIK("BloodBagEmpty", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/BloodBag_Sealed.anm");
		pType.AddItemInHandsProfileIK("BloodBagFull", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/BloodBag_Full.anm");
		pType.AddItemInHandsProfileIK("BloodBagIV", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/BloodBag_Full_IV.anm");
		pType.AddItemInHandsProfileIK("BloodTestKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/BloodTest_Kit.anm");
		pType.AddItemInHandsProfileIK("Splint", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/improvised_fixture.anm");
		pType.AddItemInHandsProfileIK("Thermometer", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/thermometer.anm");
		pType.AddItemInHandsProfileIK("CigarettePack_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		
		//! Medical two handed
		pType.AddItemInHandsProfileIK("Defibrillator", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded,		"dz/anims/anm/player/ik/two_handed/defibrillator.anm");
	}

	//! Navigation
	{
		pType.AddItemInHandsProfileIK("ChernarusMap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/Map_chernarus.anm");
		pType.AddItemInHandsProfileIK("Compass", "dz/anims/workspaces/player/player_main/props/player_main_1h_compass.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/gear/compass.anm");
		pType.AddItemInHandsProfileIK("OrienteeringCompass", "dz/anims/workspaces/player/player_main/props/player_main_1h_compass.asi", toolsOneHanded,	"dz/anims/anm/player/ik/gear/compass_modern.anm");
	}
	
	//! Optic
	{
		pType.AddItemInHandsProfileIK("Binoculars", "dz/anims/workspaces/player/player_main/props/player_main_1h_binocular.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/binoculars.anm");
		pType.AddItemInHandsProfileIK("Rangefinder", "dz/anims/workspaces/player/player_main/props/player_main_1h_rangefinder.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/rangefinder.anm");
		pType.AddItemInHandsProfileIK("NVGoggles", "dz/anims/workspaces/player/player_main/props/player_main_1h_nightvision.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/NVGoggle.anm");

	}

	//! Radio
	{
		pType.AddItemInHandsProfileIK("Transmitter_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/TransfusionKit.anm");
		pType.AddItemInHandsProfileIK("StaticRadio", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				 		"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("Megaphone", "dz/anims/workspaces/player/player_main/props/player_main_1h_megaphone.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/megaphone.anm");
		pType.AddItemInHandsProfileIK("PersonalRadio", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/PersonalRadio.anm");
		pType.AddItemInHandsProfileIK("Radio", "dz/anims/workspaces/player/player_main/props/player_main_1h_radio.asi", toolsOneHanded,				 	"dz/anims/anm/player/ik/gear/Radio.anm");  
       
	   //! Radio heavy
		pType.AddItemInHandsProfileIK("BaseRadio", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded,			"dz/anims/anm/player/ik/two_handed/BaseRadio.anm"); 

	}

	//! Traps
	{
		pType.AddItemInHandsProfileIK("LandMineTrap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/landmine.anm");
		pType.AddItemInHandsProfileIK("SmallFishTrap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/improvised_fish_trap.anm");
		pType.AddItemInHandsProfileIK("FishNetTrap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/banana.anm"); // Not configured
		pType.AddItemInHandsProfileIK("TripwireTrap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/tripwire.anm");
		pType.AddItemInHandsProfileIK("RabbitSnareTrap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/rabbit_snare_folded.anm");
    
		//! Traps two handed
		pType.AddItemInHandsProfileIK("BearTrap", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,								"dz/anims/anm/player/ik/two_handed/beartrap_g.anm");
	}
	
	//! Clothes
	    pType.AddItemInHandsProfileIK("Skirt_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/clothing/Skirt.anm");
	
	//! Headgear
	{
		pType.AddItemInHandsProfileIK("GorkaHelmet", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/gorka_helmet.anm");
		pType.AddItemInHandsProfileIK("GorkaHelmetComplete", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/gorka_helmet_complete.anm");
		pType.AddItemInHandsProfileIK("FirefightersHelmet_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,		"dz/anims/anm/player/ik/gear/firehelmet.anm");
		pType.AddItemInHandsProfileIK("BallisticHelmet_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/ballistic_helmet.anm");
		pType.AddItemInHandsProfileIK("ZSh3PilotHelmet", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/ZSh3_helmet.anm");
		pType.AddItemInHandsProfileIK("ConstructionHelmet_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,		"dz/anims/anm/player/ik/gear/construction_helmet.anm");
		pType.AddItemInHandsProfileIK("BurlapSackCover", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/burlap_cover.anm");
		pType.AddItemInHandsProfileIK("Ushanka_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/ushanka_hat.anm");
		pType.AddItemInHandsProfileIK("MotoHelmet_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/moto_helmet.anm");
		pType.AddItemInHandsProfileIK("MilitaryBeret_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/military_beret.anm");
		pType.AddItemInHandsProfileIK("CowboyHat_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/cowboy_hat.anm");
		pType.AddItemInHandsProfileIK("BoonieHat_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/boonie_hat.anm");
		pType.AddItemInHandsProfileIK("LeatherHat_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/leatherhat.anm");
		pType.AddItemInHandsProfileIK("PilotkaCap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/pilotka_cap.anm");
		pType.AddItemInHandsProfileIK("TankerHelmet", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/tanker_helmet.anm");
		pType.AddItemInHandsProfileIK("SkateHelmet_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/skate_helmet.anm");
		pType.AddItemInHandsProfileIK("PrisonerCap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/prisoner_cap.anm");
		pType.AddItemInHandsProfileIK("GreatHelm", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/greathelm.anm");
		pType.AddItemInHandsProfileIK("Ssh68Helmet", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/ssh68_helmet.anm");
		pType.AddItemInHandsProfileIK("DirtBikeHelmet_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/dirtbike_helmet.anm");
		pType.AddItemInHandsProfileIK("DirtBikeHelmet_Visor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/dirtbike_visor.anm");
		pType.AddItemInHandsProfileIK("DirtBikeHelmet_Mouthguard", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/dirtbike_mouthguard.anm");
		pType.AddItemInHandsProfileIK("NioshFaceMask", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/nioshface_mask.anm");
		pType.AddItemInHandsProfileIK("DallasMask", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/dallas_mask.anm");
		pType.AddItemInHandsProfileIK("HoxtonMask", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/dallas_mask.anm");
		pType.AddItemInHandsProfileIK("WolfMask", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/dallas_mask.anm");
		pType.AddItemInHandsProfileIK("AviatorGlasses", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/aviators_glasses.anm");
		pType.AddItemInHandsProfileIK("ThinFramesGlasses", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/aviators_glasses.anm");
		pType.AddItemInHandsProfileIK("ThickFramesGlasses", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/aviators_glasses.anm");
		pType.AddItemInHandsProfileIK("SportGlasses", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/aviators_glasses.anm");
		pType.AddItemInHandsProfileIK("DesignerGlasses", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/aviators_glasses.anm");	
		pType.AddItemInHandsProfileIK("BalaclavaMask_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/balaclava_mask.anm");
		pType.AddItemInHandsProfileIK("BandanaMask_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/bandana_mask.anm");
		pType.AddItemInHandsProfileIK("BaseballCap_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/baseball_cap.anm");
		pType.AddItemInHandsProfileIK("BeanieHat_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/beanie_hat.anm");
		pType.AddItemInHandsProfileIK("FlatCap_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,					"dz/anims/anm/player/ik/gear/flat_cap.anm");
		pType.AddItemInHandsProfileIK("MedicalScrubsHat_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,		"dz/anims/anm/player/ik/gear/medicalScrubs_hat.anm");
		pType.AddItemInHandsProfileIK("PoliceCap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/police_cap.anm");
		pType.AddItemInHandsProfileIK("RadarCap_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/radar_cap.anm");
		pType.AddItemInHandsProfileIK("ZmijovkaCap_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/zmijovka_cap.anm");
		pType.AddItemInHandsProfileIK("GasMask", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,							"dz/anims/anm/player/ik/gear/gas_mask.anm");
		pType.AddItemInHandsProfileIK("PumpkinHelmet", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded,							"dz/anims/anm/player/ik/two_handed/PumpkinHelmet.anm");
		pType.AddItemInHandsProfileIK("NVGHeadstrap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,						"dz/anims/anm/player/ik/gear/NVG_Headstrap.anm");
		pType.AddItemInHandsProfileIK("Headtorch_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,				"dz/anims/anm/player/ik/gear/head_torch.anm");
		pType.AddItemInHandsProfileIK("Balaclava3Holes_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded,			"dz/anims/anm/player/ik/gear/balaclava_3holes.anm");

		
	}

	//! Belts
	{
		pType.AddItemInHandsProfileIK("CivilianBelt", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/clothing/belts/civ_belt_g.anm");
		pType.AddItemInHandsProfileIK("MilitaryBelt", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/clothing/belts/mil_belt_g.anm");
		pType.AddItemInHandsProfileIK("NylonKnifeSheath", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/clothing/belts/nylon_KnifeSheath.anm");
		pType.AddItemInHandsProfileIK("LeatherKnifeSheath", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/clothing/belts/leather_KnifeSheath.anm");
		pType.AddItemInHandsProfileIK("LeatherBelt_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 			"dz/anims/anm/player/ik/clothing/belts/leather_belt.anm");

	}
	
	//! Gloves
	{
		pType.AddItemInHandsProfileIK("WorkingGloves_Black", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/WorkingGloves.anm");
		pType.AddItemInHandsProfileIK("WorkingGloves_Beige", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/WorkingGloves.anm");
		pType.AddItemInHandsProfileIK("WorkingGloves_Brown", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/WorkingGloves.anm");
		pType.AddItemInHandsProfileIK("WorkingGloves_Yellow", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/WorkingGloves.anm");
		pType.AddItemInHandsProfileIK("TacticalGloves_Black", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/TacticalGloves.anm");
		pType.AddItemInHandsProfileIK("TacticalGloves_Beige", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/TacticalGloves.anm");
		pType.AddItemInHandsProfileIK("TacticalGloves_Green", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/TacticalGloves.anm");
		pType.AddItemInHandsProfileIK("NBCGlovesGray", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					    "dz/anims/anm/player/ik/gear/NBCGloves.anm");
		pType.AddItemInHandsProfileIK("SurgicalGloves_Blue", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/SurgicalGloves.anm");
		pType.AddItemInHandsProfileIK("SurgicalGloves_LightBlue", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 			    "dz/anims/anm/player/ik/gear/SurgicalGloves.anm");
		pType.AddItemInHandsProfileIK("SurgicalGloves_Green", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/SurgicalGloves.anm");
		pType.AddItemInHandsProfileIK("SurgicalGloves_White", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/SurgicalGloves.anm");
		pType.AddItemInHandsProfileIK("LeatherGloves_Natural", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				"dz/anims/anm/player/ik/gear/LeatherGloves.anm");
		pType.AddItemInHandsProfileIK("LeatherGloves_Beige", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				    "dz/anims/anm/player/ik/gear/LeatherGloves.anm");
		pType.AddItemInHandsProfileIK("LeatherGloves_Black", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				    "dz/anims/anm/player/ik/gear/LeatherGloves.anm");
		pType.AddItemInHandsProfileIK("LeatherGloves_Brown", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				    "dz/anims/anm/player/ik/gear/LeatherGloves.anm");
		pType.AddItemInHandsProfileIK("OMNOGloves_Gray", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/CombatGloves.anm");
		pType.AddItemInHandsProfileIK("OMNOGloves_Brown", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/CombatGloves.anm");

	}
	
	//! Shoe
	{
		
		
		
	}
	
	//! Vest
	{
		pType.AddItemInHandsProfileIK("BallisticVest", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded,					"dz/anims/anm/player/ik/two_handed/vest_ballistic.anm");
		pType.AddItemInHandsProfileIK("HighCapacityVest_ColorBase", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded,	"dz/anims/anm/player/ik/two_handed/vest_highcapacity.anm");
		pType.AddItemInHandsProfileIK("PoliceVest", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded,					"dz/anims/anm/player/ik/two_handed/vest_police.anm");
		pType.AddItemInHandsProfileIK("PressVest_ColorBase", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded,			"dz/anims/anm/player/ik/two_handed/vest_press.anm");
		pType.AddItemInHandsProfileIK("UKAssVest_ColorBase", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", twoHanded,			"dz/anims/anm/player/ik/two_handed/vest_ukassault.anm");
        pType.AddItemInHandsProfileIK("HuntingVest", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						            "dz/anims/anm/player/ik/clothing/HuntingVest.anm");
        pType.AddItemInHandsProfileIK("ReflexVest", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						            "dz/anims/anm/player/ik/clothing/ReflexVest.anm");

	}
	
	//! Clothing attachments (holsters, plates)
	{
		pType.AddItemInHandsProfileIK("PlateCarrierPouches", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 					"dz/anims/anm/player/ik/gear/plate_carrier_pouches_g.anm");
		pType.AddItemInHandsProfileIK("CarrierHolsterSolo", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				    "dz/anims/anm/player/ik/gear/plate_carrier_holster.anm");
		pType.AddItemInHandsProfileIK("PlateCarrierHolster", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 				    "dz/anims/anm/player/ik/gear/plate_carrier_holster.anm");
	}
	
	//! Others
	{
		pType.AddItemInHandsProfileIK("Book_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded, 						"dz/anims/anm/player/ik/gear/book.anm");
	}

	
	//! Restrained items
	{
		pType.AddItemInHandsProfileIK("HandcuffsLocked", "dz/anims/workspaces/player/player_main/player_main_1h_restrained.asi", restrained, 						"dz/anims/anm/player/ik/gear/handcuffslocked.anm");
		pType.AddItemInHandsProfileIK("RopeLocked", "dz/anims/workspaces/player/player_main/player_main_1h_restrained.asi", restrained, 							"dz/anims/anm/player/ik/gear/bp_rope_tied.anm");
		pType.AddItemInHandsProfileIK("DuctTapeLocked", "dz/anims/workspaces/player/player_main/player_main_1h_restrained.asi", restrained, 						"dz/anims/anm/player/ik/gear/DuctTape_tied.anm");
		pType.AddItemInHandsProfileIK("MetalWireLocked", "dz/anims/workspaces/player/player_main/player_main_1h_restrained.asi", restrained, 						"dz/anims/anm/player/ik/gear/Metalwire_tied.anm");
		pType.AddItemInHandsProfileIK("BarbedWireLocked", "dz/anims/workspaces/player/player_main/player_main_1h_restrained.asi", restrained, 						"dz/anims/anm/player/ik/gear/Barbed_wire_tied.anm");
	}
	
	//! Surrender items (dummy!)
	{
		pType.AddItemInHandsProfileIK("SurrenderDummyItem", "dz/anims/workspaces/player/player_main/player_main_surrender.asi", surrender, 						"");
	}

	
	//! custom one handed
    modCallbacks.RegisterOneHanded(pType, toolsOneHanded);

	//! custom two handed
	modCallbacks.RegisterTwoHanded(pType, twoHanded);

	//! custom restrained
    modCallbacks.RegisterRestrained(pType, restrained);

	//! custom surrender - really needed?
    modCallbacks.RegisterSurrender(pType, surrender);

	//! custom heavy
    modCallbacks.RegisterHeavy(pType, heavyItemBehaviour);

	//! custom behaviours
	modCallbacks.RegisterCustom(pType);

	
	//!===========================================================
	// bone Remaps

	{
		// mapping "weaponBone" -> "humanBone" 

		array<string> rifleBoneRemap = { "bolt", "Weapon_Bolt", "magazine", "Weapon_Magazine", "trigger", "Weapon_Trigger", "charging", "Weapon_Bone_01", "bullet", "Weapon_Bullet", "mag_release", "Weapon_Bone_02", "boltrelease", "Weapon_Bone_03"};
		pType.AddItemBoneRemap("Rifle_Base", rifleBoneRemap);

		array<string> pistolBoneRemap ={ "bolt", "Weapon_Bolt", "magazine", "Weapon_Magazine", "bullet","Weapon_Bullet", "trigger", "Weapon_Trigger", "hammer", "Weapon_Bone_01", };
		pType.AddItemBoneRemap("Pistol_Base", pistolBoneRemap);
		
		array<string> izh18BoneRemap = { "release", "Weapon_Bone_02", "barrel", "Weapon_Bone_01", "trigger", "Weapon_Trigger","ejector","Weapon_Bone_03","bullet", "Weapon_Bullet", };
		pType.AddItemBoneRemap("Izh18_Base", izh18BoneRemap);
		
		array<string> BoltActionRifleBoneRemap ={ "bolthandle", "Weapon_Bolt", "boltbody", "Weapon_Bone_04", "boltbodyS", "Weapon_Bone_05", "trigger", "Weapon_Trigger","bullet", "Weapon_Bullet", "magazine", "Weapon_Magazine"};
		pType.AddItemBoneRemap("BoltActionRifle_Base", BoltActionRifleBoneRemap);
		
		array<string> ShotgunBoneRemap ={ "bolt", "Weapon_Bolt", "pump", "Weapon_Bone_01","feedRamp", "Weapon_Bone_02", "trigger", "Weapon_Trigger","bullet", "Weapon_Bullet", "boltrelease", "Weapon_Bone_03"};
		pType.AddItemBoneRemap("Mp133Shotgun_Base", ShotgunBoneRemap);
		
		array<string> Izh43BoneRemap = { "release", "Weapon_Bone_02", "barrel", "Weapon_Bone_01", "trigger", "Weapon_Trigger","ejector","Weapon_Bone_03","bullet", "Weapon_Bullet", "bullet2", "Weapon_Bone_04", "snaploader", "Weapon_Bone_05"};
		pType.AddItemBoneRemap("Izh43Shotgun_Base", Izh43BoneRemap);
		
		array<string> B95BoneRemap = { "release", "Weapon_Bone_02", "barrel", "Weapon_Bone_01", "trigger", "Weapon_Trigger","ejector","Weapon_Bone_03","bullet", "Weapon_Bullet", "bullet2", "Weapon_Bone_04", "snaploader", "Weapon_Bone_05"};
		pType.AddItemBoneRemap("B95_Base", B95BoneRemap);
		
		array<string> SKSBoneRemap = { "bolt", "Weapon_Bolt", "trigger", "Weapon_Trigger", "bullet", "Weapon_Bullet", "follower", "Weapon_Bone_02", "clip", "Weapon_Magazine", "cliprounds", "Weapon_Bone_01"};
		pType.AddItemBoneRemap("SKS_Base", SKSBoneRemap);
		
		array<string> MagnumBoneRemap ={ "bolt", "Weapon_Bolt", "magazine", "Weapon_Magazine", "bullet","Weapon_Bullet", "trigger", "Weapon_Trigger", "hammer", "Weapon_Bone_01","cylinder_arm", "Weapon_Bone_03" ,"ejector", "Weapon_Bone_04" };
		pType.AddItemBoneRemap("Magnum_Base", MagnumBoneRemap);
		
		//array<string> M4A1BoneRemap ={ "bolt", "Weapon_Bolt", "magazine", "Weapon_Magazine", "trigger", "Weapon_Trigger","charginghandle", "Weapon_Bone_01","bullet","Weapon_Bullet", };
		//pType.AddItemBoneRemap("M4A1_Base", M4A1BoneRemap);
		
		// mod remapping
		modCallbacks.CustomBoneRemapping(pType);
	}

	//!===========================================================
	// bone Remaps

	//
	pType.DebugPreloadAnimationInstances();
	
	GetGame().ProfilerStop("DayZPlayerTypeRegisterItems");

}

