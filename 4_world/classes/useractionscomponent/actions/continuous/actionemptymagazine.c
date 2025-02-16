class EmptyMagazineActionData : ActionData
{
	Magazine m_magazinePile;
}
class ActionEmptyMagazineCB : ActionContinuousBaseCB
{	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousRepeat(UATimeSpent.MAG_EMPTY);
	}
	
	override void OnAnimationEvent(int pEventID)	
	{
		super.OnAnimationEvent( pEventID );
		
		if ( GetGame().IsClient() )
			return;
		
		switch (pEventID)
		{
			case UA_ANIM_EVENT:
				{
					EmptyMagazineActionData emActionData = EmptyMagazineActionData.Cast(m_ActionData);
		
					Magazine mag;
					Class.CastTo(mag, m_ActionData.m_MainItem);
					
					//float dmg;
					//string bulletType;
					
					/*int ammoCount = mag.GetAmmoCount();
					if(GetAmmoCount() > 0)
					{
						mag.Get
						mag.SetAmmoCount( ammoCount - 1 );
						array< Magazine > piles;
					
						if(!callback.m_magazinePile )
						{
							piles = MiscGameplayFunctions.CreateMagazinePilesFromBullet(bulletType,action_data.m_Player.GetPosition(),1,dmg);
							if( piles.Count() > 0 )
								callback.m_magazinePile = piles.Get(0);
						}
						else
						{
							if( callback.m_magazinePile.GetAmmoMax() > callback.m_magazinePile.GetAmmoCount() && (vector.Distance(action_data.m_Player.GetPosition(),callback.m_magazinePile.GetPosition()) < MAX_GROUND_PILE_DISTANCE ) )
							{
								callback.m_magazinePile.StoreCartridge(dmg,bulletType);
							}
							else
							{
								piles = MiscGameplayFunctions.CreateMagazinePilesFromBullet(bulletType,action_data.m_Player.GetPosition(),1,dmg);
								if( piles.Count() > 0 )
									callback.m_magazinePile = piles.Get(0);
							}	
						}
						
						
						
					}	*/
					
					
					
					//----Uncomment after AcquireCartridge will work
			
					float dmg;
					string ammoType;
						
					if ( !mag.ServerAcquireCartridge(dmg,ammoType) )
					{
						return;
					}	
					
					mag.ApplyManipulationDamage();
					
					string magazineTypeName;
				 	GetGame().ConfigGetText( "CfgAmmo " + ammoType + " spawnPileType" , magazineTypeName);
					
					if ( !DayZPlayerUtils.HandleStoreCartridge(m_ActionData.m_Player, NULL, -1, dmg, ammoType, magazineTypeName, false) )
					{
						array< Magazine > piles;
					
						if ( !emActionData.m_magazinePile )
						{
							piles = MiscGameplayFunctions.CreateMagazinePilesFromBullet(ammoType,m_ActionData.m_Player.GetPosition(),1,dmg);
							if ( piles.Count() > 0 )
								emActionData.m_magazinePile = piles.Get(0);
						}
						else
						{				
							if ( emActionData.m_magazinePile.GetAmmoMax() > emActionData.m_magazinePile.GetAmmoCount() && (vector.DistanceSq(m_ActionData.m_Player.GetPosition(), emActionData.m_magazinePile.GetPosition()) < (UAMaxDistances.DEFAULT * UAMaxDistances.DEFAULT) ) )
							{
								emActionData.m_magazinePile.ServerStoreCartridge(dmg,ammoType);
							}
							else
							{
								piles = MiscGameplayFunctions.CreateMagazinePilesFromBullet(ammoType, m_ActionData.m_Player.GetPosition(), 1, dmg);
								if ( piles.Count() > 0 )
									emActionData.m_magazinePile = piles.Get(0);
							}	
						}
						//emActionData.m_magazinePile.SetSynchDirty();
					}
					//mag.SetSynchDirty();
				}
				break;
			default:
				break;
		}
	}
};

class ActionEmptyMagazine: ActionContinuousBase
{
	void ActionEmptyMagazine()
	{
		m_CallbackClass = ActionEmptyMagazineCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EMPTYMAG;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_EMPTYMAG;

	}
	
	override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = NULL)
	{
		if ( super.SetupAction( player, target, item, action_data, extra_data))
		{
			EmptyMagazineActionData emActionData = EmptyMagazineActionData.Cast(action_data);
			emActionData.m_magazinePile = NULL;
			return true;
		}
		return false;
	}
	
	override ActionData CreateActionData()
	{
		EmptyMagazineActionData action_data = new EmptyMagazineActionData;
		return action_data;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	override bool HasProneException()
	{
		return true;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Magazine itm;
		return ( Class.CastTo(itm, item) && itm.GetAmmoCount() > 0 );
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "#empty_magazine";
	}
};