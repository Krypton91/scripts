class ActionLockDoorsCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.LOCK);
	}
};

class ActionLockDoors: ActionContinuousBase
{
	void ActionLockDoors()
	{
		m_CallbackClass = ActionLockDoorsCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_HIGH;
	}
	
	override void CreateConditionComponents()
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTCursor;
	}
		
	override string GetText()
	{
		return "#lock_door";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if( !target ) return false;
		//if( IsDamageDestroyed(action_data.m_Target) ) return false;
		if( !IsBuilding(target) ) return false;
		//if( !IsInReach(player, target, UAMaxDistances.DEFAULT) ) return false;

		Building building;
		if( Class.CastTo(building, target.GetObject()) )
		{
			int doorIndex = building.GetDoorIndex(target.GetComponentIndex());
			if ( doorIndex != -1 )
				return (!building.IsDoorOpen(doorIndex) && !building.IsDoorLocked(doorIndex));
		}		
		return false;
	}

	protected void LockDoor(ActionTarget target)
	{
		Building building;

		if ( Class.CastTo(building, target.GetObject()) )
		{
			int doorIndex = building.GetDoorIndex(target.GetComponentIndex());
			if ( doorIndex != -1 )
			{
				building.LockDoor(doorIndex);
			}
		}
	}

	override void OnFinishProgressServer( ActionData action_data )
	{
		LockDoor(action_data.m_Target);
		
		//Damage the Lockpick
		float dmg = action_data.m_MainItem.GetMaxHealth() * 0.04; //Multiply max health by 'x' amount depending on number of usages wanted (0.04 = 25)
		
		action_data.m_Player.GetSoftSkillsManager().AddSpecialty( m_SpecialtyWeight );
		if (action_data.m_Player.GetSoftSkillsManager().GetSpecialtyLevel() >= 0)
		{
			action_data.m_MainItem.DecreaseHealth("", "", dmg);
			DebugPrint.Log("Damage without soft skill : " + dmg);
		}
		
		if (action_data.m_Player.GetStatSpecialty().Get() < 0)
		{
			dmg = action_data.m_MainItem.GetMaxHealth() * 0.025; //Multiply by 0.025 to get 40 uses out of pristine lockpick
			action_data.m_MainItem.DecreaseHealth("", "", dmg);
			DebugPrint.Log("Damage with soft skill : " + dmg);
		}	
	}
};