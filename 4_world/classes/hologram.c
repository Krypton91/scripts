class Hologram
{
	protected ItemBase 			m_Parent;
	protected EntityAI 			m_Projection;
	protected PlayerBase 		m_Player;
	protected ProjectionTrigger m_ProjectionTrigger;

	protected bool 				m_IsColliding;
	protected bool 				m_IsCollidingGPlot;
	protected bool 				m_IsSlope;
	protected bool 				m_IsCollidingPlayer;
	protected bool 				m_IsFloating;
	protected bool 				m_UpdatePosition;
	protected bool 				m_IsHidden;

	protected vector 			m_DefaultOrientation;
	protected vector			m_Rotation;
	protected vector			m_y_p_r_previous;
	protected vector 			m_ContactDir;
	protected const string 		ANIMATION_PLACING 				= "Placing";
	protected const string 		ANIMATION_INVENTORY 			= "Inventory";
	protected const string 		SELECTION_PLACING 				= "placing";
	protected const string 		SELECTION_INVENTORY 			= "inventory";

	protected const float 		SMALL_PROJECTION_RADIUS 		= 1;
	protected const float 		SMALL_PROJECTION_GROUND 		= 2;
	protected const float		DISTANCE_SMALL_PROJECTION		= 1;
	protected const float		LARGE_PROJECTION_DISTANCE_LIMIT	= 6;
	protected const float 		PROJECTION_TRANSITION_MIN		= 1;
	protected const float 		PROJECTION_TRANSITION_MAX		= 0.25;
	protected const float 		LOOKING_TO_SKY					= 0.75;
	static const float 			DEFAULT_MAX_PLACEMENT_HEIGHT_DIFF = 1.5;
	
	protected float 			m_SlopeTolerance;
	protected bool				m_AlignToTerrain;
	protected vector 			m_YawPitchRollLimit;
	protected int				m_ContactComponent;
	
	protected ref set<string> 	m_SelectionsToRefresh 			= new set<string>;
	
	// Watchtower correction variables	
	// These watchtower component names should be corrected when colliding with them as they are supposed to be "trigger boxes", not colliders
	static const protected ref array<string>	m_WatchtowerIgnoreComponentNames	= new array<string>;
	
	// These watchtower components are supposed to be trigger boxes, but should block placement on them (currently only the boxes above the stairs)
	static const protected ref array<string>	m_WatchtowerBlockedComponentNames	= new array<string>;

	/*
	Shape m_DebugPlugArrowLeftClose;
	Shape m_DebugPlugArrowRightClose;
	Shape m_DebugPlugArrowLeftFar;
	Shape m_DebugPlugArrowRightFar;
	*/
		
	void Hologram( PlayerBase player, vector pos, ItemBase item)
	{	
		m_Player = player;
		m_Parent = item;
		m_Projection = NULL;
		m_ProjectionTrigger = NULL;
		m_UpdatePosition = true;
		m_Rotation = "0 0 0";
		m_ContactComponent = -1;
		
		// If the static names are empty, generate the needed names
		// Refer to their definitions to see why these are required
		if (m_WatchtowerIgnoreComponentNames.Count() == 0)
		{
			string baseStringBegin = Watchtower.BASE_VIEW_NAME;
			string baseIgnoreStringEnd = Watchtower.BASE_WALL_NAME;
			int floors = Watchtower.MAX_WATCHTOWER_FLOORS;
			int walls = Watchtower.MAX_WATCHTOWER_WALLS;
			string compName;
			
			for (int i = 1; i < floors + 1; ++i)
			{
				compName = baseStringBegin + i.ToString();
				for (int j = 1; j < walls + 1; ++j)
					m_WatchtowerIgnoreComponentNames.Insert(compName + baseIgnoreStringEnd + j.ToString());
				
				if (i != 1)
					m_WatchtowerBlockedComponentNames.Insert(compName);
				else
					m_WatchtowerIgnoreComponentNames.Insert(compName);

			}
		}

		EntityAI projection_entity;
		
		if ( GetGame().IsMultiplayer() && GetGame().IsServer() )
		{	
			projection_entity = EntityAI.Cast( GetGame().CreateObjectEx( ProjectionBasedOnParent(), pos, ECE_PLACE_ON_SURFACE ) );
			SetProjectionEntity( projection_entity );
			SetAnimations();
		}
		else
		{
			projection_entity = EntityAI.Cast( GetGame().CreateObjectEx( ProjectionBasedOnParent(), pos, ECE_TRACE|ECE_LOCAL ) );
			SetProjectionEntity( projection_entity );
			SetAnimations();
			CreateTrigger();
		}
		//Print(ProjectionBasedOnParent());
		
		if ( !projection_entity.IsInherited(GardenBase) && ItemBase.Cast(projection_entity) ) // Garden plot is a special case
		{
			ItemBase.Cast(GetProjectionEntity()).SetIsHologram( true );
		}
		
		string config_path_slope = "CfgVehicles" + " " + GetProjectionEntity().GetType() + " " + "slopeTolerance";
		if ( GetGame().ConfigIsExisting( config_path_slope ) )
		{
			m_SlopeTolerance = GetGame().ConfigGetFloat( config_path_slope );
		}
		
		string config_path_align = "CfgVehicles" + " " + GetProjectionEntity().GetType() + " " + "alignHologramToTerain";
		if ( GetGame().ConfigIsExisting( config_path_align ) )
		{
			m_AlignToTerrain = GetGame().ConfigGetInt( config_path_align );
		}
		
		string config_path_w_p_r = "CfgVehicles" + " " + GetProjectionEntity().GetType() + " " + "yawPitchRollLimit";
		if ( GetGame().ConfigIsExisting( config_path_w_p_r ) )
		{
			m_YawPitchRollLimit = GetGame().ConfigGetVector( config_path_w_p_r );
		}
	}
	
	void ~Hologram()
	{
		if ( GetGame() )
		{
			if ( m_Projection )
			{
				GetGame().ObjectDelete( m_Projection );
			}
	
			if ( m_ProjectionTrigger )
			{
				GetGame().ObjectDelete( m_ProjectionTrigger );
			}
		}
	}
	
	void SetAnimations()
	{
		if ( m_Projection.HasAnimation( ANIMATION_PLACING ) )
		{
			m_Projection.SetAnimationPhase( ANIMATION_PLACING, 0 );
			SetSelectionToRefresh( SELECTION_PLACING );

			if ( m_Projection.HasAnimation( ANIMATION_INVENTORY ) )
			{
				m_Projection.SetAnimationPhase( ANIMATION_INVENTORY, 1 );
			}
		}
		else
		{
			UpdateSelections();
			SetSelectionToRefresh( SELECTION_INVENTORY );
		}
	}
	
	// Updates selections on hologram object so they reflect the state of the parent object's selections.
	void UpdateSelections()
	{
		string cfg_access = "CfgVehicles " + m_Projection.GetType() + " AnimationSources ";
		
		if ( GetGame().ConfigIsExisting(cfg_access) )
		{
			int cfg_access_count = g_Game.ConfigGetChildrenCount(cfg_access);

			for ( int i = 0; i < cfg_access_count; ++i )
			{
				string found_anim;
				GetGame().ConfigGetChildName(cfg_access, i, found_anim);
				
				float anim_phase = m_Parent.GetAnimationPhase(found_anim);
				m_Projection.SetAnimationPhase(found_anim, anim_phase);
			}
		}
	}

	string ProjectionBasedOnParent()
	{
		return GetProjectionName(m_Parent);
	}
	
	string GetProjectionName(ItemBase item)
	{
		if( !item )
			return "";
		
		if ( item.CanMakeGardenplot() )
		{
			return "GardenPlot";
		}
		
		//Camping & Base building
		if ( item.IsInherited( TentBase ) || item.IsBasebuildingKit() )
		{
			return item.GetType() + "Placing";
		}
		
		return item.GetType();
	}
	
	static bool DoesHaveProjection(ItemBase item)
	{
		return item && (item.IsDeployable() || item.CanMakeGardenplot() || item.IsInherited(DeployableContainer_Base));
	}
	
	// update loop for visuals and collisions of the hologram
	void UpdateHologram( float timeslice )
	{
		if ( !m_Parent )
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(m_Player.TogglePlacingLocal);
			
			return;
		}
		
		if ( m_Player.IsSwimming() || m_Player.IsClimbingLadder() || m_Player.IsRaised() || m_Player.IsClimbing() || m_Player.IsRestrained() || m_Player.IsUnconscious() )
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(m_Player.TogglePlacingLocal);
			
			return;
		}
		
		EvaluateCollision();
		RefreshTrigger();
		CheckPowerSource();	
		RefreshVisual();

		if ( !GetUpdatePosition() )
		{
			return;
		} 

		// update hologram position	
		SetProjectionPosition( GetProjectionEntityPosition( m_Player ) );
		SetProjectionOrientation( AlignProjectionOnTerrain( timeslice ) );		
		m_Projection.OnHologramBeingPlaced( m_Player );
	}
	
	vector AlignProjectionOnTerrain( float timeslice )
	{
		vector y_p_r;
		
		if ( m_AlignToTerrain )
		{
			vector projection_orientation_angles = GetDefaultOrientation() + GetProjectionRotation();	
			vector mat0[3];
			vector mat1[3];
			vector mat2[3];
			vector projection_position = m_Projection.GetPosition();
			vector normal;
			
			if ( m_ContactDir.Length() > 0 )
			{
				normal = m_ContactDir;
			} 
			else
			{
				normal = GetGame().SurfaceGetNormal( projection_position[0], projection_position[2] );
			}
			
			vector angles = normal.VectorToAngles();
			angles[1] = angles[1] + 270;
					
			angles[0] = Math.Clamp( angles[0], 0, 360 ); 
			angles[1] = Math.Clamp( angles[1], 0, 360 );
			angles[2] = Math.Clamp( angles[2], 0, 360 );
				
			projection_orientation_angles[0] = projection_orientation_angles[0] + ( 360 - angles[0] );
			
			Math3D.YawPitchRollMatrix( projection_orientation_angles, mat0 );
			Math3D.YawPitchRollMatrix( angles, mat1 );
			Math3D.MatrixMultiply3( mat1, mat0, mat2 );	
			
			y_p_r = Math3D.MatrixToAngles( mat2 );
		}
		else
		{
			y_p_r = GetDefaultOrientation() + GetProjectionRotation();
			
			if ( y_p_r[0] > 180 )
			{
				y_p_r[0] = y_p_r[0] - 360;
			}
			
			if ( y_p_r[0] < -180 )
			{
				y_p_r[0] = y_p_r[0] + 360;
			}
		}	
		
		return SmoothProjectionMovement( y_p_r, timeslice );
	}
	
	vector SmoothProjectionMovement( vector y_p_r, float timeslice )
	{	
		if ( m_y_p_r_previous )
		{					
			if ( Math.AbsFloat( y_p_r[0] - m_y_p_r_previous[0] ) > 100 )
			{												
				if ( y_p_r[0] > 0 )
				{		
					m_y_p_r_previous[0] = m_y_p_r_previous[0] + 360; 
				}
				
				if ( y_p_r[0] < 0 )
				{		
					m_y_p_r_previous[0] = m_y_p_r_previous[0] - 360;
				}
			}
			
			y_p_r[0] = Math.Lerp( m_y_p_r_previous[0], y_p_r[0], 15 * timeslice );
			y_p_r[1] = Math.Lerp( m_y_p_r_previous[1], y_p_r[1], 15 * timeslice );
			y_p_r[2] = Math.Lerp( m_y_p_r_previous[2], y_p_r[2], 15 * timeslice );
		}
		
		m_y_p_r_previous = y_p_r;
		
		return y_p_r;
	}
	
	void CreateTrigger()
	{
		Class.CastTo(m_ProjectionTrigger,  g_Game.CreateObject( "ProjectionTrigger", GetProjectionPosition(), true ) );

		m_ProjectionTrigger.SetOrientation( GetProjectionOrientation() );
		m_ProjectionTrigger.SetParentObject( this );
		m_ProjectionTrigger.SetParentOwner( m_Player );
	}

	void RefreshTrigger()
	{
		vector min_max[2];
		GetProjectionCollisionBox( min_max );

		m_ProjectionTrigger.SetPosition( GetProjectionPosition() );
		m_ProjectionTrigger.SetOrientation( GetProjectionOrientation() );
		m_ProjectionTrigger.SetExtents(min_max[0], min_max[1]);
	}

	void EvaluateCollision(ItemBase action_item = null)
	{
		if ( IsHidden() || IsCollidingBBox(action_item) || IsCollidingPlayer() || IsClippingRoof() || !IsBaseViable() || IsCollidingGPlot() || IsCollidingZeroPos() || IsCollidingAngle() || !IsPlacementPermitted() || !HeightPlacementCheck() || IsUnderwater() || IsInTerrain() )
		{
			SetIsColliding( true );
		}
		else if ( m_Projection.IsInherited( TrapSpawnBase ))
		{
			TrapSpawnBase trap_spawn_base;
			Class.CastTo( trap_spawn_base,  m_Projection );
			SetIsColliding( !trap_spawn_base.IsPlaceableAtPosition( m_Projection.GetPosition() ) );
		}
		else if ( m_Projection.IsInherited( TrapBase ))
		{
			TrapBase trap_base;
			Class.CastTo(trap_base,  m_Projection);
			SetIsColliding( !trap_base.IsPlaceableAtPosition( m_Projection.GetPosition() ) );
		}
		else
		{
			SetIsColliding( IsSurfaceWater( m_Projection.GetPosition() ) );
		}
	}
	
	bool IsClippingRoof()
	{
		//Some locations allow you to look up and attempt placing the hologram on the bottom side of a floor - most notably the floors of a watchtower
		//This check also prevents some very unnatural placements
		if (!GetGame().IsServer())
		{
			bool b1 = m_Projection.GetPosition()[1] > GetGame().GetCurrentCameraPosition()[1];
			bool b2 = false;
			if (m_Projection.DoPlacingHeightCheck())
			{
				b2 = MiscGameplayFunctions.IsUnderRoof(m_Projection);
			}
		}
		else
		{
			return false;
		}
		return b1 || b2;
	}
	
	bool IsCollidingAngle()
	{
		vector projection_orientation = m_Projection.GetOrientation();
		
		return Math.AbsFloat( projection_orientation[1] ) > m_YawPitchRollLimit[1] || Math.AbsFloat( projection_orientation[2] ) > m_YawPitchRollLimit[2];
		
		/*
		if ( Math.AbsFloat( projection_orientation[1] ) > m_YawPitchRollLimit[1] || Math.AbsFloat(  projection_orientation[2] ) > m_YawPitchRollLimit[2] )
		{
			Print("IsCollidingAngle");
			return true;
		}
		else
		{
			return false;
		}
		*/
	}
	
	bool IsCollidingBBox(ItemBase action_item = null)
	{
		vector center;
		vector relative_ofset; //we need to lift BBox, because it is calculated from the bottom of projection, and not from the middle
		vector absolute_ofset = "0 0.01 0"; //we need to lift BBox even more, because it colliddes with house floors due to various reasons (probably geometry or float imperfections)
		vector orientation = GetProjectionOrientation();
		vector edge_length;
		vector min_max[2];
		ref array<Object> excluded_objects = new array<Object>;
		ref array<Object> collided_objects = new array<Object>;
		
		GetProjectionCollisionBox( min_max );
		relative_ofset[1] = ( min_max[1][1] - min_max[0][1] ) / 2;
		center = m_Projection.GetPosition() + relative_ofset + absolute_ofset;
		edge_length = GetCollisionBoxSize( min_max );
		excluded_objects.Insert( m_Projection );
		if (action_item)
		{
			excluded_objects.Insert( action_item );
		}

		//add is construction check
		// Base building objects behave in a way that causes this test to generate false positives
		return GetGame().IsBoxColliding( center, orientation, edge_length, excluded_objects, collided_objects ) && (!collided_objects[0].IsInherited(BaseBuildingBase) || IsFenceOrWatchtowerKit());
		
		/*
		if ( GetGame().IsBoxColliding( center, orientation, edge_length, excluded_objects, collided_objects ) && !collided_objects[0].IsInherited(BaseBuildingBase) )
		{	
			for( int i = 0; i < collided_objects.Count(); i++ )
			{
				Object obj_collided = collided_objects[i];
				Print("obj_collided with: " + obj_collided );
			}
								
			return true;	
		}
		return false;
		*/			
	}

	bool IsBaseViable()
	{	
		//This function is not required to solve server-side fixes for clipping, saves calculations and potential false negatives
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
			return true;
		
		/*
		if(m_DebugPlugArrowLeftClose)
		{
			m_DebugPlugArrowLeftClose.Destroy();
			m_DebugPlugArrowLeftClose = NULL; 

			m_DebugPlugArrowRightClose.Destroy();
			m_DebugPlugArrowRightClose = NULL; 

			m_DebugPlugArrowLeftFar.Destroy();
			m_DebugPlugArrowLeftFar = NULL; 

			m_DebugPlugArrowRightFar.Destroy();
			m_DebugPlugArrowRightFar = NULL;
		}
		*/
		
		vector from_left_close = m_Projection.CoordToParent( GetLeftCloseProjectionVector() );
		vector to_left_close_down = from_left_close + "0 -1 0";

		vector from_right_close = m_Projection.CoordToParent( GetRightCloseProjectionVector() );
		vector to_right_close_down = from_right_close + "0 -1 0";

		vector from_left_far = m_Projection.CoordToParent( GetLeftFarProjectionVector() );
		vector to_left_far_down = from_left_far + "0 -1 0";

		vector from_right_far = m_Projection.CoordToParent( GetRightFarProjectionVector() );
		vector to_right_far_down = from_right_far + "0 -1 0";
		
		/*
		m_DebugPlugArrowLeftClose = DrawArrow( from_left_close, to_left_close_down );
		m_DebugPlugArrowRightClose = DrawArrow( from_right_close, to_right_close_down );
		m_DebugPlugArrowLeftFar = DrawArrow( from_left_far, to_left_far_down );
		m_DebugPlugArrowRightFar = DrawArrow( from_right_far, to_right_far_down );
		*/

		vector contact_pos_left_close;
		vector contact_pos_right_close;
		vector contact_pos_left_far;
		vector contact_pos_right_far;
		vector contact_dir_left_close;
		vector contact_dir_right_close;
		vector contact_dir_left_far;
		vector contact_dir_right_far;
		int contact_component_left_close;
		int contact_component_right_close;
		int contact_component_left_far;
		int contact_component_right_far;
		set<Object> results_left_close = new set<Object>;
		set<Object> results_right_close = new set<Object>;
		set<Object> results_left_far = new set<Object>;
		set<Object> results_right_far = new set<Object>;
		Object obj_left_close;
		Object obj_right_close;
		Object obj_left_far;
		Object obj_right_far;

		//Not sure what the intention here was before, but it boiled down to a very bloated version of what you see here right now
		DayZPhysics.RaycastRV( from_left_close, to_left_close_down, contact_pos_left_close, contact_dir_left_close, contact_component_left_close, results_left_close, m_Projection, m_Projection, false, false );
		if (results_left_close.Count() > 0)
			obj_left_close = results_left_close[results_left_close.Count() - 1];

		DayZPhysics.RaycastRV( from_right_close, to_right_close_down, contact_pos_right_close, contact_dir_right_close, contact_component_right_close, results_right_close,m_Projection, m_Projection, false, false );
		if (results_right_close.Count() > 0)	
			obj_right_close = results_right_close[results_right_close.Count() - 1];

		DayZPhysics.RaycastRV( from_left_far, to_left_far_down, contact_pos_left_far, contact_dir_left_far, contact_component_left_far, results_left_far, m_Projection, m_Projection, false, false );
		if (results_left_far.Count() > 0) 
			obj_left_far = results_left_far[results_left_far.Count() - 1];

		DayZPhysics.RaycastRV( from_right_far, to_right_far_down, contact_pos_right_far, contact_dir_right_far, contact_component_right_far, results_right_far, m_Projection, m_Projection, false, false );
		if (results_right_far.Count() > 0)
			obj_right_far = results_right_far[results_right_far.Count() - 1];
		
		return IsBaseIntact( obj_left_close, obj_right_close, obj_left_far, obj_right_far ) && IsBaseStatic( obj_left_close ) &&  IsBaseFlat( contact_pos_left_close, contact_pos_right_close, contact_pos_left_far, contact_pos_right_far );
	}

	bool IsCollidingGPlot()
	{
		return m_IsCollidingGPlot;
		/*
		if ( m_IsCollidingGPlot )
		{
			Print("NOT suitable terrain for garden plot");
			return true; 
		}
		else
		{
			Print("Suitable terrain for garden plot");
			return false;
		}
		*/
	}

	bool IsCollidingZeroPos()
	{
		vector origin = Vector(0, 0, 0);
		return GetProjectionPosition() == origin;
		
		/*
		if ( GetProjectionPosition() == origin )
		{
			Print("NOT able to place, projection in origin");
			return true; 
		}
		else
		{
			Print("Projection not in origin");
			return false;
		}
		*/
	}
	
	//This check is currently not in use
	bool IsBehindObstacle()
	{			
		vector starting_pos = GetParentEntity().GetPosition();
		vector headingDirection = MiscGameplayFunctions.GetHeadingVector(m_Player);
		array<Object> coneObjects = new array<Object>;
		vector c_MaxTargetDistance = GetProjectionPosition() - m_Player.GetPosition();
		float c_ConeAngle = c_MaxTargetDistance.Length() * 5;
	 	float c_ConeHeightMin = 0.25;
	 	float c_ConeHeightMax = 1;
		Building building;
		
		//DrawDebugCone( true, starting_pos, c_MaxTargetDistance.Length(), c_ConeAngle );
		
		DayZPlayerUtils.GetEntitiesInCone( starting_pos, headingDirection, c_ConeAngle, c_MaxTargetDistance.Length(), c_ConeHeightMin, c_ConeHeightMax, coneObjects );
		
		for ( int i = 0; i < coneObjects.Count(); i++ )
		{
			Object object = coneObjects.Get(i);
			//Print("coneObjects.Count(): " + coneObjects.Count());
			//Print("object: " + object.GetName());
			//Print("index: " + i);
					
			//Tere is always 1 object in the cone and i am not able to find out what it is, so skip the object.
			if ( coneObjects.Count() > 1 && i > 0 )
			{
				if ( object == m_Player )
				{
					//Print( "Mame playera" );
					continue;
				}
				
				if ( object == GetParentEntity() )
				{
					//Print( "Mame GetParentEntity" );
					continue;
				}
				
				if ( object == GetProjectionEntity() )
				{
					//Print( "Mame GetProjectionEntity" );
					continue;
				}
				
				if ( object.IsTree() )
				{
					//Print( "Mame strom" );
					continue;
				}
				
				if ( Class.CastTo( building, object ) )
				{
					//Print( "Mame budovu" );
					continue;
				}
				
				if ( object.IsInherited( CarTent ) )
				{
					//Print( "Mame Cartent" );
					continue;
				}
				
				//Print( "Mame obstacle" );
				return true;
			}
		}
		
		//Print("No obstacle in cone");
		return false;
	}
	/*	
	ref array<Shape> dbgConeShapes = new array<Shape>();
	
	private void DrawDebugCone(bool enabled, vector starting_pos, float MaxTargetDistance, float cone_angle )
	{
		// "cone" settings
		vector start, endL, endR;
		float playerAngle;
		float xL,xR,zL,zR;
		
		float c_MaxTargetDistance = MaxTargetDistance;
		float c_ConeAngle = cone_angle;
		
		if (enabled)
		{
			CleanupDebugShapes(dbgConeShapes);
			
			start = starting_pos;
			playerAngle = MiscGameplayFunctions.GetHeadingAngle(m_Player);
			
			// offset position of the shape in height
			start[1] = start[1] + 0.2;

			endL = start;
			endR = start;
			xL = c_MaxTargetDistance * Math.Cos(playerAngle + Math.PI_HALF + c_ConeAngle * Math.DEG2RAD); // x
			zL = c_MaxTargetDistance * Math.Sin(playerAngle + Math.PI_HALF + c_ConeAngle * Math.DEG2RAD); // z
			xR = c_MaxTargetDistance * Math.Cos(playerAngle + Math.PI_HALF - c_ConeAngle * Math.DEG2RAD); // x
			zR = c_MaxTargetDistance * Math.Sin(playerAngle + Math.PI_HALF - c_ConeAngle * Math.DEG2RAD); // z
			endL[0] = endL[0] + xL;
			endL[2] = endL[2] + zL;
			endR[0] = endR[0] + xR;
			endR[2] = endR[2] + zR;

			dbgConeShapes.Insert( Debug.DrawLine(start, endL, COLOR_BLUE) );
			dbgConeShapes.Insert( Debug.DrawLine(start, endR, COLOR_BLUE) ) ;
			dbgConeShapes.Insert( Debug.DrawLine(endL, endR, COLOR_BLUE) );
		}
	}
	
	private void CleanupDebugShapes(array<Shape> shapesArr)
	{
		for ( int it = 0; it < shapesArr.Count(); ++it )
		{
			Debug.RemoveShape( shapesArr[it] );
			shapesArr.Remove(it);
		}
	}
	*/
	
	//This function only takes one of the found objects since IsBaseIntact already verifies that all of them are either null or the same object
	bool IsBaseStatic( Object objectToCheck )
	{	
		//check if the object below hologram is dynamic object. Dynamic objects like barrels can be taken to hands 
		//and item which had been placed on top of them, would stay floating in the air			
		return objectToCheck == null || IsObjectStatic(objectToCheck);
	}
	
	bool IsObjectStatic( Object obj )
	{
		return obj.IsBuilding() || obj.IsPlainObject() || (obj.IsInherited(BaseBuildingBase) && (m_WatchtowerBlockedComponentNames.Find(obj.GetActionComponentName(m_ContactComponent, "view")) == -1));
	}

	bool IsBaseIntact( Object under_left_close, Object under_right_close, Object under_left_far, Object under_right_far )
	{
		return (under_left_close == under_right_close && under_right_close == under_left_far && under_left_far == under_right_far);
		/*
		if( under_left_close == under_right_close && under_right_close == under_left_far && under_left_far == under_right_far )
		{
			//Print("base is intact");
			return true;			
		}
		else
		{
			//Print("base is NOT intact");
			return false;			
		}
		*/
	}

	bool IsBaseFlat( vector contact_pos_left_close, vector contact_pos_right_close, vector contact_pos_left_far, vector contact_pos_right_far )
	{		
		vector projection_pos = GetProjectionPosition();
		float slope_pos_left_close = projection_pos[1] - contact_pos_left_close[1];
		float slope_pos_right_close = projection_pos[1] - contact_pos_right_close[1];
		float slope_pos_left_far = projection_pos[1] - contact_pos_left_far[1];
		float slope_pos_right_far = projection_pos[1] - contact_pos_right_far[1];
		
		/*
		Print(GetProjectionPosition());
				
		Print(contact_pos_left_close);
		Print(contact_pos_right_close);
		Print(contact_pos_left_far);
		Print(contact_pos_right_far);
		*/
		
		return slope_pos_left_close < m_SlopeTolerance && slope_pos_right_close < m_SlopeTolerance && slope_pos_left_far < m_SlopeTolerance && slope_pos_right_far < m_SlopeTolerance;
		
		/*
		if( slope_pos_left_close < m_SlopeTolerance && slope_pos_right_close < m_SlopeTolerance && slope_pos_left_far < m_SlopeTolerance && slope_pos_right_far < m_SlopeTolerance )
		{
			Print("base is flat");
			return true;	
		}
		
		Print("base is NOT flat");
		return false;	
		*/		
	}
	
	/*
	Shape DrawArrow(vector from, vector to, float size = 0.5, int color = 0xFFFFFFFF, float flags = 0)
	{
		vector dir = to - from;
		dir.Normalize();
		vector dir1 = dir * size;
		size = size * 0.5;
	
		vector dir2 = dir.Perpend() * size;
	
		vector pts[5];
		pts[0] = from;
		pts[1] = to;
		pts[2] = to - dir1 - dir2;
		pts[3] = to - dir1 + dir2;
		pts[4] = to;
		
		return Shape.CreateLines(color, flags, pts, 5);
	}
	*/
	
	//! Checks if the item can be legally placed (usually checked by action as well)
	bool IsPlacementPermitted()
	{
		ItemBase item = m_Player.GetItemInHands();
		if( item && item.Type() == GetProjectionEntity().Type() && !item.CanBePlaced(m_Player,GetProjectionPosition()) )
		{
			return false;
		}
		return true;
	}
	
	//! Checks height relative to player's position
	bool HeightPlacementCheck()
	{
		if( GetProjectionEntity() ) //simple height check
		{
			vector playerpos = m_Player.GetPosition();
			vector projectionpos = GetProjectionPosition();
			float delta1 = playerpos[1] - projectionpos[1];
			
			if( delta1 > DEFAULT_MAX_PLACEMENT_HEIGHT_DIFF || delta1 < -DEFAULT_MAX_PLACEMENT_HEIGHT_DIFF )
			{
				return false;
			}
		}
		return true;
	}
	
	bool IsUnderwater()
	{
		// Fast check middle of object
		string type;
		int liquid;
		g_Game.SurfaceUnderObject(m_Projection, type, liquid);
		
		if (liquid == LIQUID_WATER)
			return true;
		
		// Check every corner of the object
		vector left_close = m_Projection.CoordToParent( GetLeftCloseProjectionVector() );
		vector right_close = m_Projection.CoordToParent( GetRightCloseProjectionVector() );
		vector left_far = m_Projection.CoordToParent( GetLeftFarProjectionVector() );
		vector right_far = m_Projection.CoordToParent( GetRightFarProjectionVector() );
		
		return (g_Game.GetWaterDepth(left_close) > 0 || g_Game.GetWaterDepth(right_close) > 0 || g_Game.GetWaterDepth(left_far) > 0 || g_Game.GetWaterDepth(right_far) > 0);
	}
	
	bool IsInTerrain()
	{
		vector fromHeightOffset = "0 0.15 0";
		vector toHeightOffset = "0 1 0";
		
		vector from_left_close = m_Projection.CoordToParent( GetLeftCloseProjectionVector() ) + fromHeightOffset;
		vector to_left_close_down = from_left_close + toHeightOffset;

		vector from_right_close = m_Projection.CoordToParent( GetRightCloseProjectionVector() ) + fromHeightOffset;
		vector to_right_close_down = from_right_close + toHeightOffset;

		vector from_left_far = m_Projection.CoordToParent( GetLeftFarProjectionVector() ) + fromHeightOffset;
		vector to_left_far_down = from_left_far + toHeightOffset;

		vector from_right_far = m_Projection.CoordToParent( GetRightFarProjectionVector() ) + fromHeightOffset;
		vector to_right_far_down = from_right_far + toHeightOffset;

		vector contact_pos_left_close;
		vector contact_pos_right_close;
		vector contact_pos_left_far;
		vector contact_pos_right_far;
		
		vector contact_dir_left_close;
		vector contact_dir_right_close;
		vector contact_dir_left_far;
		vector contact_dir_right_far;
		
		int contact_component_left_close;
		int contact_component_right_close;
		int contact_component_left_far;
		int contact_component_right_far;

		if (DayZPhysics.RaycastRV( from_left_close, to_left_close_down, contact_pos_left_close, contact_dir_left_close, contact_component_left_close, NULL, m_Projection, m_Projection, false, true ))
			return true;

		if (DayZPhysics.RaycastRV( from_right_close, to_right_close_down, contact_pos_right_close, contact_dir_right_close, contact_component_right_close, NULL,m_Projection, m_Projection, false, true ))
			return true;

		if (DayZPhysics.RaycastRV( from_left_far, to_left_far_down, contact_pos_left_far, contact_dir_left_far, contact_component_left_far, NULL, m_Projection, m_Projection, false, true ))
			return true;

		if (DayZPhysics.RaycastRV( from_right_far, to_right_far_down, contact_pos_right_far, contact_dir_right_far, contact_component_right_far, NULL, m_Projection, m_Projection, false, true ))
			return true;
		
		return false;
	}
	
	void CheckPowerSource()
	{
		//in range of its power source.
		if ( m_Player && m_Parent && m_Parent.HasEnergyManager() && m_Parent.GetCompEM().IsPlugged() )
		{
			// Now we know we are working with an electric device which is plugged into a power source.
			EntityAI placed_entity = m_Parent;
					
			// Unplug the device when the player is too far from the power source.
			placed_entity.GetCompEM().UpdatePlugState();
			
			// Delete local hologram when plug is rippled out and advanced placement is active
			if( GetGame().IsMultiplayer() && GetGame().IsClient() )
			{
				if ( !placed_entity.GetCompEM().IsPlugged() )
				{
					m_Player.MessageImportant("The plug was ripped out!");
					
					// Cancel placement mode
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(m_Player.TogglePlacingLocal);
				}
			}
		}				
	}
	
	EntityAI PlaceEntity( EntityAI entity_for_placing )
	{
		//string-based comparison
		if ( entity_for_placing.IsInherited( TentBase ) || entity_for_placing.IsBasebuildingKit() ) //special little snowflakes
		{
			//condition redundant, return if you want
			/*
			string type_string = GetProjectionEntity().GetType();
			type_string = type_string.Substring(0,type_string.IndexOf("Placing"));
			if( type_string == entity_for_placing.GetType() ) //done via string matching, not inheritence. Consistent with previous implementation (...)
			*/
				return entity_for_placing;
		}
		//inheritance comparison
		if( !GetProjectionEntity().IsKindOf( m_Parent.GetType() ))
		{
			Class.CastTo(entity_for_placing, GetGame().CreateObjectEx( m_Projection.GetType(), m_Projection.GetPosition(), ECE_PLACE_ON_SURFACE ));
		}
		
		return entity_for_placing;
	}
	
	protected void GetProjectionCollisionBox( out vector min_max[2] )
	{
		if (!m_Projection.GetCollisionBox( min_max ) && m_Projection.MemoryPointExists("box_placing_min"))
		{
			min_max[0] = m_Projection.GetMemoryPointPos( "box_placing_min" );
			min_max[1] = m_Projection.GetMemoryPointPos( "box_placing_max" );
			//Debug.DrawSphere(m_Projection.ModelToWorld(min_max[0]) , 0.8,Colors.RED, ShapeFlags.ONCE);
			//Debug.DrawSphere(m_Projection.ModelToWorld(min_max[1]), 0.8,Colors.RED, ShapeFlags.ONCE);
		}
		else
		{
			//Debug.DrawSphere(m_Projection.ModelToWorld(min_max[0]) , 0.8,Colors.GREEN, ShapeFlags.ONCE);
			//Debug.DrawSphere(m_Projection.ModelToWorld(min_max[1]), 0.8,Colors.GREEN, ShapeFlags.ONCE);
		}
	}
	
	protected vector GetCollisionBoxSize( vector min_max[2] )
	{
		vector box_size = Vector(1,1,1);
		
		box_size[0] = min_max[1][0] - min_max[0][0];
		box_size[2] = min_max[1][2] - min_max[0][2];
		box_size[1] = min_max[1][1] - min_max[0][1];
		
		return box_size;
	}

	vector GetLeftCloseProjectionVector()
	{
		vector min_max[2];
		GetProjectionCollisionBox( min_max );

		return min_max[0];
	}

	vector GetRightCloseProjectionVector()
	{
		vector min_max[2];
		GetProjectionCollisionBox( min_max );
		min_max[1][1] = min_max[0][1];
		min_max[1][2] = min_max[0][2];

		return min_max[1];
	}

	vector GetLeftFarProjectionVector()
	{
		vector min_max[2];
		GetProjectionCollisionBox( min_max );
		min_max[0][2] = min_max[1][2];

		return min_max[0];
	}

	vector GetRightFarProjectionVector()
	{
		vector min_max[2];
		GetProjectionCollisionBox( min_max );
		min_max[1][1] = min_max[0][1]; 

		return min_max[1];
	}

	bool IsSurfaceWater( vector position )
	{
		CGame game = GetGame();
		return game.SurfaceIsSea( position[0], position[2] ) || game.SurfaceIsPond( position[0], position[2] );
	}

	protected vector GetProjectionEntityPosition( PlayerBase player )
	{
		float min_projection_dist;
		float max_projection_dist; 
		m_ContactDir = vector.Zero;
		vector min_max[2];
		float projection_radius = GetProjectionRadius();
		float camera_to_player_distance = vector.Distance( GetGame().GetCurrentCameraPosition(), player.GetPosition() );

		if( projection_radius < SMALL_PROJECTION_RADIUS )	// objects with radius smaller than 1m
		{
			min_projection_dist = DISTANCE_SMALL_PROJECTION;
			max_projection_dist = DISTANCE_SMALL_PROJECTION * 2;
		}
		else
		{
			min_projection_dist = projection_radius;
			max_projection_dist = projection_radius * 2;
			max_projection_dist = Math.Clamp( max_projection_dist, 0, LARGE_PROJECTION_DISTANCE_LIMIT );
		}
		
		vector from = GetGame().GetCurrentCameraPosition();
		vector to = from + ( GetGame().GetCurrentCameraDirection() * ( max_projection_dist + camera_to_player_distance ) );
		vector contact_pos;
		set<Object> hit_object = new set<Object>;
		
		DayZPhysics.RaycastRV( from, to, contact_pos, m_ContactDir, m_ContactComponent, hit_object, player, m_Projection, false, false );
		
		static const float raycastOriginOffsetOnFail = 0.25;
		static const float minDistFromStart = 0.01;
		// Camera isn't correctly positioned in some cases, leading to raycasts hitting the object directly behind the camera
		if ((hit_object.Count() > 0) && (vector.DistanceSq(from, contact_pos) < minDistFromStart))
		{
			from = contact_pos + GetGame().GetCurrentCameraDirection() * raycastOriginOffsetOnFail;
			DayZPhysics.RaycastRV( from, to, contact_pos, m_ContactDir, m_ContactComponent, hit_object, player, m_Projection, false, false );
		}
		
		if ((hit_object.Count() > 0) && hit_object[0].IsInherited(Watchtower))
			contact_pos = CorrectForWatchtower( m_ContactComponent, contact_pos, player, hit_object[0] );


		float player_to_projection_distance = vector.Distance( player.GetPosition(), contact_pos );
		vector player_to_projection_vector;

		//hologram is at min distance from player
		if( player_to_projection_distance <= min_projection_dist )
		{
			player_to_projection_vector = contact_pos - player.GetPosition();		  
			player_to_projection_vector.Normalize();
			//prevents the hologram to go underground/floor while hologram exceeds min_projection_dist
			player_to_projection_vector[1] = player_to_projection_vector[1] + PROJECTION_TRANSITION_MIN;
			
			contact_pos = player.GetPosition() + (player_to_projection_vector * min_projection_dist);			
			SetIsFloating( true );
		}
		//hologram is at max distance from player
		else if( player_to_projection_distance >= max_projection_dist )
		{
			player_to_projection_vector = contact_pos - player.GetPosition();	
			player_to_projection_vector.Normalize();
			//prevents the hologram to go underground/floor while hologram exceeds max_projection_dist
			player_to_projection_vector[1] = player_to_projection_vector[1] + PROJECTION_TRANSITION_MAX;		
			
			contact_pos = player.GetPosition() + (player_to_projection_vector * max_projection_dist);
			SetIsFloating( true );
		}
		//hologram is between min and max distance from player
		else
		{
			SetIsFloating( false );
		}			
			
		return contact_pos;
	}
	
	bool IsFenceOrWatchtowerKit()
	{
		return m_Parent.IsBasebuildingKit() || m_Parent.IsInherited(TentBase);
	}
	
	vector CorrectForWatchtower( int contactComponent, vector contactPos, PlayerBase player, Object hitObject )
	{
		// Raycast has hit one of the trigger boxes that show construction prompts, so projection would be floating in the air without this correction
		if ( m_WatchtowerIgnoreComponentNames.Find(hitObject.GetActionComponentName( contactComponent, "view" )) != -1 )
			contactPos[1] = hitObject.GetActionComponentPosition(contactComponent, "view")[1];
		
		return contactPos;
	}
	
	/*
	bool ShouldContinueRaycasting( vector from, vector contactPos, set<Object> hitObjects, PlayerBase player, int contactComponent )
	{		
		static const float minDistFromStart = 0.01;
		
		Print((hitObjects.Count() > 0));
		Print(vector.DistanceSq(from, contactPos) < minDistFromStart);
		if (hitObjects.Count() > 0)
			Print(hitObjects[0].IsInherited( Watchtower ));
		Print( m_BaseBuildingIgnoreComponents.Find(contactComponent) != -1 );
		
		return (hitObjects.Count() > 0) && (vector.DistanceSq(from, contactPos) > 1.0) && ((vector.DistanceSq(from, contactPos) < minDistFromStart) || (hitObjects[0].IsInherited( Watchtower ) && m_BaseBuildingIgnoreComponents.Find(contactComponent) != -1));
	}
	*/

	//This function is currently unused
	bool IsProjectionTrap()
	{
		return m_Projection.IsInherited( TrapBase ) || m_Projection.IsInherited( TrapSpawnBase );
	}

	float GetProjectionDiameter()
	{		
		float diameter;
		float radius;
		vector diagonal;
		vector min_max[2];

		GetProjectionCollisionBox( min_max );
		diagonal = GetCollisionBoxSize( min_max );
		diameter = diagonal.Length();

		return diameter;	
	}

	float GetProjectionRadius()
	{		
		float diameter;
		float radius;
		vector diagonal;
		vector min_max[2];

		GetProjectionCollisionBox( min_max );
		diagonal = GetCollisionBoxSize( min_max );
		diameter = diagonal.Length();
		radius = diameter / 2;

		return radius;	
	}
		
	void SetUpdatePosition( bool state )
	{
		m_UpdatePosition = state;
	}

	bool GetUpdatePosition()
	{
		return m_UpdatePosition;
	}

	EntityAI GetParentEntity()
	{
		return m_Parent;
	}
	
	void SetProjectionEntity( EntityAI projection )
	{
		m_Projection = projection;
	}

	EntityAI GetProjectionEntity()
	{
		return m_Projection;
	}
	
	void SetIsFloating( bool is_floating )
	{
		m_IsFloating = is_floating;
	}
		
	void SetIsColliding( bool is_colliding )
	{
		m_IsColliding = is_colliding;
	}

	void SetIsHidden( bool is_hidden )
	{
		m_IsHidden = is_hidden;
	}
	
	void SetIsCollidingPlayer( bool is_colliding )
	{
		m_IsCollidingPlayer = is_colliding;
	}

	void SetIsCollidingGPlot( bool is_colliding_gplot )
	{
		m_IsCollidingGPlot = is_colliding_gplot;
	}

	bool IsFloating()
	{
		return m_IsFloating;
	}

	bool IsColliding()
	{
		return m_IsColliding;
	}
	
	bool IsHidden()
	{
		return m_IsHidden;
	}

	bool IsCollidingPlayer()
	{
		return m_IsCollidingPlayer;
	}

	void SetProjectionPosition( vector position )
	{	
		m_Projection.SetPosition( position );			
		
		if ( IsFloating() )
		{
			m_Projection.SetPosition( SetOnGround( position ) );
		}
	}

	void SetProjectionOrientation( vector orientation )
	{	
		m_Projection.SetOrientation( orientation );
	}
	
	vector GetProjectionRotation()
	{
		return m_Rotation;
	}
	
	void AddProjectionRotation( float addition )
	{	
		m_Rotation[0] = m_Rotation[0] + addition;
	}
	
	void SubtractProjectionRotation( float subtraction )
	{	
		m_Rotation[0] = m_Rotation[0] - subtraction;
	}
	
	vector SetOnGround( vector position )
	{
		vector from = position;
		vector ground;
		vector player_to_projection_vector;
		float projection_diameter = GetProjectionDiameter();
			
		ground = Vector(0, - Math.Max( projection_diameter, SMALL_PROJECTION_GROUND ), 0);	
		/*
		if( projection_diameter < SMALL_PROJECTION_GROUND )
		{
			ground = Vector(0, - SMALL_PROJECTION_GROUND, 0);
		}
		else
		{
			ground = Vector(0, - projection_diameter, 0);
		}
		*/

		vector to = from + ground;
		vector contact_pos;
		//vector contact_dir;
		int contact_component;
		
		DayZPhysics.RaycastRV( from, to, contact_pos, m_ContactDir, contact_component, NULL, NULL, m_Projection, false, false );

		HideWhenClose( contact_pos );

		return contact_pos;
	}

	vector HideWhenClose( vector pos )
	{
		//if the hologram is too close to player when he looks to the sky, send the projection to away
		vector cam_dir = GetGame().GetCurrentCameraDirection();

		if( cam_dir[1] > LOOKING_TO_SKY )
		{
			pos = "0 0 0";
		}

		return pos;
	}

	vector GetProjectionPosition()
	{
		return m_Projection.GetPosition();
	}
	
	vector GetProjectionOrientation()
	{
		return m_Projection.GetOrientation();
	}
	
	vector GetDefaultOrientation()
	{
		m_DefaultOrientation = GetGame().GetCurrentCameraDirection().VectorToAngles();
		m_DefaultOrientation[1] = 0;
		
		if (!GetParentEntity().PlacementCanBeRotated())
		{
			m_DefaultOrientation = vector.Zero;
		}
		
		return m_DefaultOrientation;
	}

	int GetHiddenSelection( string selection )
	{
		string config_path = "CfgVehicles " + m_Projection.GetType() + " hiddenSelections";
		ref array<string> hidden_selection_array = new array<string>;

		GetGame().ConfigGetTextArray( config_path, hidden_selection_array );	

		for (int i = 0; i < hidden_selection_array.Count(); i++)
		{
			if ( hidden_selection_array.Get(i) == selection )
			{
				return i;
			}			
		}	

		return 0;
	}

	// the function accepts string
	void SetSelectionToRefresh( string selection )
	{
		m_SelectionsToRefresh.Insert( selection );
	}

	//overloaded function to accept array of strings
	void SetSelectionToRefresh( array<string> selection )
	{
		for( int i = 0; i < selection.Count(); i++ )
		{
			m_SelectionsToRefresh.Insert( selection.Get(i) );
		}
	}
		
	void RefreshVisual()
	{
		if ( m_Projection )
		{
			string config_material = "CfgVehicles" + " " + m_Projection.GetType() + " " + "hologramMaterial";
			string hologram_material = GetGame().ConfigGetTextOut( config_material );
			string config_model = "CfgVehicles" + " " + m_Projection.GetType() + " " + "hologramMaterialPath";
			string hologram_material_path = GetGame().ConfigGetTextOut( config_model ) + "\\" + hologram_material;
			string selection_to_refresh;
			int hidden_selection = 0;
			static const string textureName = "#(argb,8,8,3)color(0.5,0.5,0.5,0.75,ca)";
			
			hologram_material_path += CorrectMaterialPathName();
				
			for (int i = 0; i < m_SelectionsToRefresh.Count(); ++i)
			{
				selection_to_refresh = m_SelectionsToRefresh.Get(i);
				hidden_selection = GetHiddenSelection( selection_to_refresh );
				m_Projection.SetObjectTexture( hidden_selection, textureName );
				m_Projection.SetObjectMaterial( hidden_selection, hologram_material_path );
			}
		}
	}
	
	// Returns correct string to append to material path name
	string CorrectMaterialPathName()
	{
		if (IsColliding())
			return "_undeployable.rvmat";
		
		else if (m_Parent.HasEnergyManager())
		{
			ComponentEnergyManager comp_em = m_Parent.GetCompEM();
			string SEL_CORD_PLUGGED = m_Parent.GetCompEM().SEL_CORD_PLUGGED;
			string SEL_CORD_FOLDED = m_Parent.GetCompEM().SEL_CORD_FOLDED;
			
			if (comp_em.IsPlugged() && comp_em.IsEnergySourceAtReach( GetProjectionPosition() ))
			{
				m_Projection.SetAnimationPhase( SEL_CORD_PLUGGED, 0);
				m_Projection.SetAnimationPhase( SEL_CORD_FOLDED, 1);	
				return "_powered.rvmat";
			}
			else
			{
				m_Projection.SetAnimationPhase( SEL_CORD_PLUGGED, 1);
				m_Projection.SetAnimationPhase( SEL_CORD_FOLDED, 0);
			}
		}
		return "_deployable.rvmat";
	}
};

class ProjectionTrigger extends Trigger
{
	protected int m_TriggerUpdateMs;
	protected Hologram m_ParentObj;
	protected PlayerBase m_Player;

	override void OnEnter( Object obj )
	{
		//Print("OnEnter");
		if ( m_ParentObj )
		{
			m_ParentObj.SetIsCollidingPlayer( true );
			m_TriggerUpdateMs = 50;
		}
	}

	override void OnLeave( Object obj )
	{
		//Print("OnLeave");
		if ( m_ParentObj )
		{
			m_ParentObj.SetIsCollidingPlayer( false );
		}
	}

	override protected void UpdateInsiders(int timeout )
	{
		super.UpdateInsiders(m_TriggerUpdateMs);
	}
	
	void SetParentObject( Hologram projection )
	{
		m_ParentObj = projection;
	}

	void SetParentOwner( PlayerBase player )
	{
		m_Player = player;
	}
}

