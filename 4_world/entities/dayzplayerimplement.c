/*
DayZPlayerImplement

this file is implemenation of dayzPlayer in script
- logic of movement
- camera switching logic

*/

class DayZPlayerCommandDeathCallback : HumanCommandDeathCallback
{
	//----------------------------------
	// callbacks

	override void 	OnSimulationEnd()
	{
		syncDebugPrint("DZPI::OnSimulationEnd - trying to drop item");
		
		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			EntityAI itemInHands = m_pPlayer.GetHumanInventory().GetEntityInHands();
			if (itemInHands)
			{
				int boneIndex = m_pPlayer.GetBoneIndexByName("RightHand_Dummy");
			
				vector m4[4];
				m_pPlayer.GetBoneTransformWS(boneIndex, m4);
			
				m_pPlayer.GetInventory().DropEntityWithTransform(InventoryMode.SERVER, m_pPlayer, itemInHands, m4);
				
				if ( GetCEApi() )
				{
					int deadBodyLifetime = GetCEApi().GetCEGlobalInt( "CleanupLifetimeDeadPlayer" );
					if ( deadBodyLifetime <= 0 )
					{
						deadBodyLifetime = 3600;
					}
					itemInHands.SetLifetime( deadBodyLifetime );
				}
			}	
		}
		
		/*if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			EntityAI itemInHands = m_pPlayer.GetHumanInventory().GetEntityInHands();
			if (itemInHands)
			{
				if (m_pPlayer.CanDropEntity(itemInHands))
				{
					string item_name = itemInHands.GetType();
					m_pPlayer.ServerReplaceItemInHandsWithNewElsewhere(new DestroyItemInCorpsesHandsAndCreateNewOnGndLambda(itemInHands, item_name, m_pPlayer, false));
				}
			}
		}*/
	}
	
	PlayerBase m_pPlayer;
}

class DayZPlayerImplement extends DayZPlayer
{
	// Timer 	m_UpdateTick;

	ref HumanMovementState 					m_MovementState = new HumanMovementState();	//!< cached ptr for movement state
	protected ref DayZPlayerImplementAiming				m_AimingModel;
	protected ref DayZPlayerImplementMeleeCombat		m_MeleeCombat;
	protected ref DayZPlayerMeleeFightLogic_LightHeavy	m_MeleeFightLogic;
	protected ref DayZPlayerImplementSwimming			m_Swimming;
	protected ref DayZPlayerImplementThrowing			m_Throwing;
	protected ref DayZPlayerImplementJumpClimb			m_JumpClimb;
	protected ref DayZPlayerImplementFallDamage			m_FallDamage;
	protected ref PlayerSoundEventHandler 				m_PlayerSoundEventHandler;
	protected SHumanCommandMoveSettings 				m_MoveSettings;
	protected float 									m_FallYDiff;
	protected float										m_SprintedTime;
	protected bool										m_SprintFull;
	protected bool										m_IsFireWeaponRaised; //currently changed in 'HandleWeapons'
	protected bool										m_ShouldReload;
	protected bool										m_Camera3rdPerson;
	protected bool										m_CameraZoomToggle;
	protected bool										m_bADS;
	protected bool 										m_ProcessWeaponRaiseCompleted;
	protected bool 										m_WeaponRaiseCompleted;
	protected bool										m_CameraEyeZoom;
	protected bool										m_WasIronsight; // tag of ironsight - if false - then optics
///**/	protected bool										m_REMOVE; // stupid backup ironsight :-/
	protected bool										m_CameraIronsight; // Ironsight NOW!
	protected bool										m_CameraOptics; // Optics NOW!
//	protected float 									m_CameraIronsightNotRaisedTime;
	protected float 									m_DeathDarkeningCurrentTime;
	protected bool										m_IsTryingHoldBreath;
	protected bool										m_IsShootingFromCamera;
	protected bool										m_PlayerSelected;
	protected bool 										m_Suicide;
	protected bool										m_IsUnconscious;
	protected bool										m_ShouldBeUnconscious;
	bool												m_UnconsciousDebug;
	protected int			 							m_LastCommandBeforeUnconscious;
	ref WeaponDebug										m_WeaponDebug;
	ref Timer 											m_DeathEffectTimer;
	ref Timer 											m_ADSAutomationTimer;
	protected bool 										m_ADSTimerLaunched; //Nescessary, timer tick may not correspond to command handler tick!
	ref Timer 											m_FightEndBlendTimer;
//	protected bool 										m_ShouldReturnToOptics;
//	protected bool 										m_ShouldReturnToRegularIronsights;
//	protected bool 										m_ShouldReturnToBackupIronsights;
	//protected bool 									m_ForceHandleOptics;
	protected bool										m_ProcessFirearmMeleeHit;
	protected bool										m_ContinueFirearmMelee;
	protected bool 										m_LiftWeapon_player;
	protected bool 										m_ProcessLiftWeapon;
	protected bool 										m_ProcessLiftWeaponState;
	protected int										m_LastSurfaceUnderHash;
	protected Transport									m_TransportCache;
	protected string 									m_ClimbingLadderType;
	bool												m_isFBsymptomPlaying;
	protected bool 										m_HandheldOpticsInUse;
	protected bool										m_RaiseStarted;
	protected bool 										m_ResetADS;
	protected int										m_StepCounter;
	//protected bool 									m_DeathSyncSent;
	//protected bool 									m_KilledByHeadshot;
	//protected ref KillerData 							m_KillerData;
	protected int 										m_NextVoNNoiseTime;
	
	//! constructor 
	void 	DayZPlayerImplement()
	{
		//Print("DayZPlayerImplement: Init");
		m_IsFireWeaponRaised = false;
		m_SprintFull = false;
		m_SprintedTime = 0;
		m_AimingModel = new DayZPlayerImplementAiming(this);
		m_MeleeCombat = new DayZPlayerImplementMeleeCombat(this);
		m_MeleeFightLogic = new DayZPlayerMeleeFightLogic_LightHeavy(this);
		m_Swimming = new DayZPlayerImplementSwimming(this);		
		m_Throwing = new DayZPlayerImplementThrowing(this);
		m_JumpClimb = new DayZPlayerImplementJumpClimb(this);
		m_FallDamage = new DayZPlayerImplementFallDamage(this);
		m_bADS = false;
		m_WeaponRaiseCompleted = false;
		m_CameraEyeZoom = false;
		m_CameraOptics = false;
		m_IsShootingFromCamera = true;
		m_ProcessFirearmMeleeHit = false;
		m_ContinueFirearmMelee = false;
		m_WasIronsight = true;
		#ifdef PLATFORM_CONSOLE
		m_Camera3rdPerson = !GetGame().GetWorld().Is3rdPersonDisabled();
		#endif
		m_LastSurfaceUnderHash = ("cp_gravel").Hash();
		m_NextVoNNoiseTime = 0;
	}

	DayZPlayerImplementAiming GetAimingModel()
	{
		return m_AimingModel;
	}

	DayZPlayerImplementMeleeCombat GetMeleeCombat()
	{
		return m_MeleeCombat;
	}
	
	DayZPlayerMeleeFightLogic_LightHeavy GetMeleeFightLogic()
	{
		return m_MeleeFightLogic;
	}
	
	DayZPlayerImplementThrowing GetThrowing()
	{
		return m_Throwing;
	}

	//! destructor 
	void 	~DayZPlayerImplement()
	{
	}

	override bool IsEyeZoom()
	{
		return m_CameraEyeZoom;
	}
	
	override bool IsShootingFromCamera()
	{
		return m_IsShootingFromCamera;
	}
	
	void OverrideShootFromCamera(bool pState)
	{
		m_IsShootingFromCamera = pState;
	}

	bool IsInIronsights()
	{
		return m_CameraIronsight;
	}
	
	bool IsInOptics()
	{
		return m_CameraOptics;
	}

	bool IsFireWeaponRaised()
	{
		return m_IsFireWeaponRaised;
	}

	bool IsTryingHoldBreath()
	{
		return m_IsTryingHoldBreath;
	}

	bool IsSprintFull()
	{
		return m_SprintFull;
	}

	void ShowWeaponDebug(bool show)
	{
		if (show)
		{
			m_WeaponDebug = new WeaponDebug;
		}
		else
		{
			m_WeaponDebug = null;
		}
	}
	
	void SetFallYDiff(float value)
	{
		m_FallYDiff = value;
	}
	
	
	//TODO: Swap from optics
	void SetIronsights(bool value)
	{
//		Print("SetIronsights");
		m_CameraIronsight = value;
		
		if( m_CameraIronsight )
			m_CameraOptics = false;

//		if( value )
//			Print("m_WasIronsight = true");
//		else
//			Print("m_WasIronsight = false");
			
		m_WasIronsight = value;
	}

	void SetClimbingLadderType(string value)
	{
		m_ClimbingLadderType = value;
	}

	//! Implementations only! - used on PlayerBase
	bool CanConsumeStamina(EStaminaConsumers consumer);
	bool CanStartConsumingStamina(EStaminaConsumers consumer);
	void DepleteStamina(EStaminaModifiers modifier, float dT = -1) {}
	
	bool PlaySoundEvent(EPlayerSoundEventID id, bool from_anim_system = false, bool is_from_server = false);
	
		
	bool IsFBSymptomPlaying()
	{
		return m_isFBsymptomPlaying;
	}

	//experiment with on-update input handling
	/*override void EOnFrame(IEntity other, float timeSlice)
	{
		array<string> input_actions = new array<string>;
		input_actions.Insert("UAZoomInOptics");
		input_actions.Insert("UAZoomOutOptics");
		input_actions.Insert("UAADSToggle");
		input_actions.Insert("UAZoomIn");
		
		Input input = GetGame().GetInput();
		
		for (int i = 0; i< input_actions.Count(); i++)
		{
			if (input.LocalPress(input_actions.Get(i),true))
			{
				m_input_name = input_actions.Get(i);
				break;
			}
		}
	}
	
	override void EOnPostFrame(IEntity other, int extra)
	{
		if (m_input_process_successful)
		{
			m_input_name = "";
			m_input_process_successful = false;
		}
	}*/

	//-------------------------------------------------------------
	//!
	//! HandleDeath
	//! 

	int m_DeathAnimType = -2;
	float m_DeathHitDir = 0;
	static const float m_DeathSimulationTimeout = 10.0; //in seconds
	float m_DeathStartTime = 0.0;
	bool m_DeathJuctureSent = false;

	bool	HandleDeath(int pCurrentCommandID)
	{
		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_DEATH)
		{
			if ((GetGame().GetTickTime() - m_DeathStartTime) > m_DeathSimulationTimeout)
				DisableSimulation(true);
			return true;
		}
		
		if (m_DeathAnimType != -2 && g_Game.GetMissionState() == g_Game.MISSION_STATE_GAME)
		{
			if (!m_Suicide) 	
			{
				int type = m_DeathAnimType;
				if( type == -1 ) 
					type = GetTypeOfDeath(pCurrentCommandID);
				
				// Maybe move elsewhere? Couldn't find a more fitting place to do this but I'm sure there is one
				// If player dies in driver seat, car shouldn't be simulated by the dead player anymore
				if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_VEHICLE)
				{
					HumanCommandVehicle hcv = GetCommand_Vehicle();
					Transport transport = hcv.GetTransport();
					int crewPos = transport.CrewMemberIndex( this );
					transport.CrewDeath( crewPos );
				}
				
				DayZPlayerCommandDeathCallback callback = DayZPlayerCommandDeathCallback.Cast(StartCommand_Death(type, m_DeathHitDir, DayZPlayerCommandDeathCallback));
				callback.m_pPlayer = PlayerBase.Cast(this);
			}
			
			ResetDeathStartTime();
			// disable voice communication
			GetGame().GetWorld().SetVoiceOn(false);
			
