class CCTWaterSurface : CCTBase
{
	protected float m_MaximalActionDistanceSq;
	protected string m_SurfaceType;
	
	void CCTWaterSurface ( float maximal_target_distance = UAMaxDistances.DEFAULT, string surfaceType = "" )
	{
		m_MaximalActionDistanceSq = maximal_target_distance * maximal_target_distance;
		m_SurfaceType = surfaceType;
	}
	
	override bool Can( PlayerBase player, ActionTarget target )
	{	
		if ( !target || ( target && target.GetObject() ) )
			return false;
		
		if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
			return true;
		
		// See if we are looking at something			
		vector hit_pos = target.GetCursorHitPos();
		if (hit_pos == vector.Zero)
			return false;
			
		// If we are standing in water, water action is allowed (liquidType == LIQUID_WATER)
		string surfType;
		int liquidType;
		g_Game.SurfaceUnderObjectByBone(player, SurfaceAnimationBone.LeftBackLimb, surfType, liquidType);
		if (liquidType == LIQUID_WATER)
			return true;
		
		// See if the surface at the cursor position is water (surfType.Contains("water"))
		// Small Y offset, as it will prioritize water when surfaces are really close together
		g_Game.SurfaceGetType3D(hit_pos[0], hit_pos[1] + 0.1, hit_pos[2], surfType);
		
		// See if the player is looking at the sea
		bool isSeaCheck = false;
		if ( m_SurfaceType == UAWaterType.ALL )
			isSeaCheck = ( Math.AbsFloat(hit_pos[1] - g_Game.SurfaceGetSeaLevel()) <= 0.001);
		
		// Sometimes SurfaceGetType3D ignores the Y, this makes it so the proper distance is calculated
		hit_pos[1] = g_Game.SurfaceY(hit_pos[0],hit_pos[2]);
		
		// Combine the tests and check the distance
		return ( vector.DistanceSq(hit_pos, player.GetPosition()) <= m_MaximalActionDistanceSq && (surfType.Contains(m_SurfaceType) || isSeaCheck) );
	}
};
