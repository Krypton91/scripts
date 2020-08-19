class ActionTurnOnTransmitterOnGround: ActionInteractBase
{
	void ActionTurnOnTransmitterOnGround()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
	}

	override string GetText()
	{
		return "#turn_on";
	}
		
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object target_object = target.GetObject();
		if ( player && target_object && target_object.IsStaticTransmitter() )
		{
			EntityAI target_entity = EntityAI.Cast( target_object );
			string selection = target_object.GetActionComponentName( target.GetComponentIndex() );
			
			if ( target_entity.HasEnergyManager() && target_entity.GetCompEM().CanSwitchOn() && selection == "power_panel" )
			{
				AdvancedCommunication transmitter = AdvancedCommunication.Cast( target_object );
				//transmitter.DisplayRadioInfo( "OFF", player );
				
				return true;
			}
		}
		
		return false;
	}
		
	override void OnExecuteServer( ActionData action_data )
	{
		AdvancedCommunication transmitter = AdvancedCommunication.Cast( action_data.m_Target.GetObject() );
		transmitter.TurnOnTransmitter();
	}
}