			return true;
		}

		return false;
	}
	
	void	ResetDeathStartTime()
	{
		m_DeathStartTime = GetGame().GetTickTime();
	}
	
	int		GetTypeOfDeath(int pCurrentCommandID)
	{
		switch( pCurrentCommandID )
		{
		case DayZPlayerConstants.COMMANDID_SWIM:
			return 11;
		case DayZPlayerConstants.COMMANDID_FALL:
			return 12;
		case DayZPlayerConstants.COMMANDID_UNCONSCIOUS:
			{
				
				HumanCommandUnconscious hcu = GetCommand_Unconscious();
				if( hcu )
				{
					if( hcu.IsOnLand() )
						return 13;
					if( hcu.IsInWater() )
						return 14;
				}		
			}
			break;
		}
		
		return 0;
	}
	
	void SendDeathJuncture(int pAnimTypeDeath, float pAnimHitDirDeath)
	{
		if( m_DeathJuctureSent )
			return;
		
		DayZPlayerSyncJunctures.SendDeath(this, pAnimTypeDeath, pAnimHitDirDeath);
		m_DeathJuctureSent = true;
	}
	
	override void EEKilled( Object killer )
	{
		SendDeathJuncture(-1, 0);
		
		super.EEKilled(killer);
	}
	
	static const int DEAD_SCREEN_DELAY = 1000; //ms
	static const float DEFAULT_DYING_TIME = 2.5; //s
	static const float DYING_PROGRESSION_TIME = 0.05; //s
	
	void ShowDeadScreen(bool show, float duration)
	{
	#ifdef PLATFORM_PS4
		//PSN Set multiplay state - NOT ACTIVE	
		OnlineServices.SetMultiplayState( false );
		//Print("=== PLATFORM_PS4 - SetMultiplayState NOT ACTIVE [ShowDeadScreen]");
		//
	#endif		
		
	#ifndef NO_GUI
		if (show && IsPlayerSelected())
		{
		#ifdef PLATFORM_CONSOLE
			GetGame().GetUIManager().ScreenFadeIn(duration, "#dayz_implement_dead", FadeColors.DARK_RED, FadeColors.WHITE);
		#else
			GetGame().GetUIManager().ScreenFadeIn(duration, "#dayz_implement_dead", FadeColors.BLACK, FadeColors.WHITE);
		#endif
		}
		else
		{
			GetGame().GetUIManager().ScreenFadeOut(0);
		}
		
		if (duration > 0)
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(StopDeathDarkeningEffect, duration*1000, false);
		else
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(StopDeathDarkeningEffect);
		
	#endif
	}
	
	override void StopDeathDarkeningEffect()
	{
		if ( m_DeathEffectTimer && m_DeathEffectTimer.IsRunning() )
		{
			m_DeathEffectTimer.Stop();
		}
		//PPEffects.SetDeathDarkening(0);
	}
	
	void SimulateDeath(bool state)
	{
		//bool 	BrainDeath;
		float 	duration = DEFAULT_DYING_TIME;
		if (g_Game.GetMissionState() != DayZGame.MISSION_STATE_GAME)	 return;
		//Print("Calling simulate death in state: " + state)
		
		//controlls
		LockControls(state);
		
		//video
		if (m_KilledByHeadshot)
		{
			duration = 0;
		}
		if (state && duration > DYING_PROGRESSION_TIME && !m_IsUnconscious)
		{
			//Print("--SimulateDeath--");
			if (!m_DeathEffectTimer)
				m_DeathEffectTimer = new Timer();
			//TODO: move below audio to execute the delayed stuff all at once. Plus, rename the fn called.
			m_DeathEffectTimer.Run(DYING_PROGRESSION_TIME, this, "SetDeathDarknessLevel", new Param2<float,float>( duration, DYING_PROGRESSION_TIME ), true);
		}
		/*else if (!state && IsAlive() && m_DeathEffectTimer && m_DeathEffectTimer.IsRunning())
		{
			m_DeathEffectTimer.Stop();
		}*/
		if (state)
			ShowDeadScreen(state,duration);
		else
			ShowDeadScreen(state,0);
		//audio
		if (state == true)
		{
			GetGame().GetSoundScene().SetSoundVolume(0,0);
			GetGame().GetSoundScene().SetSpeechExVolume(0,0);
			GetGame().GetSoundScene().SetMusicVolume(0,0);
			GetGame().GetSoundScene().SetVOIPVolume(0,0);
			GetGame().GetSoundScene().SetRadioVolume(0,0);
		}
		else
		{
			GetGame().GetSoundScene().SetSoundVolume(g_Game.m_volume_sound,1);
			GetGame().GetSoundScene().SetSpeechExVolume(g_Game.m_volume_speechEX,1);
			GetGame().GetSoundScene().SetMusicVolume(g_Game.m_volume_music,1);
			GetGame().GetSoundScene().SetVOIPVolume(g_Game.m_volume_VOIP,1);
			GetGame().GetSoundScene().SetRadioVolume(g_Game.m_volume_radio,1);
		}
	}
	
	void LockControls(bool state)
	{
		if (state == true)
		{
			GetGame().GetInput().ChangeGameFocus(1, INPUT_DEVICE_MOUSE);
			GetGame().GetInput().ChangeGameFocus(1, INPUT_DEVICE_KEYBOARD);
			GetGame().GetInput().ChangeGameFocus(1, INPUT_DEVICE_GAMEPAD);
			
			if (GetGame().GetUIManager()) 	GetGame().GetUIManager().ShowUICursor(true);
		}
		else
		{
			GetGame().GetInput().ChangeGameFocus(-1, INPUT_DEVICE_MOUSE);
			GetGame().GetInput().ChangeGameFocus(-1, INPUT_DEVICE_KEYBOARD);
			GetGame().GetInput().ChangeGameFocus(-1, INPUT_DEVICE_GAMEPAD);
			
			if (GetGame().GetUIManager())
			{
				if (GetGame().GetUIManager().GetMenu())
				{
					GetGame().GetUIManager().ShowUICursor(true);
				}
				else
				{
					GetGame().GetUIManager().ShowUICursor(false);
				}
			}
		}
	}
	
	//TODO: rename and use for audio as well
	void SetDeathDarknessLevel(float duration, float tick_time)
	{
	}
	
	void RunADSTimer()
	{
		if (!m_ADSAutomationTimer)
			m_ADSAutomationTimer = new Timer();
		if (!m_ADSAutomationTimer.IsRunning() && !m_ADSTimerLaunched)
		{
			//Print("RunADSTimer | STS: " + GetSimulationTimeStamp());
			m_ADSAutomationTimer.Run(PlayerConstants.WEAPON_RAISE_BLEND_DELAY,this,"SendCompleteWeaponRaiseJuncture");
			m_ADSTimerLaunched = true;
		}
	}
	
	void StopADSTimer()
	{
		if (m_ADSAutomationTimer)
			m_ADSAutomationTimer.Stop();
		m_ADSTimerLaunched = false;
		m_WeaponRaiseCompleted = false;
	}
	
	void SendCompleteWeaponRaiseJuncture()
	{
		ScriptJunctureData pCtx = new ScriptJunctureData;
		//pCtx.Write(state);
		
		SendSyncJuncture(DayZPlayerSyncJunctures.SJ_WEAPON_RAISE_COMPLETED, pCtx);
	}
	
	void CompleteWeaponRaise()
	{
		m_WeaponRaiseCompleted = true;
		//Print("m_WeaponRaiseCompleted | STS: " + GetSimulationTimeStamp());
	}
	
	bool IsWeaponRaiseCompleted()
	{
		return m_WeaponRaiseCompleted;
	}
	
	DayZPlayerInventory GetDayZPlayerInventory ()
	{
		DayZPlayerInventory inv = DayZPlayerInventory.Cast(GetInventory());
		return inv;
	}

	override void OnInputForRemote (ParamsReadContext ctx)
	{
		int userDataType = 0;
		if (!ctx.Read(userDataType))
		{
			Error("DayZPlayer: OnInputForRemote - cannot read input type");
			return;
		}

		//Print("[inv] Input For Remote! type=" + userDataType);
		switch (userDataType)
		{
			case INPUT_UDT_WEAPON_REMOTE_EVENT:
				GetDayZPlayerInventory().OnEventForRemoteWeapon(ctx);
				break;
			case INPUT_UDT_HAND_REMOTE_EVENT:
				GetDayZPlayerInventory().OnHandEventForRemote(ctx);
				break;
			case INPUT_UDT_INVENTORY:
				GetDayZPlayerInventory().OnInputUserDataForRemote(ctx);
				break;
			default:
				Error("OnInputForRemote - unknown userDataType=" + userDataType);
				break;
		}
	}

	override void OnInputFromServer (ParamsReadContext ctx)
	{
		int userDataType = 0;
		if (!ctx.Read(userDataType))
		{
			Error("DayZPlayer: OnInputFromServer - cannot read input type");
			return;
		}

		switch (userDataType)
		{
			case INPUT_UDT_INVENTORY:
				GetDayZPlayerInventory().OnServerInventoryCommand(ctx);
				break;
			default:
				Error("OnInputFromServer - unknown userDataType=" + userDataType);
				break;
		}
	}
	void RequestSoundEvent(EPlayerSoundEventID id, bool from_server_and_client = false);
	protected void SendSoundEvent(EPlayerSoundEventID id);
	
	override void OnItemInHandsChanged ()
	{
		Print("inv: DayZPlayerImplement::OnItemInHandsChanged: "/* + GetHumanInventory().GetEntityInHands()*/);
		StopWeaponEvent();
		GetItemAccessor().OnItemInHandsChanged();
		
		if (!IsAlive())
		{
			Print("inv: DayZPlayerImplement::OnItemInHandsChanged - human not alive! calling ResetWeaponInHands");
			GetItemAccessor().ResetWeaponInHands();
		}
	}

	WeaponManager GetWeaponManager () { return null; }

	//-------------------------------------------------------------
	//!
	//! HandleWeapons
	//!
	void HandleWeapons(float pDt, Entity pInHands, HumanInputController pInputs, out bool pExitIronSights)
	{
		HumanCommandWeapons		hcw = GetCommandModifier_Weapons();
    	GetDayZPlayerInventory().HandleWeaponEvents(pDt, pExitIronSights);

		Weapon_Base weapon;
		Class.CastTo(weapon, pInHands);
		
		CheckLiftWeapon();
		ProcessLiftWeapon();

		// @TODO: refactor to action (rm from input, mk action)
		/*if (pInputs.IsReloadOrMechanismSingleUse() && GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT )
		{
			if (weapon && weapon.CanProcessWeaponEvents())
			{
				if (GetWeaponManager().CanEjectBullet(weapon))
				{
					GetWeaponManager().EjectBullet();
					pExitIronSights = true;
				}
			}
		}*/
		
		GetMovementState(m_MovementState);
		
		//Print("IsInIronsights " + IsInIronsights());
		//Print("IsInOptics " + IsInOptics());
		// hold breath
		if (pInputs.IsHoldBreath() && m_MovementState.IsRaised() && (IsInIronsights() || IsInOptics()))
		{
			m_IsTryingHoldBreath = true;
		}
		else
		{
			m_IsTryingHoldBreath = false;
		}
		
		ItemOptics optic = weapon.GetAttachedOptics();

		if (pInputs.IsFireModeChange())
		{
			GetWeaponManager().SetNextMuzzleMode();
		}
		if (pInputs.IsZeroingUp())
		{
			if (optic && (optic.IsInOptics() || optic.IsUsingWeaponIronsightsOverride()) )
				optic.StepZeroingUp();
			else
			{
				//weapon.StepZeroingUp(weapon.GetCurrentMuzzle());
				weapon.StepZeroingUpAllMuzzles();
			}
		}
		if (pInputs.IsZeroingDown())
		{
			if (optic && (optic.IsInOptics() || optic.IsUsingWeaponIronsightsOverride()) )
				optic.StepZeroingDown();
			else
			{
				//weapon.StepZeroingDown(weapon.GetCurrentMuzzle());
				weapon.StepZeroingDownAllMuzzles();
			}
		}
		
		if (!m_LiftWeapon_player && (m_CameraIronsight || !weapon.CanEnterIronsights() || m_CameraOptics/*m_ForceHandleOptics*/)) 	// HACK straight to optics, if ironsights not allowed
		{
			if (optic)
				HandleOptic(optic, false, pInputs, pExitIronSights);
		}

		/*if (!m_LiftWeapon_player && weapon && weapon.CanProcessWeaponEvents())
		{
			if (pInputs.IsReloadOrMechanismContinuousUseStart())
			{
				if (GetWeaponManager().CanUnjam(weapon))
				{
					//weapon.ProcessWeaponEvent(new WeaponEventUnjam(this));
					GetWeaponManager().Unjam();
					//pExitIronSights = true;
				}
			}
		}*/

		if (!m_MovementState.IsRaised())
		{
			m_IsFireWeaponRaised = false;
			if (weapon && weapon.IsInOptics())
				weapon.ExitOptics();
			
			StopADSTimer();

			return; // if not raised => return
		}
		else
		{
			m_IsFireWeaponRaised = true;
			if ( !m_WeaponRaiseCompleted && (!m_ADSAutomationTimer || (m_ADSAutomationTimer && !m_ADSAutomationTimer.IsRunning())) )
			{
				if (GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER || !GetGame().IsMultiplayer())
					RunADSTimer();
			}
			if (m_ProcessWeaponRaiseCompleted)
			{
				CompleteWeaponRaise();
				m_ProcessWeaponRaiseCompleted = false;
			}
		}
		
		#ifdef PLATFORM_CONSOLE
			if ( !pInputs.IsMeleeFastAttackModifier() )
			{
		#endif
		#ifdef SERVER_FOR_CONSOLE
			if ( !pInputs.IsMeleeFastAttackModifier() )
			{
		#endif
		
		//! fire
		//if (!m_LiftWeapon_player && weapon && !weapon.IsDamageDestroyed() && weapon.CanProcessWeaponEvents() )
		if (GetWeaponManager().CanFire(weapon))
		{
			bool autofire = weapon.GetCurrentModeAutoFire(weapon.GetCurrentMuzzle()) && weapon.IsChamberEjectable(weapon.GetCurrentMuzzle());
			int burst = weapon.GetCurrentModeBurstSize(weapon.GetCurrentMuzzle());
			if (!autofire)
			{
				if (pInputs.IsUseButtonDown())
				{
					GetWeaponManager().Fire(weapon);
				}
			}
			else
			{
				if (pInputs.IsUseButton())
				{
					GetWeaponManager().Fire(weapon);
				}
			}
		}		

		#ifdef PLATFORM_CONSOLE
		if( GetGame().GetInput().LocalRelease( "UAFire", false ) || m_ShouldReload )
		{
			if( !weapon.IsWaitingForActionFinish() && !IsFighting() )
			{
				int muzzle_index = weapon.GetCurrentMuzzle();
			
				if ( weapon.IsChamberFiredOut( muzzle_index ) )
				{
					if ( weapon.CanProcessWeaponEvents() )
					{
						if ( GetWeaponManager().CanEjectBullet(weapon) )
						{
							GetWeaponManager().EjectBullet();
							pExitIronSights = true;
							m_ShouldReload = false;
						}
					}
				}
			}
			else
			{
				m_ShouldReload = true;
			}
		}
		#endif
		#ifdef PLATFORM_CONSOLE
			}
		#endif
		#ifdef SERVER_FOR_CONSOLE
			}
		#endif
	}
	
	void HandleOptic (notnull ItemOptics optic, bool inHands, HumanInputController pInputs, out bool pExitOptics)
	{
		Weapon_Base weapon = Weapon_Base.Cast(optic.GetHierarchyParent());
		if (inHands)
		{
			if (m_CameraOptics)
			{
				if (!optic.IsInOptics())
					m_CameraOptics = false;
			}
			else
			{
				if (optic.IsInOptics())
					m_CameraOptics = true;
			}
		}
		else if (weapon)
		{
			Input input = GetGame().GetInput();
			/*if (pInputs.IsWeaponRaised() && m_ShouldReturnToOptics && !m_CameraOptics && optic)
			{
				SwitchOptics(optic,true);
			}*/
			
			if (/*input.LocalPress("UAZoomInOptics",true)*/pInputs.IsZoomIn())
			{
				m_WasIronsight = false;
//				Print("m_WasIronsight = false");
			
				if (!m_CameraOptics/* && (weapon.CanEnterIronsights() && m_CameraIronsight)*/)
				{
					SwitchOptics(optic,true);
	//				m_WasIronsight = true;
				}
				else
				{
					optic.StepFOVUp();
				}
			}
	
			if (/*input.LocalPress("UAZoomOutOptics",true)*/pInputs.IsZoomOut())
			{
				if (m_CameraOptics)
				{
					if (optic.GetStepFOVIndex() > 0)
					{
						optic.StepFOVDown();
					}
					else if (weapon.CanEnterIronsights())
					{
						//SwitchOptics(optic,false);
						ExitSights();
						m_CameraIronsight = true;
//						Print("m_WasIronsight = true");
						m_WasIronsight = true;
						//m_ShouldReturnToOptics = false;
						//m_ForceHandleOptics = false;
					}
					
//					if ( optics && optics.IsUsingWeaponIronsightsOverride()) )
//						m_WasIronsight = true;
					
				}
			}
		}
	}

	//-------------------------------------------------------------
	//!
	//! HandleDamageHit
	//! 

	bool m_DamageHitFullbody = false;
	int m_DamageHitAnimType = -1;
	float m_DamageHitDir = 0;

	bool HandleDamageHit(int pCurrentCommandID)
	{
		// If transportcache is not null, player is unconscious inside of a car and should not head into a damage command
		if (m_DamageHitAnimType != -1 && m_TransportCache == null)
		{
			if (m_DamageHitFullbody)
			{
				StartCommand_Damage(m_DamageHitAnimType, m_DamageHitDir);
				m_DamageHitAnimType = -1;
				return true;
			}
			else
			{
				AddCommandModifier_Damage(m_DamageHitAnimType, m_DamageHitDir);
				m_DamageHitAnimType = -1;
				return false;
			}
		}

		return pCurrentCommandID == DayZPlayerConstants.COMMANDID_DAMAGE;
	}

	//! selects animation type and direction based on damage system data
	bool EvaluateDeathAnimation(int pDamageType, EntityAI pSource, string pAmmoType, out int pAnimType, out float pAnimHitDir)
	{
		//! 
		bool doPhxImpulse = GetGame().ConfigGetInt("cfgAmmo " + pAmmoType + " doPhxImpulse") > 0;

		//! anim type (-1 = default)
		pAnimType = -1;		
		if( doPhxImpulse )
			pAnimType = 10;

		//! direction
		vector targetDirection = GetDirection();
		vector toSourceDirection = (pSource.GetPosition() - GetPosition());

		targetDirection[1] = 0;
		toSourceDirection[1] = 0;

		targetDirection.Normalize();
		toSourceDirection.Normalize();

		float cosFi = vector.Dot(targetDirection, toSourceDirection);
		vector cross = targetDirection * toSourceDirection;

		pAnimHitDir = Math.Acos(cosFi) * Math.RAD2DEG;
		if (cross[1] < 0)
			pAnimHitDir = -pAnimHitDir;

		//Print("hitdir: " + pAnimHitDir.ToString());

		return true;
	}

	//! selects animation type and direction based on damage system data
	bool EvaluateDamageHitAnimation(TotalDamageResult pDamageResult, int pDamageType, EntityAI pSource, string pComponent, string pAmmoType, vector pModelPos, out int pAnimType, out float pAnimHitDir, out bool pAnimHitFullbody)
	{
		pAnimType = 0;
		pAnimHitFullbody = false; // additive anm
		GetMovementState(m_MovementState);

		switch(pDamageType)
		{
			case DT_CLOSE_COMBAT:
				//! ignore hit impacts in prone (for now)
				if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE || m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
					return false;

				//! impact from infected attack will be light only
			 	if (pSource.IsInherited(DayZInfected))
					break;

				pAnimType = GetGame().ConfigGetInt("cfgAmmo " + pAmmoType + " hitAnimation");
				if (pAnimType == 1 && !m_MeleeFightLogic.IsInBlock())
					pAnimHitFullbody = true;
			break;
			case DT_FIRE_ARM:
				int impactBehaviour = 0;
				//! play full body when these coponents were hit
				if ( pComponent == "Torso" || pComponent == "Head")
				{
					impactBehaviour = GetGame().ConfigGetInt("cfgAmmo " + pAmmoType + " impactBehaviour");
					float fireDamage = pDamageResult.GetHighestDamage("Health");
					if( fireDamage > 80.0 && impactBehaviour == 1 )
					{
						pAnimHitFullbody = true;
					}
				}

			break;
			case DT_EXPLOSION:
			break;
			case DT_CUSTOM:
				pAnimType = GetGame().ConfigGetInt("cfgAmmo " + pAmmoType + " hitAnimation");
				if( pAnimType == 1 )
					pAnimHitFullbody = true;
				else
					return false; //! skip evaluation of dmg hit animation
			break;
		}

		//! direction
		vector targetDirection = GetDirection();
		vector toSourceDirection = (pSource.GetPosition() - GetPosition());

		targetDirection[1] = 0;
		toSourceDirection[1] = 0;

		targetDirection.Normalize();
		toSourceDirection.Normalize();

		float cosFi = vector.Dot(targetDirection, toSourceDirection);
		vector cross = targetDirection * toSourceDirection;

		pAnimHitDir = Math.Acos(cosFi) * Math.RAD2DEG;
		if (cross[1] < 0)
			pAnimHitDir = -pAnimHitDir;

		//Print("hitdir: " + pAnimHitDir.ToString());

		return true;
	}

	//! event from damage system
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		m_TransportHitRegistered = false;
		
		if( !IsAlive() )
		{
			int animTypeDeath;
			float animHitDirDeath;
			if (EvaluateDeathAnimation(damageType, source, ammo, animTypeDeath, animHitDirDeath))
			{
				SendDeathJuncture(animTypeDeath, animHitDirDeath);
			}
			dBodySetInteractionLayer(this, PhxInteractionLayers.AI_NO_COLLISION);
			
			if ( !m_DeathSyncSent ) //checked until the death is evaluated by 'OnCommandHandlerTick' higher up the road
			{
				Man killer = source.GetHierarchyRootPlayer();
				
				if ( !m_KillerData ) //only one player is considered killer in the event of crossfire
				{
					m_KillerData = new KillerData;
					m_KillerData.m_Killer = killer;
					m_KillerData.m_MurderWeapon = source;
				}
				
				if ( killer && killer.IsPlayer() )
				{
					// was player killed by headshot?
					if ( dmgZone == "Brain" )
					{
						m_KilledByHeadshot = true;
						if (m_KillerData.m_Killer == killer)
							m_KillerData.m_KillerHiTheBrain = true;
					}
				}
			}
		}
		else
		{
			int animType;
			float animHitDir;
			bool animHitFullbody;
			if (EvaluateDamageHitAnimation(damageResult, damageType, source, dmgZone, ammo, modelPos, animType, animHitDir, animHitFullbody))
			{
				DayZPlayerSyncJunctures.SendDamageHit(this, animType, animHitDir, animHitFullbody);
			}
			else
			{
				RequestSoundEvent(EPlayerSoundEventID.TAKING_DMG_LIGHT);
				//add code here
			}
		}

		// interupt melee for non-blocked hit or heavy hit
		if( /*m_MeleeFightLogic.IsInBlock() ||*/ animHitFullbody )
		{
			HumanCommandMelee2 hcm = GetCommand_Melee2();
			if(hcm) hcm.Cancel();
		}
	}

	override void EEHitByRemote(int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos)
	{
		super.EEHitByRemote(damageType, source, component, dmgZone, ammo, modelPos);
		
		//Print("DayZPlayerImplement : EEHitByRemote");
	}

	//-------------------------------------------------------------
	//!
	//! This HeadingModel
	//! 

	float 	m_fLastHeadingDiff = 0;

	//!
	override bool	HeadingModel(float pDt, SDayZPlayerHeadingModel pModel)
	{
		GetMovementState(m_MovementState);
		
		//! on ladder - do nothing 
		if( m_MovementState.m_CommandTypeId == DayZPlayerConstants.COMMANDID_LADDER )
		{
			m_fLastHeadingDiff = 0;
			return false;
		}
		if( m_MovementState.m_CommandTypeId == DayZPlayerConstants.COMMANDID_VEHICLE )
		{
			m_fLastHeadingDiff = 0;
			return false;
		}
		if( m_MovementState.m_CommandTypeId == DayZPlayerConstants.COMMANDID_UNCONSCIOUS )
		{
			m_fLastHeadingDiff = 0;
			//pModel.m_iCamMode = DayZPlayerConstants.CAMERAMODE_HEAD;
			return false;
		}
		
		if( m_MovementState.m_CommandTypeId == DayZPlayerConstants.COMMANDID_CLIMB )
		{
			m_fLastHeadingDiff = 0;
			return false;
		}
		/*if ( IsInFBEmoteState() )
		{
			m_fLastHeadingDiff = 0;
			return false;
		}*/
		
#ifdef DEVELOPER
		int	actMenuValue = DiagMenu.GetValue(DayZPlayerConstants.DEBUG_TURNSLIDE);
		if (actMenuValue != 0)
		{
			//! this clamps heading 
			return DayZPlayerImplementHeading.ClampHeading(pDt, pModel, m_fLastHeadingDiff);
		}
#endif
		
		if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
		{
			m_fLastHeadingDiff = 0;
			return false;
		}

		//! if it's standing from prone on back  
		if (m_MovementState.m_CommandTypeId == DayZPlayerConstants.COMMANDID_MOVE)
		{
			HumanCommandMove hcm = GetCommand_Move();
			if (hcm.IsStandingFromBack())
			{
				m_fLastHeadingDiff = 0;
				return false;
			}
		}
		
		HumanItemAccessor hia = GetItemAccessor();
		HumanItemBehaviorCfg hibcfg = hia.GetItemInHandsBehaviourCfg();
		if (hibcfg && hibcfg.m_StanceRotation[m_MovementState.m_iStanceIdx] == DayZPlayerConstants.ROTATION_DISABLE)
		{
			return DayZPlayerImplementHeading.NoHeading(pDt, pModel, m_fLastHeadingDiff);
		}

		return DayZPlayerImplementHeading.RotateOrient(pDt, pModel, m_fLastHeadingDiff);	
	}


	//-------------------------------------------------------------
	//!
	//! This Aiming Model
	//! 

	override bool	AimingModel(float pDt, SDayZPlayerAimingModel pModel)
	{
		GetMovementState(m_MovementState);
		
		bool isInIronsights = IsInIronsights();
		bool isFireWeaponRaised = IsFireWeaponRaised();
		
		
		if(isFireWeaponRaised)
		{
			if(!m_RaiseStarted)
			{
				m_AimingModel.OnRaiseBegin(this);
				m_RaiseStarted = true;
			}
			m_AimingModel.ProcessAimFilters( pDt, pModel, m_MovementState.m_iStanceIdx );
		}
		else
		{
			m_RaiseStarted = false;
		}
		
		if (isInIronsights)
		{
			pModel.m_fAimSensitivity = 0.3;
		}
		else if (isFireWeaponRaised)
		{
			pModel.m_fAimSensitivity = 0.5;
		}
		else
		{
			pModel.m_fAimSensitivity = 1.0;
		}
		
		return true;
	}

	//-------------------------------------------------------------
	//!
	//! Jump
	//! 
	
	
	//!
	bool CanJump()
	{
		if ( IsFBSymptomPlaying() || IsRestrained() || IsUnconscious() || IsInFBEmoteState() )
			return false;
		
		if ( m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE || m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
			return false;
		
		HumanItemBehaviorCfg hibcfg = GetItemAccessor().GetItemInHandsBehaviourCfg();
		if ( !hibcfg.m_bJumpAllowed )
			return false;
		
		if ( !DayZPlayerUtils.PlayerCanChangeStance(this, DayZPlayerConstants.STANCEIDX_ERECT) || !DayZPlayerUtils.PlayerCanChangeStance(this, DayZPlayerConstants.STANCEIDX_RAISEDERECT) )
			return false;
		
		return true;
	}

	bool CanClimb( int climbType, SHumanCommandClimbResult climbRes )
	{
		if ( IsFBSymptomPlaying() || IsRestrained() || IsUnconscious() || IsInFBEmoteState() )
			return false;
		
		if ( m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE || m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
			return false;
		
		HumanItemBehaviorCfg hibcfg = GetItemAccessor().GetItemInHandsBehaviourCfg();
		if ( !hibcfg.m_bJumpAllowed )
			return false;
		
		if (climbRes)
		{
			EntityAI entity;
			if (Class.CastTo(entity,climbRes.m_GrabPointParent) && entity.IsHologram())
				return false;
			if (Class.CastTo(entity,climbRes.m_ClimbStandPointParent) && entity.IsHologram())
				return false;
			if (Class.CastTo(entity,climbRes.m_ClimbOverStandPointParent) && entity.IsHologram())
				return false;
		}

		return true;
	}
	

	void OnJumpStart()
	{
	}
	
	void OnJumpEnd(int pLandType = 0)
	{
	}
	
	void StopHandEvent()
	{
		GetDayZPlayerInventory().CancelHandEvent();
	}
	
	void StopWeaponEvent()
	{
		GetDayZPlayerInventory().CancelWeaponEvent();
	}
	
	void AbortWeaponEvent()
	{
		GetDayZPlayerInventory().AbortWeaponEvent();
	}

	//-------------------------------------------------------------
	//!
	//! This is main command logic
	//! 

	void HandleADS()
	{
		if ( !IsAlive() )
		{
			if (m_CameraIronsight || m_CameraOptics)
			{
				ExitSights();
			}
			return;
		}
		
		bool bADSToggle = false;
		bool exitSights = false;
		HumanInputController hic = GetInputController();
		HumanItemAccessor 	hia = GetItemAccessor();
		HumanCommandWeapons	hcw = GetCommandModifier_Weapons();
		PlayerBase playerPB = PlayerBase.Cast(this);
		
		//Print("UAWalkRunTemp = " + GetUApi().GetInputByName("UAWalkRunTemp").LocalValue());
		
		if( !hia.IsItemInHandsWeapon() )
			hic.ResetADS();
		
		if( m_ResetADS )
		{
			hic.ResetADS();
			m_ResetADS = false;
		}
		
		if( (!m_IsFireWeaponRaised) || m_LiftWeapon_player )
		{
			StopADSTimer();
			if (m_ADSAutomationTimer)
				m_ADSAutomationTimer = null;
			
			exitSights = true;
		}
		else
		{
			if( m_bADS != hic.WeaponADS() )
			{
				m_bADS = hic.WeaponADS();
				bADSToggle = true;
/*				if( m_bADS && hic.WeaponWasRaiseClick() )
					m_WasIronsights = true;
				else
					m_WasIronsights = false;*/
			}
			
			if( m_bADS && !m_CameraIronsight && !m_CameraOptics )
			{				
				//!artificial Auto-ADS delay
				if ( !m_WeaponRaiseCompleted )
				{
					bADSToggle = false;
					exitSights = true;
				}
				else
				{
					bADSToggle = true;
				}
			}
		}
	
		if( bADSToggle )
		{
			if (hia.IsItemInHandsWeapon() && playerPB.GetItemInHands() && playerPB.GetItemInHands().IsWeapon() && playerPB.GetWeaponManager() && !playerPB.GetWeaponManager().IsRunning() )
			{
				Weapon_Base weapon = Weapon_Base.Cast(GetHumanInventory().GetEntityInHands());
				ItemOptics optic = weapon.GetAttachedOptics();
				bool switchToOptics = false;
				
				if( weapon.CanEnterIronsights() )
					switchToOptics = m_bADS;
				
				if( optic && !m_CameraOptics )
					switchToOptics = m_bADS;
				
				// go to ironsights - disable ironsights when
				//! if !raised
				//! if sprinting
				if ( !GetWeaponManager().IsRunning() )
				{
					GetMovementState(m_MovementState);
					if (!hic.IsWeaponRaised() || m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_SPRINT)
					{
						switchToOptics = false;
					}
				}
				
				// fixes camera switching during item transitions
				if( IsLiftWeapon() || !IsRaised() || GetDayZPlayerInventory().IsProcessing() || !IsWeaponRaiseCompleted() || IsFighting() )
					exitSights = true;
				
				// enter ironsight or optics
				if( switchToOptics )
				{					
					// for weapons which cannot enter ironsights anymore
					if( m_WasIronsight )
					{
						if( !weapon.CanEnterIronsights() )
							m_WasIronsight = false;
					}

					if( m_WasIronsight || !optic )
					{
//						Print("GO: IRONSIGHT");
						m_CameraIronsight = true;
						m_CameraOptics = false;
						m_WasIronsight = true;
					}
					else
					{
//						Print("GO: OPTICS");
						SwitchOptics(optic,true);
						m_CameraOptics = true;
						m_CameraIronsight = false;
					}
					
					if( hcw )
						hcw.SetADS(true);
				}
				else
				{			
					exitSights = true;
				}
			}
		}
		
		// leave ironsight and/ or optics
		if( exitSights && !m_HandheldOpticsInUse && (m_CameraIronsight || m_CameraOptics) || GetCommand_Melee2() )
		{
			ExitSights();
			
			if( hcw )
				hcw.SetADS(false);
		}
		
		/*Print("m_CameraIronsight " + m_CameraIronsight);
		Print("m_CameraOptics " + m_CameraOptics);
		Print("---------------");*/
	}
	
	void HandleView()
	{
		Input input = GetGame().GetInput();
	

		HumanInputController hic = GetInputController();
	
		bool camChange = hic.CameraViewChanged();

		//! 3rd person camera
		if ( camChange )
		{
			if (!GetGame().GetWorld().Is3rdPersonDisabled())
			{
				m_Camera3rdPerson = !m_Camera3rdPerson;
			}
			else
			{
				m_Camera3rdPerson = false;
			}
		}

		// exits optics completely, comment to return to ADS
		if (m_LiftWeapon_player && (IsInOptics() || IsInIronsights()))
			ExitSights();
		
		if (IsPlayerInStance(DayZPlayerConstants.STANCEMASK_RAISEDPRONE) && (IsInOptics() || IsInIronsights()))
			GetDayZPlayerType().SetVerticalMinimumAimLimit(DayZPlayerCameraIronsights.CONST_UD_MIN_BACK);
		else
			GetDayZPlayerType().SetVerticalMinimumAimLimit(DayZPlayerCameraIronsights.CONST_UD_MIN);

		//Print("m_LiftWeapon_player " + !m_LiftWeapon_player);
		//Print("IsFireWeaponRaised " + IsFireWeaponRaised());

		if( !m_MovementState.IsRaised() && hic.IsZoomToggle() )        
		{
			m_CameraZoomToggle = true;
			m_CameraEyeZoom = true;
		}
		else
		{
			m_CameraZoomToggle = false;
			m_CameraEyeZoom = false;
		}

		if (m_MovementState.IsRaisedInProne())
		{
			float headingAngle = MiscGameplayFunctions.GetHeadingAngle(this);
			float headingAngleDiffBlock = Math.AbsFloat(m_LastHeadingAngleBlock - headingAngle);
			float headingAngleDiffBlock2 = Math.AbsFloat(m_LastHeadingAngleBlock2 - headingAngle);
			float headingAngleDiff = 0.0;

			if (m_LastHeadingAngle * headingAngle < 0.0)
			{
				headingAngleDiff = Math.AbsFloat(m_LastHeadingAngle + headingAngle);
			}
			else
			{
				headingAngleDiff = Math.AbsFloat(m_LastHeadingAngle - headingAngle);
			}

			if (headingAngleDiff > 0.2)
			{
				int time = GetGame().GetTime();

				float timefilterconstant = 400 - (headingAngleDiff * 100);

				if (headingAngleDiffBlock > 0.8 && time - m_LastBackSoundTime > timefilterconstant)
				{

					float volume = headingAngleDiff / 0.5;
					if (volume > 1)
					{
						volume = 1;
					}
					if (volume < 0.25)
					{
						volume = 0.25;
					}

					m_SoundOffset = headingAngleDiff / 2;
					if (m_SoundOffset > 0.25)
					{
						m_SoundOffset = 0.25;
					}

					string soundSetName = "Cloth_Body_longmove_TShirt_Soundset";
					
					string bodyClothName = "";
					EntityAI attachment = GetInventory().FindAttachment(InventorySlots.BODY);
					if (attachment)
					{
						bodyClothName = attachment.GetAttachmentSoundType();
					}

					if (bodyClothName != "")
					{
						string path = "CfgSoundTables CfgAttachmentSoundTables Cloth_Body_Longmove_LookupTable";
						int soundCount = GetGame().ConfigGetChildrenCount(path);

						for (int i = 0; i < soundCount; i++)
						{
							string name = "";
							GetGame().ConfigGetChildName(path, i, name);
							
							if (name == bodyClothName)
							{
								TStringArray stringArray = new TStringArray();
								GetGame().ConfigGetTextArray(path + " " + name + " soundSets", stringArray);
								soundSetName = stringArray.Get(0);

								delete stringArray;
							}
						}
					}

					SoundParams soundParams = new SoundParams(soundSetName);
					SoundObjectBuilder soundObjectBuilder = new SoundObjectBuilder(soundParams);
					
					if (soundObjectBuilder != NULL)
					{
						SoundObject soundObject = soundObjectBuilder.BuildSoundObject();
						
						if (soundObject != NULL)
						{
							soundObject.SetPosition(GetPosition());

							AbstractWave wave = GetGame().GetSoundScene().Play3D(soundObject, soundObjectBuilder);
							wave.SetStartOffset(m_SoundOffset);
							wave.SetVolumeRelative(volume);

							m_LastBackSoundTime = GetGame().GetTime();
							m_LastHeadingAngleBlock = headingAngle;
						}
					}
				}

				if (headingAngleDiffBlock2 > 1.5 && time - m_LastBackSoundTime2 > (timefilterconstant * 2))
				{
					float volume2 = headingAngleDiff * 2;
					if (volume2 > 1)
					{
						volume2 = 1;
					}

					m_SoundOffset = headingAngleDiff / 1.8;
					if (m_SoundOffset < 0.1)
					{
						m_SoundOffset = 0.1;
					}
					if (m_SoundOffset > 0.3)
					{
						m_SoundOffset = 0.3;
					}

					string soundSetName2 = "walkProne_noHS_asphalt_ext_Char_SoundSet";
					string surfaceType = GetSurfaceType();

					if (surfaceType != "")
					{
						string movementSurfaceType = "walkProne_" + surfaceType;
						
						string path2 = "CfgSoundTables CfgStepSoundTables walkProne_noHS_Char_LookupTable";
						int soundCount2 = GetGame().ConfigGetChildrenCount(path2);

						for (int i2 = 0; i2 < soundCount2; i2++)
						{
							string name2 = "";
							GetGame().ConfigGetChildName(path2, i2, name2);
							
							if (name2 == movementSurfaceType)
							{
								TStringArray stringArray2 = new TStringArray();
								GetGame().ConfigGetTextArray(path2 + " " + name2 + " soundSets", stringArray2);
								soundSetName2 = stringArray2.Get(0);

								delete stringArray2;
							}
						}
					}

					SoundParams soundParams2 = new SoundParams(soundSetName2);
					SoundObjectBuilder soundObjectBuilder2 = new SoundObjectBuilder(soundParams2);
					
					if (soundObjectBuilder2 != NULL)
					{
						SoundObject soundObject2 = soundObjectBuilder2.BuildSoundObject();
						
						if (soundObject2 != NULL)
						{
							soundObject2.SetPosition(GetPosition());

							AbstractWave wave2 = GetGame().GetSoundScene().Play3D(soundObject2, soundObjectBuilder2);
							wave2.SetStartOffset(m_SoundOffset);
							wave2.SetVolumeRelative(volume2);            

							m_LastBackSoundTime2 = GetGame().GetTime();
							m_LastHeadingAngleBlock2 = headingAngle;
						}
					}
				}

				m_LastHeadingAngle = headingAngle;
			}
		}
	}

	float m_LastHeadingAngleBlock;
	float m_LastHeadingAngleBlock2;
	float m_LastHeadingAngle;
	int m_LastBackSoundTime;
	int m_LastBackSoundTime2;
	float m_SoundOffset;


	//-------------------------------------------------------------
	//!
	//! ModOverrides
	//! 
	// these functions are for modded overide in script command mods 

	bool	ModCommandHandlerBefore(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		return false;
	}

	bool	ModCommandHandlerInside(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		return false;
	}
	
	bool	ModCommandHandlerAfter(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		return false;
	}

	//-------------------------------------------------------------
	//!
	//! This is main command logic
	//! 

	
	//! 
	override void  CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (ModCommandHandlerBefore(pDt, pCurrentCommandID, pCurrentCommandFinished))
		{
			return;
		}
	
		//! handle weapon mode switch
		HandleADS();
		
		HumanInputController hic = GetInputController();
		EntityAI entityInHands = GetHumanInventory().GetEntityInHands();
		bool isWeapon		= entityInHands	&& entityInHands.IsInherited(Weapon);
		
		// handle weapons
		if(hic)
		{
			bool isOptics		= entityInHands	&& entityInHands.IsInherited(ItemOptics);
		
			if( isWeapon && (!hic.IsImmediateAction() || !m_ProcessFirearmMeleeHit || !m_ContinueFirearmMelee) )
			{
				m_ProcessFirearmMeleeHit = false;
				bool exitIronSights = false;
				HandleWeapons(pDt, entityInHands, hic, exitIronSights);
				//if (exitIronSights)
					//return;
			}
			else if( isOptics )
			{
				ItemOptics opticInHands = ItemOptics.Cast(entityInHands);
				bool exitOptic = false;
				HandleOptic(opticInHands, true, hic, exitOptic);
				//if (exitOptic)
					//return;		
			}	
		}
		
		//! handle death with high priority
		if (HandleDeath(pCurrentCommandID))
		{
			return;
		}

		GetMovementState(m_MovementState);
		/*
		// Unconscious test 
		if (hic.IsUseButtonDown())
		{
			if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE)
			{
				StartCommand_Unconscious();
			}
			else if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_UNCONSCIOUS)
			{
				HumanCommandUnconscious	hcu = GetCommand_Unconscious();
				hcu.WakeUp();
			}
		}
		*/
		
		//! handle Camera switch + zoom
		HandleView();	
	
		//! handle finished commands
		if (pCurrentCommandFinished)
		{
			if( pCurrentCommandID == DayZPlayerConstants.COMMANDID_UNCONSCIOUS)
			{
				if( (m_LastCommandBeforeUnconscious == DayZPlayerConstants.COMMANDID_VEHICLE) && (m_TransportCache != null))
				{
					int crew_index = m_TransportCache.CrewMemberIndex(this);
					int seat = m_TransportCache.GetSeatAnimationType(crew_index);
					StartCommand_Vehicle(m_TransportCache,crew_index, seat, true);
					m_TransportCache = null;
					return;
				}
			}
			// if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_ACTION || pCurrentCommandID == DayZPlayerConstants.COMMANDID_MELEE)
			// start moving

			// start falling ? (could happen after climbing)
			if (PhysicsIsFalling(true))
			{
				StartCommand_Fall(0);
				m_FallYDiff = GetPosition()[1];
				return;
			}

			//! default behaviou after finish is to start move
			if( m_Swimming.m_bWasSwimming )
			{
				StartCommand_Swim();
				return;
			}

			StartCommand_Move();
			return;
		};
	

		//--------------------------------------------
		//! debug test script command !PSOVIS

		if (ModCommandHandlerInside(pDt, pCurrentCommandID, pCurrentCommandFinished))
		{
			return;
		}
		

		//--------------------------------------------
		// vehicle handling
		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_VEHICLE)
		{
			HumanCommandVehicle hcv = GetCommand_Vehicle();
			if( hcv.WasGearChange() )
			{
				GearChangeActionCallback cb = GearChangeActionCallback.Cast(AddCommandModifier_Action(DayZPlayerConstants.CMD_ACTIONMOD_SHIFTGEAR, GearChangeActionCallback));
				cb.SetVehicleCommand(hcv);
			}
			
			return;
		}

		//! Sprint attack limiting - player has to be in full sprint for at least 0.5s
		//--------------------------------------------
		HumanCommandMove hcm = GetCommand_Move();
		if (hcm && hcm.GetCurrentMovementSpeed() > 2.99 && m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_SPRINT)
		{
			m_SprintedTime += pDt;
			if( m_SprintedTime > 0.5)
				m_SprintFull = true;
			else
				m_SprintFull = false;

			//Print(m_SprintedTime);
		}
		else
		{
			m_SprintedTime = 0.0;
			m_SprintFull = false;
		}

		//--------------------------------------------
		// swimming handling
		if (m_Swimming.HandleSwimming(pCurrentCommandID, hcm, m_MovementState))
		{
			m_JumpClimb.CheckAndFinishJump();
			return;			
		}
		
		//--------------------------------------------
		// ladder handling
		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_LADDER)
		{
			OnLadder(pDt, m_MovementState);
			return;
		}

		//--------------------------------------------
		// climb handling
		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_CLIMB)
		{
			return;
		}

		//--------------------------------------------
		// fall handling

		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_FALL)
		{
			// vector	vel;
			// PhysicsGetVelocity(vel);
			// Print(vel);

			int landType = 0;
			HumanCommandFall fall = GetCommand_Fall();

			if (fall.PhysicsLanded())
			{
				DayZPlayerType type = GetDayZPlayerType();
				NoiseParams npar;

				// land
				m_FallYDiff = m_FallYDiff - GetPosition()[1];
				//Print(m_FallYDiff);
				if (m_FallYDiff < 0.5)
				{
					landType = HumanCommandFall.LANDTYPE_NONE; 
					fall.Land(landType);
					npar = type.GetNoiseParamsLandLight();
					AddNoise(npar);
				}
				else if (m_FallYDiff < 1.0)
				{
					landType = HumanCommandFall.LANDTYPE_LIGHT;
					fall.Land(landType);
					npar = type.GetNoiseParamsLandLight();
					AddNoise(npar);
				}
				else if (m_FallYDiff < 2.0)
				{
					landType = HumanCommandFall.LANDTYPE_MEDIUM;
					fall.Land(landType);
					npar = type.GetNoiseParamsLandHeavy();
					AddNoise(npar);
				}
				else
				{
					landType = HumanCommandFall.LANDTYPE_HEAVY;
					fall.Land(landType);
					npar = type.GetNoiseParamsLandHeavy();
					AddNoise(npar);
				}
				
				if( m_FallYDiff >= DayZPlayerImplementFallDamage.FD_DMG_FROM_HEIGHT && GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT )
				{
					this.SpawnDamageDealtEffect();
				}
				
				m_FallDamage.HandleFallDamage(m_FallYDiff);
				m_JumpClimb.CheckAndFinishJump(landType);
			}

			return;
		}

		// start falling ? 
		if (PhysicsIsFalling(false))
		{
			StartCommand_Fall(0);
			m_FallYDiff = GetPosition()[1];
			return;
		}

		//--------------------------------------------
		// damage hits
		if (HandleDamageHit(pCurrentCommandID))
		{
			return;
		}

		//--------------------------------------------
		// handle jumping



