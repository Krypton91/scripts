class ActionGetInTransport: ActionBase
{
	void ActionGetInTransport()
	{
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		//m_HUDCursorIcon = "GetInDriver";
	}


	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}

	override string GetText()
	{
		return "#get_in_vehicle";
	}

	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}
	
	override bool HasProgress()
	{
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
 		Transport trans = null;
		int crew_index = -1;

		if ( !target )
			return false;

		if ( !Class.CastTo(trans, target.GetObject()) )
			return false;

		if ( player.GetCommand_Vehicle() )
			return false;

		int componentIndex = target.GetComponentIndex();
		
		crew_index = trans.CrewPositionIndex(componentIndex);
		if ( crew_index < 0 )
			return false;

		Human crew = trans.CrewMember( crew_index );
		if ( crew )
			return false;
		
		if ( !trans.CrewCanGetThrough( crew_index ) || !trans.IsAreaAtDoorFree( crew_index ) )
			return false;
		
		array<string> selections = new array<string>();

		trans.GetActionComponentNameList( componentIndex, selections );
		
		for ( int i = 0; i < selections.Count(); i++ )
		{
			if ( trans.CanReachSeatFromDoors(selections[i], player.GetPosition(), 1.0) )
				return true;
		}

		return false;
	}

	override void Start( ActionData action_data )
	{
		super.Start( action_data );
		
		Transport trans = Transport.Cast(action_data.m_Target.GetObject());
		int componentIndex = action_data.m_Target.GetComponentIndex();
		int crew_index = trans.CrewPositionIndex(componentIndex);
		
		
		int seat = trans.GetSeatAnimationType(crew_index);
		HumanCommandVehicle vehCommand = action_data.m_Player.StartCommand_Vehicle(trans, crew_index, seat);
		if( vehCommand )
		{
			vehCommand.SetVehicleType(trans.GetAnimInstance());
			action_data.m_Player.GetItemAccessor().HideItemInHands(true);
			
			GetDayZGame().GetBacklit().OnEnterCar();
			if ( action_data.m_Player.GetInventory() ) 
				action_data.m_Player.GetInventory().LockInventory(LOCK_FROM_SCRIPT);
		}
	}

	override bool CanBeUsedInRestrain()
	{
		return true;
	}
	
	override void OnUpdate(ActionData action_data)
	{

		if(action_data.m_State == UA_START)
		{
			if( !action_data.m_Player.GetCommand_Vehicle() || !action_data.m_Player.GetCommand_Vehicle().IsGettingIn() )
			{
				End(action_data);
			}
		}
	}
	
	override int GetActionCategory()
	{
		return AC_INTERACT;
	}
	
	override void OnEndClient( ActionData action_data )
	{
		if ( action_data.m_Player.GetInventory() ) 
				action_data.m_Player.GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
	}
	
	override void OnEndServer( ActionData action_data )
	{
		if ( action_data.m_Player.GetInventory() ) 
				action_data.m_Player.GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
	}
};