#ifndef NO_GUI
#ifdef DEVELOPER
		//! enable this later for everything

		if (DiagMenu.GetValue(DayZPlayerConstants.DEBUG_ENABLEJUMP))
		{
			SHumanCommandClimbSettings hcls = GetDayZPlayerType().CommandClimbSettingsW();
			
			if( m_MovementState.m_iMovement != DayZPlayerConstants.MOVEMENTIDX_IDLE )
				hcls.m_fFwMaxDistance = 3;
			else
				hcls.m_fFwMaxDistance = 1.2;

			SHumanCommandClimbResult 	ret;

			//! debug draw climb heuristics
			HumanCommandClimb.DoClimbTest(this, ret, 0x3);

		}

#endif
#endif 

		// start jump 
		if(hic.IsJumpClimb())
		{
			m_JumpClimb.JumpOrClimb();
			return;
		}

		HumanCommandAdditives ad = GetCommandModifier_Additives();
	
		bool force = false;
#ifndef NO_GUI
#ifdef DEVELOPER
		//! force speaking for debug
		force = DiagMenu.GetValue(DayZPlayerConstants.DEBUG_ENABLETALKING);
#endif
#endif 
		float amplitude = IsPlayerSpeaking();
		
		if ( amplitude > 0.1 || force )
		{	
			if ( ad )
				ad.SetTalking(true);
			
			// add noises on server
			if ( GetGame().IsServer() )
			{
				int now = GetGame().GetTime();
				if ( now >= m_NextVoNNoiseTime )
				{
					m_NextVoNNoiseTime = now + 1000;
					
					int voiceLevel = GetGame().GetVoiceLevel(this);
					DayZPlayerType pt = GetDayZPlayerType();
					
					// add noise every second while talking 
					NoiseParams vonpar;
					switch ( voiceLevel )
					{
						case VoiceLevelWhisper:
							vonpar = pt.GetNoiseParamsWhisper();
						break;
						case VoiceLevelTalk:
							vonpar = pt.GetNoiseParamsTalk();
						break;
						case VoiceLevelShout:
							vonpar = pt.GetNoiseParamsShout();
						break; 
					}
					AddNoise(vonpar);
				}
			}
		}
		else
		{
			if ( ad )
				ad.SetTalking(false);
		}
		
		//--------------------------------------------
		// anything whats handled by InputController

		//! 

		if (hic)
		{
			//! throwing handling
			HumanCommandWeapons hcw = GetCommandModifier_Weapons();
			m_Throwing.HandleThrowing(hic, hcw, entityInHands, pDt);
					
			//! melee
			if(m_MeleeFightLogic.HandleFightLogic(pCurrentCommandID, hic, entityInHands, m_MovementState, m_ContinueFirearmMelee))
			{
				m_ProcessFirearmMeleeHit = isWeapon && (hic.IsMeleeWeaponAttack() || m_ProcessFirearmMeleeHit) || m_ContinueFirearmMelee;
				return;
			}
		}


		//!
		if (ModCommandHandlerAfter(pDt, pCurrentCommandID, pCurrentCommandFinished))
		{
			return;
		}
	}




	void ExitSights()
	{
//		Print("ExitSights");
	
		ItemOptics optics = null;
		EntityAI entityInHands = GetHumanInventory().GetEntityInHands();
		Weapon weapon = Weapon.Cast(entityInHands);
		if (weapon)
		{
			optics = weapon.GetAttachedOptics();
		}
		else
		{
			optics = ItemOptics.Cast(entityInHands);
		}
		
		//Print("m_CameraOptics " + m_CameraOptics);
		if ( m_CameraOptics )
		{
//			m_ShouldReturnToOptics = true;
//			m_ShouldReturnToBackupIronsights = false;
//			m_ShouldReturnToRegularIronsights = false;
		}
		else if (m_CameraIronsight && !m_CameraOptics && optics && optics.IsUsingWeaponIronsightsOverride())
		{
//			m_ShouldReturnToOptics = false;
//			m_ShouldReturnToBackupIronsights = true;
//			m_ShouldReturnToRegularIronsights = false;
		}
		else
		{
//			m_ShouldReturnToOptics = false;
//			if ( !optics || (optics && !optics.IsUsingWeaponIronsightsOverride()) )
//				m_ShouldReturnToBackupIronsights = false;
//			m_ShouldReturnToRegularIronsights = true;
		}
		
		m_CameraIronsight = false;
		m_CameraOptics = false;
		
		if (optics)
		{
			SwitchOptics(optics,false);
		}
		
		HumanCommandWeapons hcw = GetCommandModifier_Weapons();
		if (hcw)
			hcw.SetADS(false);
	}
	
	void SwitchOptics(ItemOptics optic, bool state)
	{
//		Print("SwitchOptics");
//		Print(state);
	
		if (state)
		{
			m_CameraOptics = true;
			if (optic.HasEnergyManager())
				optic.GetCompEM().SwitchOn();
			optic.HideSelection("hide");
			optic.EnterOptics();
			//m_ForceHandleOptics = true;
		}
		else
		{
			optic.ShowSelection("hide");
			
			//HideClothing(optic,state);
			
			optic.ExitOptics();
			if (optic.HasEnergyManager())
				optic.GetCompEM().SwitchOff();
			m_CameraOptics = false;
		}
	}
	
	void HideClothing(ItemOptics optic, bool state) {}

	//-------------------------------------------------------------
	//!
	//! This is Debug functionality 
	//! 

	float 	m_TestDamageCounter 	= -1;	//! VKOSTIK - tady dat 0 misto -1
	float 	m_DebugDirectionVal 	= -180;
	bool	m_DebugTypeVal 			= false;

	int 		m_DebugWeaponChangeStage = 0;
	string	m_DebugWeaponChangeItem;
	int			m_DebugWeaponChangeShowSlot;

	//! 
	void  CommandHandlerDebug(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{

		//! 
		/*
		// debug - print ironsights pos / dir

		HumanItemAccessor hia = GetItemAccessor();

		vector pos, dir;

		if (hia.WeaponGetCameraPoint(pos, dir))
		{
			string 	a = "ITACC: ";
			a += pos.ToString();
			a += " ";
			a += dir.ToString();

		//Print("DayZPlayerImplement: Init");

			Print(a);
		}
		*/

		/* {
			HumanCommandMove mc = GetCommand_Move();
			if (mc)
			{

				float gcms = mc.GetCurrentMovementSpeed();
				Print(gcms);
			}
		} */

		//! 
		if (GetPluginManager())
		{
			PluginDayzPlayerDebug playerDebug = PluginDayzPlayerDebug.Cast(GetPluginManager().GetPluginByType(PluginDayzPlayerDebug));
			if (playerDebug)
			{
				playerDebug.CommandHandler();

				//! debug handling of adding commands 
				if (m_DebugWeaponChangeStage == 0)
				{

					// debug weapon change
					int 		hideSlot;

					if (playerDebug.IsWeaponChange(m_DebugWeaponChangeItem, hideSlot, m_DebugWeaponChangeShowSlot))
					{
						Print("Change Weapon started: " + m_DebugWeaponChangeItem + "," + hideSlot.ToString() + "," + m_DebugWeaponChangeShowSlot.ToString());

						HumanCommandWeapons	w = GetCommandModifier_Weapons();
						w.StartAction(WeaponActions.HIDE, hideSlot);

						m_DebugWeaponChangeStage = 1;
					}
				}
				else if (m_DebugWeaponChangeStage == 1)
				{
					// wait for weapon hide to meet event 
					HumanCommandWeapons	w2 = GetCommandModifier_Weapons();
					if (w2 && w2.GetRunningAction() == WeaponActions.HIDE)
					{
						if (w2.IsEvent() == WeaponEvents.CHANGE_HIDE)
						{
							//! set item in hands
							Print("Change Weapon - hidden: ");

							//! fuj
							PlayerBase player = PlayerBase.Cast(this);
							EntityAI item_in_hands = player.GetHumanInventory().GetEntityInHands();
							if (item_in_hands != NULL && player.CanDropEntity(item_in_hands) && GetGame().GetPlayer().GetHumanInventory().CanRemoveEntityInHands())
							{
								player.PredictiveDropEntity(item_in_hands);
							}

							if (m_DebugWeaponChangeItem != "")
							{
								InventoryLocation dst = new InventoryLocation;
								dst.SetHands(this, NULL);
								EntityAI item = GetGame().SpawnEntity(m_DebugWeaponChangeItem, dst,ECE_IN_INVENTORY,RF_DEFAULT);
							}

							// LocalTakeEntityToHands(item, dst);

							//! start show
							w2.StartAction(WeaponActions.SHOW, 3);

							m_DebugWeaponChangeStage = 2;
						}
					}
				}
				else if (m_DebugWeaponChangeStage == 2)
				{
					HumanCommandWeapons	w3 = GetCommandModifier_Weapons();
					if (w3 && w3.IsActionFinished())
					{
						m_DebugWeaponChangeStage = 0;
					}
				}
			}
		}
	

		//! damage test
		if (m_TestDamageCounter >= 0)
		{
			// HumanCommandDamage damage = GetCommandModifier_Damage();
			if (m_TestDamageCounter > 3)
			{
				m_DebugDirectionVal = m_DebugDirectionVal + 90;
				m_TestDamageCounter = 0;

				if (m_DebugDirectionVal > 90)
				{
					m_DebugDirectionVal = -180;
					m_DebugTypeVal		= !m_DebugTypeVal;
				}

				if (m_DebugTypeVal)
				{
					AddCommandModifier_Damage(0, m_DebugDirectionVal); // starts light
				}
				else
				{
					StartCommand_Damage(0, m_DebugDirectionVal); // starts heavy (full body)
				}
				// 0.. types
				// from: -180 back, -90 left, 0 front, 90 right, 180 back 
				//AddCommandModifier_Damage(0, directionVal); // starts light
				//StartCommand_Damage(0, 0); // starts heavy (full body)
			}

			m_TestDamageCounter += pDt;
		}


#ifndef NO_GUI
#ifdef DEVELOPER

		// injury / exhaustion
		{
			HumanCommandAdditives ad = GetCommandModifier_Additives();

			//! diag menu - injured
			float v = DiagMenu.GetValue(DayZPlayerConstants.DEBUG_SHOWINJURY);
			if (v > 0)
			{
				v = (v - 1.0) * 0.1;
				ad.SetInjured(v, true);
			}

			//! diag menu - injured
			v = DiagMenu.GetValue(DayZPlayerConstants.DEBUG_SHOWEXHAUSTION);
			if (v > 0)
			{
				v = (v - 1) * 0.1;
				ad.SetExhaustion(v, true);
			}
		}
#endif
#endif 

	}



	//-------------------------------------------------------------
	//!
	//! This is main camera selection logic
	//! 
	override int 	CameraHandler(int pCameraMode)
	{
		// Print("DayZPlayerCfgBase: Camera Handler called");

		/* {
			int perItemCamUD = GetCurrentPerItemCameraUD();
			string 		a	= "Per Item Camera User Data: " + ToString(perItemCamUD);
			Print(a);
		} */

		//! ironsights
		if (!m_LiftWeapon_player)
		{
			if (m_CameraIronsight)
			{
				if (m_CameraOptics)
					return DayZPlayerCameras.DAYZCAMERA_OPTICS;
				else
					return DayZPlayerCameras.DAYZCAMERA_IRONSIGHTS;
			}
			else if (m_CameraOptics)
				return DayZPlayerCameras.DAYZCAMERA_OPTICS;
		}
		//ResetOpticsPP();
		
		HumanCommandVehicle vehicleCommand = GetCommand_Vehicle();
		
		if (!m_Camera3rdPerson)
		{
			if( vehicleCommand )
			{
				return DayZPlayerCameras.DAYZCAMERA_1ST_VEHICLE;
			}
			
			if( m_IsUnconscious )
			{
				return DayZPlayerCameras.DAYZCAMERA_1ST_UNCONSCIOUS;
			}
			return DayZPlayerCameras.DAYZCAMERA_1ST;
		}
		else // if (pCameraMode == DayZPlayerConstants.CAMERAMODE_EXTERNAL)
		{
			if( vehicleCommand )
			{
				Transport transport = vehicleCommand.GetTransport();
				return transport.Get3rdPersonCameraType();
			}
			
			//! get movement state 
			GetMovementState(m_MovementState);
			
			//! VKOSTIK: v ostatnich commandech mimo COMMANDID_MOVE je 
			//! m_MovementState.m_iStanceIdx - DayZPlayerConstants.STANCEIDX_ERECT
			//! m_MovementState.m_iMovement = 0 (idle)

			//! COMMANDID_ACTION returns stance right - ERECT/CROUCH

			//! melee camera - not melee - stays in stance camera

			/* if (m_MovementState.m_CommandTypeId == DayZPlayerConstants.COMMANDID_MELEE)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_ERC_RAISED;
			}*/

			if (m_MovementState.m_CommandTypeId == DayZPlayerConstants.COMMANDID_CLIMB)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_CLIMB;
			}

			if( m_JumpClimb.m_bIsJumpInProgress )
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_JUMP;
			}
			
			//		
			// normal movement cameras			
			if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_CRO;
			}

			else if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_CRO_RAISED;
			}

			if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE)
			{
				if( m_IsUnconscious )
				{
					return DayZPlayerCameras.DAYZCAMERA_3RD_ERC;
				}
				return DayZPlayerCameras.DAYZCAMERA_3RD_PRO;
			}
			else if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_PRO_RAISED;
			}

			if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT)
			{
				EntityAI entityInHands = GetHumanInventory().GetEntityInHands();
				bool isWeapon		= entityInHands	&& entityInHands.IsInherited(Weapon);

				if (isWeapon) 
				{
					return DayZPlayerCameras.DAYZCAMERA_3RD_ERC_RAISED;
				}
				else
				{
					return DayZPlayerCameras.DAYZCAMERA_3RD_ERC_RAISED_MELEE;
				}
			}
			else if (m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_SPRINT)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_ERC_SPR;
			}

			// DayZPlayerConstants.STANCEIDX_ERECT
			return DayZPlayerCameras.DAYZCAMERA_3RD_ERC;

			//
		}

		return DayZPlayerCameras.DAYZCAMERA_1ST;
	}


	//-------------------------------------------------------------
	//!
	//! Input userdata
	//! 

	void OnInputUserDataReceived(ParamsReadContext ctx)
	{
		int userDataTypeParam = 0;
		if (!ctx.Read(userDataTypeParam))
		{
			Error("DayZPlayerImplement: cannot read input type");
			return;
		}

		OnInputUserDataProcess(userDataTypeParam, ctx);
	}

	bool OnInputUserDataProcess(int userDataType, ParamsReadContext ctx)
	{
		if (userDataType == INPUT_UDT_MELEE_TARGET)
		{
			EntityAI target = null;
			if (ctx.Read(target))
			{
				m_MeleeCombat.SetTargetObject(target);
			}

			vector hitPos = vector.Zero;
			if (ctx.Read(hitPos))
			{
				m_MeleeCombat.SetHitPos(hitPos);
			}

			int hitZoneIdx = -1;
			if (ctx.Read(hitZoneIdx))
			{
				m_MeleeCombat.SetHitZoneIdx(hitZoneIdx);
			}
			return true;
		}

		return false;
	}

	//-------------------------------------------------------------
	//!
	//! SyncJunctures
	//! 

	void OnSyncJuncture(int pJunctureID, ParamsReadContext pCtx)
	{
		switch (pJunctureID)
		{
		case DayZPlayerSyncJunctures.SJ_DEATH:
			if( m_DeathAnimType == -2 )
				DayZPlayerSyncJunctures.ReadDeathParams(pCtx, m_DeathAnimType, m_DeathHitDir);
			break;
			
		case DayZPlayerSyncJunctures.SJ_DAMAGE_HIT:
			DayZPlayerSyncJunctures.ReadDamageHitParams(pCtx, m_DamageHitAnimType, m_DamageHitDir, m_DamageHitFullbody);
			break;
		case DayZPlayerSyncJunctures.SJ_WEAPON_RAISE_COMPLETED:
			m_ProcessWeaponRaiseCompleted = true;
			break;
		}
	}

	AnimBootsType GetBootsType()
	{
		EntityAI boots = GetInventory().FindAttachment(InventorySlots.FEET);
		if(boots)
		{
			switch( boots.GetAttachmentSoundType() )
			{
				case "Sneakers":
					return AnimBootsType.Sneakers;
				case "Boots":
					return AnimBootsType.Boots;
			}
		}

		return AnimBootsType.None;
	}
	
	//Get top body clothes
	AnimUpperBodyType GetBodyAttachmentType()
	{
		EntityAI attachment = GetInventory().FindAttachment(InventorySlots.BODY);
		if(attachment)
		{
			switch( attachment.GetAttachmentSoundType() )
			{
				case "NylonJacket":
					return AnimUpperBodyType.NylonJacket;
				case "TShirt":
					return AnimUpperBodyType.TShirt;
				case "WoolShirt":
					return AnimUpperBodyType.WoolShirt;
				case "HeavyJacket":
					return AnimUpperBodyType.HeavyJacket; 
				case "LeatherJacket":
					return AnimUpperBodyType.LeatherJacket;
				case "Coat":
					return AnimUpperBodyType.Coat;
				case "ChemlonDress":
					return AnimUpperBodyType.ChemlonDress;
				case "Ghillie":
					return AnimUpperBodyType.Ghillie;
			}
		}
		
		return AnimUpperBodyType.None;
	}
	
	AnimBackType GetBackAttachmentType()
	{
		EntityAI back = GetInventory().FindAttachment(InventorySlots.BACK);
		if(back)
		{
			switch( back.GetAttachmentSoundType() )
			{
				case "Small":
					return AnimBackType.Small;
				case "Military":
					return AnimBackType.Military;
				case "Outdoor":
					return AnimBackType.Outdoor;
				case "Ghillie":
					return AnimBackType.Ghillie; 
			}
		}
		
		return AnimBackType.None;
	}
	
	//Get weapon on back
	AnimRangedWeaponType GetShoulderAttachmentType()
	{
		EntityAI shoulderAttachment = GetInventory().FindAttachment(InventorySlots.SHOULDER);
		EntityAI meleeAttachment = GetInventory().FindAttachment(InventorySlots.MELEE);
		
		AnimRangedWeaponType shoulderAttType = AnimRangedWeaponType.None;
		AnimRangedWeaponType meleeAttType = AnimRangedWeaponType.None;
		
		if(shoulderAttachment)
		{
			switch( shoulderAttachment.GetAttachmentSoundType())
			{
				case "Shotgun":
				{
					shoulderAttType = AnimRangedWeaponType.Shotgun;
					break;
				}	
				case "Rifle":
				{
					shoulderAttType = AnimRangedWeaponType.Rifle;
					break;
				}
			}
		}
		if(meleeAttachment)
		{
			switch( meleeAttachment.GetAttachmentSoundType())
			{
				case "Shotgun":
				{
					meleeAttType = AnimRangedWeaponType.Shotgun;
					break;
				}
				case "Rifle":
				{
					meleeAttType = AnimRangedWeaponType.Rifle;
					break;
				}
			}
		}
		
		if(shoulderAttType == AnimRangedWeaponType.Shotgun || meleeAttType == AnimRangedWeaponType.Shotgun)
			return AnimRangedWeaponType.Shotgun;
		
		if(shoulderAttType == AnimRangedWeaponType.Rifle || meleeAttType == AnimRangedWeaponType.Rifle)
			return AnimRangedWeaponType.Rifle;

		return AnimRangedWeaponType.None;
	}
	
	string GetSurfaceType(SurfaceAnimationBone limbType)
	{
		string surfaceType;
		int liquidType;
		
		GetGame().SurfaceUnderObjectByBone(this, limbType, surfaceType, liquidType);
		
		return surfaceType;
	}

	//-------------------------------------------------------------
	//!
	//! Sounds
	//! 

	AbstractWave PlaySound(SoundObject so, SoundObjectBuilder sob)
	{
		if (so == NULL)
		{
			//Print("PlaySound: NULL argument");
			return NULL;
		}

		so.SetPosition(GetPosition());
		AbstractWave wave = GetGame().GetSoundScene().Play3D(so, sob);
		return wave;
	}

	void AddNoise(NoiseParams noisePar, float noiseMultiplier = 1.0)
	{
		if(noisePar != NULL)
			GetGame().GetNoiseSystem().AddNoise(this, noisePar, noiseMultiplier);
	}

	//-------------------------------------------------------------
	//!
	//! Event Processing
	//! 

	void OnStepEvent(string pEventType, string pUserString, int pUserInt)
	{
		/*
		Print(pEventType);
		Print(pUserString);
		Print(pUserInt);
		Print("-----------------------------");
		*/
		DayZPlayerType type = GetDayZPlayerType();
		ref HumanMovementState	state = new HumanMovementState();
		GetMovementState(state);

		/*
		if(pUserString == "walk")
			state.m_iMovement = DayZPlayerConstants.MOVEMENTIDX_WALK;
		else if(pUserString == "run")
			state.m_iMovement = DayZPlayerConstants.MOVEMENTIDX_RUN;
		else if(pUserString == "sprint")
			state.m_iMovement = DayZPlayerConstants.MOVEMENTIDX_SPRINT;
		
		if (state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
			state.m_iMovement--;
		*/
		
		if(GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			DayZPlayerTypeStepSoundLookupTable table = type.GetStepSoundLookupTable();
			SoundObjectBuilder soundBuilder = NULL;
			if(pUserInt < 100)
			{
				string surface;
				if(pUserInt % 2 == 1)
				{
					surface = GetSurfaceType(SurfaceAnimationBone.LeftBackLimb);
					if (surface.Length() == 0)//if no surface found, try other leg
						surface = GetSurfaceType(SurfaceAnimationBone.RightBackLimb);
				}
				else
				{
					surface = GetSurfaceType(SurfaceAnimationBone.RightBackLimb);
					if (surface.Length() == 0)//if no surface found, try other leg
						surface = GetSurfaceType(SurfaceAnimationBone.LeftBackLimb);
				}
				
				if (surface.Length() != 0)
					m_LastSurfaceUnderHash = surface.Hash();
					
				soundBuilder = table.GetSoundBuilder(pUserInt, state.m_iMovement, m_LastSurfaceUnderHash, GetBootsType());
			}
			else
			{
				string surface2 = GetSurfaceType();
				if (surface2.Length() != 0)
					m_LastSurfaceUnderHash = surface2.Hash();
				
				soundBuilder = table.GetSoundBuilder(pUserInt, state.m_iMovement, m_LastSurfaceUnderHash, GetBootsType());
			}

			if (soundBuilder != NULL && GetGame().GetPlayer())
			{
				vector orientation = Vector(0, 0, 0);
				vector edgeLength = Vector(1.5, 3.0, 1.5);
				array<Object> excludedObjects = new array<Object>;
				array<Object> collidedObjects = new array<Object>;
				
				if (GetGame().IsBoxColliding(GetPosition(), orientation, edgeLength, excludedObjects, collidedObjects))
				{
					for (int i = 0; i < collidedObjects.Count(); ++i)
					{
						string objectClass = collidedObjects.Get(i).GetType();
						
						if (objectClass.Contains("BushSoft") || objectClass.Contains("BushHard"))
						{
							for (int j = 0; j < type.GetVegetationSounds().Count(); ++j)
							{
								VegetationSound vegetationSound = type.GetVegetationSounds().Get(j);
								
								if (vegetationSound.GetAnimEventIds().Find(pUserInt) >= 0)
								{
									SoundObjectBuilder vegSoundObjectBuilder = vegetationSound.GetSoundObjectBuilder();
									SoundObject vegSoundObject = vegetationSound.GetSoundObjectBuilder().BuildSoundObject();
									
									if (vegSoundObject != NULL)
									{
										AttenuateSoundIfNecessary(vegSoundObject);
										PlaySound(vegSoundObject, vegSoundObjectBuilder);
									}
									
									break;
								}
							}
							
							break;
						}
					}
				}
				
				SoundObject soundObject = soundBuilder.BuildSoundObject();
				if (soundObject != NULL)
				{
					AttenuateSoundIfNecessary(soundObject);
					PlaySound(soundObject, soundBuilder);
				}
			}
		}
		
		if(GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			m_StepCounter++;//move outside of server condition if needed on client as well
			float noiseMultiplier = 0;
			float speedNoiseMultiplier = 0;
			float bootsNoiseMultiplier = 0;
			float surfaceNoiseMultiplier = GetSurfaceNoise(); //! gets noise multiplayer base on surface player walks on
			
			ref HumanMovementState	hms = new HumanMovementState();

			//! noise multiplier based on player speed
			GetMovementState(hms);

			switch(AITargetCallbacksPlayer.StanceToMovementIdxTranslation(hms))
			{
				case DayZPlayerConstants.MOVEMENTIDX_SPRINT:
					speedNoiseMultiplier = 1.0;
					break;
				case DayZPlayerConstants.MOVEMENTIDX_RUN:
					speedNoiseMultiplier = 0.66;
					break;
				case DayZPlayerConstants.MOVEMENTIDX_WALK:
					speedNoiseMultiplier = 0.11;
					break;
			}

			//! noise multiplier based on type of boots
			switch(GetBootsType())
			{
				case AnimBootsType.Boots:
					bootsNoiseMultiplier = 1.0;
					break;
				case AnimBootsType.Sneakers:
					bootsNoiseMultiplier = 0.66;
					break;
				case AnimBootsType.None:
					bootsNoiseMultiplier = 0.0;
					break;
			}

			NoiseParams noiseParams;
			if (state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT)
				noiseParams = type.GetNoiseParamsStand();
			else if (state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH)
				noiseParams = type.GetNoiseParamsCrouch();
			else if (state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
				noiseParams = type.GetNoiseParamsProne();
			else
			{
				Print("OnStepEvent: wrong stance, id: " + state.m_iStanceIdx + "using backup with stand stance (id: 0)");
				noiseParams = type.GetNoiseParamsStand();
			}
			
			//! calcs noise multiplier from players speed, stance and surface under the player when not in idle
			if (hms.m_iMovement > DayZPlayerConstants.MOVEMENTIDX_IDLE)
			{
				noiseMultiplier = (speedNoiseMultiplier + bootsNoiseMultiplier + surfaceNoiseMultiplier) / 3;
			}
			if( (m_StepCounter % PlayerConstants.CHECK_EVERY_N_STEP) == 0 )
			{
				ProcessFeetDamageServer(pUserInt);
			}
			AddNoise(noiseParams, noiseMultiplier);
		}
	}

	void OnSoundEvent(string pEventType, string pUserString, int pUserInt)
	{
		if (pEventType == "Sound")
		{
			ProcessSoundEvent(pEventType, pUserString, pUserInt);
		}
		else if (pEventType == "SoundWeapon")
		{
			ProcessWeaponEvent(pEventType, pUserString, pUserInt);
		}
		else if (pEventType == "SoundAttachment")
		{
			ProcessAttachmentEvent(pEventType, pUserString, pUserInt);
		}
		else if (pEventType == "SoundVoice")
		{
			if( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT || GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_REMOTE )
			{
				int event_id = m_PlayerSoundEventHandler.ConvertAnimIDtoEventID(pUserInt);
				if( event_id > 0 )
				{
					PlaySoundEvent(event_id);
					return;
				}
			}
			ProcessVoiceEvent(pEventType, pUserString, pUserInt);
		}
		else
		{
			Print("OnSoundEvent: Unknown sound event \"" + pEventType + "\"");
		}
	}

	void OnParticleEvent(string pEventType, string pUserString, int pUserInt)
	{
		if( GetGame().IsClient() || !GetGame().IsMultiplayer() )
		{
			if ( pUserInt == 123456 ) // 123456 is ID for vomiting effect. The current implementation is WIP.
				{
				/*Print(pEventType);
				Print(pUserString);
				Print(pUserInt);*/
				
				PlayerBase player = PlayerBase.Cast(this);
				int boneIdx = player.GetBoneIndexByName("Head");
				
				if( boneIdx != -1 )
				{
					EffVomit eff = new EffVomit();
					vector player_pos = player.GetPosition();
					eff.SetDecalOwner( player );
					SEffectManager.PlayInWorld( eff, "0 0 0" );
					Particle p = eff.GetParticle();
					player.AddChild(p, boneIdx);
				}
			}
		}
	}

	
	void ProcessWeaponEvent(string pEventType, string pUserString, int pUserInt)
	{
		DayZPlayerType type = GetDayZPlayerType();
		AnimSoundEvent soundEvent = NULL;

		float quantity = 0;
		
		EntityAI entityInHands = GetHumanInventory().GetEntityInHands();
		if (entityInHands != NULL && entityInHands.IsInherited(ItemBase))
		{
			ItemBase item;
			Class.CastTo(item, entityInHands);
			if(item.HasQuantity())
				quantity = (float)item.GetQuantity() / (item.GetQuantityMax() - item.GetQuantityMin());
			InventoryItemType invItemType = item.GetInventoryItemType();
			soundEvent = invItemType.GetSoundEvent(pUserInt);
		}

		if (soundEvent == NULL)
		{
			soundEvent = type.GetSoundWeaponEvent(pUserInt);
		}

		if (soundEvent != NULL)
		{
			if(GetGame().IsClient() || !GetGame().IsMultiplayer())
			{
				SoundObjectBuilder builder = soundEvent.GetSoundBuilder();
				builder.SetVariable("quantity", quantity);
				builder.SetVariable("interior", IsSoundInsideBuilding());
				
				SoundObject soundObject = builder.BuildSoundObject();
				if (soundObject != NULL)
				{
					AttenuateSoundIfNecessary(soundObject);
					PlaySound(soundObject, builder);
				}
			}
			
			if(GetGame().IsServer() || !GetGame().IsMultiplayer())
			{
				AddNoise(soundEvent.m_NoiseParams);
			}
		}
		else
		{
			//commented out, some events in animation should be processed only when some items are in hand
			//string className;
			//if (entityInHands != NULL)
			//	className = entityInHands.GetType();
			//else
			//	className = "SurvivorBase";
			//Print("OnSoundEvent: eventType: \"SoundWeapon\", Not defined event id: " + pUserInt + ", in class \"" + className + "\"");
		}
	}

	void ProcessAttachmentEvent(string pEventType, string pUserString, int pUserInt)
	{
		DayZPlayerType type = GetDayZPlayerType();
		DayZPlayerTypeAttachmentSoundLookupTable table = type.GetAttachmentSoundLookupTable();
			
		array<string> attachments = new array<string>();
		pUserString.Split(",", attachments);
		for(int i = 0; i < attachments.Count(); i++)
		{
			int attachmentHash = -1;
			if( attachments[i] == "shoulder" )
				attachmentHash = GetShoulderAttachmentType();
			else if( attachments[i] == "body" )
				attachmentHash = GetBodyAttachmentType();
			else if( attachments[i] == "back" )
				attachmentHash = GetBackAttachmentType();
			
			SoundObjectBuilder soundBuilder = table.GetSoundBuilder(pUserInt, attachments[i], attachmentHash);
			
			if (soundBuilder != NULL)
			{
				SoundObject soundObject = soundBuilder.BuildSoundObject();
				if (soundObject != NULL)
				{
					AttenuateSoundIfNecessary(soundObject);
					PlaySound(soundObject, soundBuilder);
				}
			}
		}
	}

	void SetVariablesLadderSoundObjectBuilder(SoundObjectBuilder soundObjectBuilder)
	{
		if (m_ClimbingLadderType == "wood")
		{
			soundObjectBuilder.SetVariable("laddertype", 1);
		}
		else
		{
			soundObjectBuilder.SetVariable("laddertype", 0);
		}
		
		AnimBootsType pBoots = GetBootsType();
		
		if (pBoots == AnimBootsType.None)
		{
			soundObjectBuilder.SetVariable("bare", 1);
			soundObjectBuilder.SetVariable("sneakers", 0);
			soundObjectBuilder.SetVariable("boots", 0);
		}
		else if (pBoots == AnimBootsType.Sneakers)
		{
			soundObjectBuilder.SetVariable("bare", 0);
			soundObjectBuilder.SetVariable("sneakers", 1);
			soundObjectBuilder.SetVariable("boots", 0);
		}
		else if (pBoots == AnimBootsType.Boots)
		{
			soundObjectBuilder.SetVariable("bare", 0);
			soundObjectBuilder.SetVariable("sneakers", 0);
			soundObjectBuilder.SetVariable("boots", 1);
		}
	}

	void ProcessSoundEvent(string pEventType, string pUserString, int pUserInt)
	{
		DayZPlayerType type = GetDayZPlayerType();
		DayZPlayerTypeAnimTable table = type.GetSoundTable();
		AnimSoundEvent soundEvent;
		if( table )
		{
			soundEvent = table.GetSoundEvent(pUserInt);
		}
		
		if(soundEvent != NULL)
		{
			if(GetGame().IsClient() || !GetGame().IsMultiplayer())
			{
				SoundObjectBuilder objectBuilder = soundEvent.GetSoundBuilder();
				if (GetCommand_Ladder())
				{
					SetVariablesLadderSoundObjectBuilder(objectBuilder);
				}
				objectBuilder.UpdateEnvSoundControllers(GetPosition());

				SoundObject soundObject = objectBuilder.BuildSoundObject();
				if (soundObject != NULL)
				{
					AttenuateSoundIfNecessary(soundObject);
					PlaySound(soundObject, objectBuilder);
				}
			}
			
			if(GetGame().IsServer() || !GetGame().IsMultiplayer())
			{
				if(soundEvent.m_NoiseParams != NULL)
					GetGame().GetNoiseSystem().AddNoise(this, soundEvent.m_NoiseParams);
			}
		}
	}
	
	

	AbstractWave ProcessVoiceEvent(string pEventType, string pUserString, int pUserInt)
	{
		DayZPlayerType type = GetDayZPlayerType();
		DayZPlayerTypeVoiceSoundLookupTable table = type.GetVoiceSoundLookupTable();
		if(!table)
			return null;
		AbstractWave wave;
		EntityAI mask = GetInventory().FindAttachment(InventorySlots.MASK);
		EntityAI head_gear = GetInventory().FindAttachment(InventorySlots.HEADGEAR);
		
		string category;
		if( mask || head_gear )
		{
			string category_mask;
			string category_headgear;
			
			int priority_mask;
			int priority_headgear;
			
			if(mask)
			{
				category_mask = mask.ConfigGetString("soundVoiceType");
				priority_mask = mask.ConfigGetInt("soundVoicePriority");
			}
			if(head_gear)
			{
				category_headgear = head_gear.ConfigGetString("soundVoiceType");
				priority_headgear = head_gear.ConfigGetInt("soundVoicePriority");
			}
			
			if(priority_headgear >= priority_mask && category_headgear != "")
			{
				category = category_headgear;
			}
			else
			{
				category = category_mask;
			}
		}

		if(category == "")
		{
			category = "none";
		}
		
		if(GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			SoundObjectBuilder soundBuilder = table.GetSoundBuilder(pUserInt, category.Hash());
			
			if (soundBuilder)
			{
				PlayerBase player = PlayerBase.Cast(this);
				int maleVoiceType = 0;
				int femaleVoiceType = 0;
				if(player.IsMale())
				{
					maleVoiceType = player.GetVoiceType();
				}
				else
				{
					femaleVoiceType = player.GetVoiceType();
				}

				soundBuilder.SetVariable("male", maleVoiceType);
				soundBuilder.SetVariable("female", femaleVoiceType);
				
				SoundObject soundObject = soundBuilder.BuildSoundObject();
				if (soundObject != NULL)
				{
					AttenuateSoundIfNecessary(soundObject);
					wave = PlaySound(soundObject, soundBuilder);
				}
			}
		}
		
		/*
		if(GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			NoiseParams noise_params = table.GetNoiseParams(pUserInt);
			if(noise_params != NULL)
					GetGame().GetNoiseSystem().AddNoise(this, noise_params);
		}
		*/
		return wave;
	}
	
	void ProcessFeetDamageServer(int pUserInt);
	

	//-------------------------------------------------------------
	//!
	//! anti-cheat condition
	//! 

	bool CheckForTakeItem(EntityAI item)
	{
		bool restrained = IsRestrained(); // IsRestrained()
		bool unconscious = IsUnconscious(); // IsRestrained()

		ItemBase ib = ItemBase.Cast(item);
		if (!IsAlive() || restrained || unconscious || ib.CanBeMovedOverride())
			return true;
		return false;
	}
	
	bool CheckForDropItem(EntityAI item)
	{
		/*
		bool restrained = IsRestrained(); // IsRestrained()
		bool unconscious = IsUnconscious(); // IsRestrained()
		
		ItemBase ib = ItemBase.Cast(item);
		if (!IsAlive() || restrained || unconscious || ib.CanBeMovedOverride())
			return true;
		return false;
		*/
		return CheckForTakeItem(item); //the two functions are currently identical
	}
	
	//-------------------------------------------------------------
	//!
	//! Player respawn
	//! 
	bool CheckForRespawn(EntityAI item)
	{
		PlayerBase player = PlayerBase.Cast(this);
		if (!player)
			return true;
		
		if (!IsAlive() || player.IsUnconscious() /*|| IsRestrained()*/)
			return true;
		return false;
	}

#ifdef DEVELOPER

	// -------------------------------------------------------------------------
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		if (rpc_type == ERPCs.RPC_DAYZPLAYER_DEBUGSERVERWALK)
		{
			Print("ERPCs.RPC_DAYZPLAYER_DEBUGSERVERWALK");

			Param1<bool> rp = new Param1<bool>(false);

			ctx.Read(rp);

			GetInputController().OverrideMovementSpeed(rp.param1, 1);
			GetInputController().OverrideAimChangeX(rp.param1, 0.01);

		}
	}

#endif
	
	//! movement sliding override, originally for FB gestures
	void OverrideSlidePoseAngle(float value)
	{
		if (!m_MoveSettings)
			m_MoveSettings = GetDayZPlayerType().CommandMoveSettingsW();
		if (m_MoveSettings.m_fSlidingPoseAngle != value)
		{
			//Print("Change from: " + m_MoveSettings.m_fSlidingPoseAngle);
			//Print("Change to: " + value);
			m_MoveSettings.m_fSlidingPoseAngle = value;
			StartCommand_Move(); //nescessary, re-initializes with adjusted values
		}
	}
	
	float GetSlidePoseAngle()
	{
		return GetDayZPlayerType().CommandMoveSettingsW().m_fSlidingPoseAngle;
	}
	
	void CheckAnimationOverrides()
	{
	}
	
	bool IsPlayerSelected()
	{
		return m_PlayerSelected;
	}
	
	bool IsRestrained();
	
	//! Checks if fullbody animation or specific locked state is active in emote manager
	bool IsInFBEmoteState();
	
	void SetSuicide(bool state)
	{
		m_Suicide = state;
	}
	
	bool CommitedSuicide()
	{
		return m_Suicide;
	}
	
	void SetReturnToOptics(bool state)
	{
//		m_ShouldReturnToOptics = state;
//		m_WasIronsight = !state;
	}	
	
	
	//!called every command handler tick when player is on ladder
	void OnLadder(float delta_time, HumanMovementState pState);
	
	//-------------------------------------------------------------
	//!
	//! Phx contact event
	//! 
	
	override private void EOnContact(IEntity other, Contact extra)
	{
		if( !IsAlive() )
			return;
		
		if( GetParent() == other )
			return;

		Transport transport = Transport.Cast(other);
		if( transport )
		{
			if ( GetGame().IsServer() || !GetGame().IsMultiplayer() )
			{
				RegisterTransportHit(transport);
			}
		}
	}
	
	protected void AttenuateSoundIfNecessary(SoundObject soundObject)
	{
		if (GetGame().GetPlayer() != NULL && (IsSoundInsideBuilding() != GetGame().GetPlayer().IsSoundInsideBuilding() || IsCameraInsideVehicle() != GetGame().GetPlayer().IsCameraInsideVehicle()))
		{
			soundObject.SetKind(WaveKind.WAVEATTALWAYS);
		}
		else
		{
			soundObject.SetKind(WaveKind.WAVEEFFECTEX);
		}
	}
	
	void CheckLiftWeapon() {};
	void ProcessLiftWeapon() {};
	
	void SetHandheldOpticsInUse(bool state)
	{
		m_HandheldOpticsInUse = state;
	}
	
	bool IsHandheldOpticsInUse()
	{
		return m_HandheldOpticsInUse;
	}
	
	bool IsLiftWeapon();
	bool IsRaised();
	bool IsFighting();
	/*bool ProcessInputDown(string input_name)
	{
		if (input_name == m_input_name)
		{
			//m_input_name = "";
			m_input_process_successful = true;
			return true;
		}
		return false;
	}
	
	bool m_input_process_successful;
	string m_input_name = "";*/
	
	//! ---------------- Checks if player can pick up heavy item -------------------------
	// Will return true when the item is not heavy as well
	bool CanPickupHeavyItem(notnull EntityAI item)
	{
		return !item.IsHeavyBehaviour() || (item.IsHeavyBehaviour() && DayZPlayerUtils.PlayerCanChangeStance(this, DayZPlayerConstants.STANCEIDX_ERECT));
	}
	
	bool CanPickupHeavyItemSwap(notnull EntityAI item1, notnull EntityAI item2)
	{
		return CanPickupHeavyItem(item1) && CanPickupHeavyItem(item2);
	}
}
