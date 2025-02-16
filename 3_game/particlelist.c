// Register all particles below!

// Example how to register particles from a mod
/*
modded class ParticleList
{
	static const int MODDED_PARTICLE = RegisterParticle( "mod_folder/" , "my_modded_particle");
}
*/

class ParticleList
{
	ref static map<int, string> m_ParticlePaths; // Contains paths to all particles. WARNING: Paths are without the '.ptc' suffix!
	static int m_lastID = 0;
	
	// REGISTER ALL PARTICLES BELOW:
	
	static const int INVALID						= -1;
	static const int PARTICLE_TEST					= RegisterParticle("_test_orientation");
	static const int DEBUG_DOT						= RegisterParticle("debug_dot");
	
	// FIREPLACE
	// Normal fireplace
	static const int CAMP_FIRE_START				= RegisterParticle("fire_small_camp_01_start");
	static const int CAMP_SMALL_FIRE 				= RegisterParticle("fire_small_camp_01");
	static const int CAMP_NORMAL_FIRE				= RegisterParticle("fire_medium_camp_01");
	static const int CAMP_SMALL_SMOKE 				= RegisterParticle("smoke_small_camp_01");
	static const int CAMP_NORMAL_SMOKE				= RegisterParticle("smoke_medium_camp_01");
	static const int CAMP_FIRE_END					= RegisterParticle("fire_small_camp_01_end");
	static const int CAMP_STEAM_2END				= RegisterParticle("steam_medium_camp_2end");
	static const int CAMP_STEAM_EXTINGUISH_START	= RegisterParticle("default_01");
	static const int CAMP_STOVE_FIRE				= RegisterParticle("fire_small_stove_01");
	static const int CAMP_STOVE_FIRE_START			= RegisterParticle("fire_small_stove_01_start");
	static const int CAMP_STOVE_FIRE_END			= RegisterParticle("fire_small_stove_01_end");
	// Fireplace indoor
	static const int HOUSE_FIRE_START				= RegisterParticle("fire_small_house_01_start");
	static const int HOUSE_SMALL_FIRE 				= RegisterParticle("fire_small_house_01");
	static const int HOUSE_SMALL_SMOKE 				= RegisterParticle("smoke_small_house_01");
	static const int HOUSE_NORMAL_FIRE				= RegisterParticle("fire_medium_house_01");
	static const int HOUSE_NORMAL_SMOKE				= RegisterParticle("smoke_medium_house_01");
	static const int HOUSE_FIRE_END					= RegisterParticle("fire_small_house_01_end");
	static const int HOUSE_FIRE_STEAM_2END			= RegisterParticle("steam_medium_house_2end");
	// Fireplace in barrel with holes
	static const int BARREL_FIRE_START				= RegisterParticle("fire_small_barrel_01_start");
	static const int BARREL_SMALL_FIRE 				= RegisterParticle("fire_small_barrel_01");
	static const int BARREL_SMALL_SMOKE 			= RegisterParticle("smoke_small_barrel_01");
	static const int BARREL_NORMAL_FIRE				= RegisterParticle("fire_medium_barrel_01");
	static const int BARREL_NORMAL_SMOKE			= RegisterParticle("smoke_medium_barrel_01");
	static const int BARREL_FIRE_END				= RegisterParticle("fire_small_barrel_01_end");
	static const int BARREL_FIRE_STEAM_2END			= RegisterParticle("steam_medium_camp_2end");
	// Fireplace in indoor oven
	static const int OVEN_FIRE_START				= RegisterParticle("fire_small_oven_01_start");
	static const int OVEN_SMALL_FIRE 				= RegisterParticle("fire_small_oven_01");
	static const int OVEN_NORMAL_FIRE				= RegisterParticle("fire_medium_oven_01");
	static const int OVEN_FIRE_END					= RegisterParticle("fire_small_ovenl_01_end");
	
	// COOKING
	static const int COOKING_BOILING_EMPTY			= RegisterParticle("cooking_boiling_empty");
	static const int COOKING_BOILING_START			= RegisterParticle("cooking_boiling_start");
	static const int COOKING_BOILING_DONE			= RegisterParticle("cooking_boiling_done");
	static const int COOKING_BAKING_START			= RegisterParticle("cooking_baking_start");
	static const int COOKING_BAKING_DONE			= RegisterParticle("cooking_baking_done");	
	static const int COOKING_DRYING_START			= RegisterParticle("cooking_drying_start");
	static const int COOKING_DRYING_DONE			= RegisterParticle("cooking_drying_done");
	static const int COOKING_BURNING_DONE			= RegisterParticle("cooking_burning_done");
	
	// TORCH
	static const int TORCH_T1						= RegisterParticle("fire_small_torch_02");
	static const int TORCH_T2						= RegisterParticle("fire_small_torch_03");
	static const int TORCH_T3						= RegisterParticle("fire_small_torch_01");
	
	// ROADFLARE
	static const int ROADFLARE_BURNING_INIT			= RegisterParticle("fire_small_roadflare_red_04");
	static const int ROADFLARE_BURNING_MAIN			= RegisterParticle("fire_small_roadflare_red_01");
	static const int ROADFLARE_BURNING_ENDING		= RegisterParticle("fire_small_roadflare_red_02");
	static const int ROADFLARE_BURNING_SMOKE		= RegisterParticle("fire_small_roadflare_red_03");
	
	// DIGGING
	static const int DIGGING_STASH					= RegisterParticle("digging_ground");
	
	// SMOKE GRENADES
	//! RDG2
	static const int GRENADE_RDG2_BLACK_START		= RegisterParticle("smoke_RDG2_black_01");
	static const int GRENADE_RDG2_BLACK_LOOP		= RegisterParticle("smoke_RDG2_black_02");
	static const int GRENADE_RDG2_BLACK_END			= RegisterParticle("smoke_RDG2_black_03");
	static const int GRENADE_RDG2_WHITE_START		= RegisterParticle("smoke_RDG2_white_01");
	static const int GRENADE_RDG2_WHITE_LOOP		= RegisterParticle("smoke_RDG2_white_02");
	static const int GRENADE_RDG2_WHITE_END			= RegisterParticle("smoke_RDG2_white_03");
	//! M18
	static const int GRENADE_M18_GREEN_START		= RegisterParticle("smoke_M18_green_01");
	static const int GRENADE_M18_GREEN_LOOP			= RegisterParticle("smoke_M18_green_02");
	static const int GRENADE_M18_GREEN_END			= RegisterParticle("smoke_M18_green_03");
	static const int GRENADE_M18_PURPLE_START		= RegisterParticle("smoke_M18_purple_01");
	static const int GRENADE_M18_PURPLE_LOOP		= RegisterParticle("smoke_M18_purple_02");
	static const int GRENADE_M18_PURPLE_END			= RegisterParticle("smoke_M18_purple_03");
	static const int GRENADE_M18_RED_START			= RegisterParticle("smoke_M18_red_01");
	static const int GRENADE_M18_RED_LOOP			= RegisterParticle("smoke_M18_red_02");
	static const int GRENADE_M18_RED_END			= RegisterParticle("smoke_M18_red_03");
	static const int GRENADE_M18_WHITE_START		= RegisterParticle("smoke_M18_white_01");
	static const int GRENADE_M18_WHITE_LOOP			= RegisterParticle("smoke_M18_white_02");
	static const int GRENADE_M18_WHITE_END			= RegisterParticle("smoke_M18_white_03");
	static const int GRENADE_M18_YELLOW_START		= RegisterParticle("smoke_M18_yellow_01");
	static const int GRENADE_M18_YELLOW_LOOP		= RegisterParticle("smoke_M18_yellow_02");
	static const int GRENADE_M18_YELLOW_END			= RegisterParticle("smoke_M18_yellow_03");
	
	//! FLASH GRENADE
	//! M84
	static const int GRENADE_M84					= RegisterParticle("explosion_M84_01");
	
	// FRAGMENTATION GRENADES
	static const int RGD5							= RegisterParticle("explosion_RGD5_01");
	static const int M67							= RegisterParticle("explosion_M67_01");
	
	// ELECTRICITY	        
	static const int POWER_GENERATOR_SMOKE			= RegisterParticle("smoke_small_generator_01");
	static const int BARBED_WIRE_SPARKS				= RegisterParticle("electro_shortc2");

	// PLAYER
	static const int BLEEDING_SOURCE				= RegisterParticle("blood_bleeding_01");
	static const int BLEEDING_SOURCE_LIGHT			= RegisterParticle("blood_bleeding_02");
	static const int BLOOD_SURFACE_DROPS			= RegisterParticle("blood_surface_drops");
	static const int BLOOD_SURFACE_CHUNKS			= RegisterParticle("blood_surface_chunks");
	static const int VOMIT							= RegisterParticle("character_vomit_01");
	static const int BREATH_VAPOUR_LIGHT			= RegisterParticle("breath_vapour_light");
	static const int BREATH_VAPOUR_MEDIUM			= RegisterParticle("breath_vapour_medium");
	static const int BREATH_VAPOUR_HEAVY			= RegisterParticle("breath_vapour_heavy");
	static const int VOMIT_CHUNKS					= RegisterParticle("character_vomit_puddle");
	
	// GUNS
	static const int GUN_MUZZLE_FLASH_SVD_STAR		= RegisterParticle("weapon_shot_Flame_3D_4star");
	static const int GUN_SOLO_MUZZLE_FLASH			= RegisterParticle("weapon_shot_Flame_3D");
	static const int GUN_PARTICLE_TEST				= RegisterParticle("weapon_shot_fnx_01");
	static const int GUN_PARTICLE_CASING			= RegisterParticle("weapon_shot_chamber_smoke");
	static const int GUN_PARTICLE_CASING_RAISE		= RegisterParticle("weapon_shot_chamber_smoke_raise");
	static const int GUN_FNX						= RegisterParticle("weapon_shot_fnx_01");
	static const int GUN_FNX_SUPPRESSED				= RegisterParticle("weapon_shot_fnx_02");
	static const int GUN_CZ75						= RegisterParticle("weapon_shot_cz75_01");
	static const int GUN_AKM						= RegisterParticle("weapon_shot_akm_01");
	static const int GUN_AKM_SUPPRESSED				= RegisterParticle("weapon_shot_akm_02");
	static const int GUN_IZH18						= RegisterParticle("weapon_shot_izh18_01");
	static const int GUN_IZH18_SUPPRESSED			= RegisterParticle("weapon_shot_izh18_02");
	static const int GUN_MP5K						= RegisterParticle("weapon_shot_mp5k_01");
	static const int GUN_MP5K_2						= RegisterParticle("weapon_shot_mp5k_02_boris");
	static const int GUN_MP5K_SUPPRESSED			= RegisterParticle("weapon_shot_mp5k_02");
	static const int GUN_MP5K_COMPENSATOR			= RegisterParticle("weapon_shot_mp5k_02");
	static const int GUN_UMP45						= RegisterParticle("weapon_shot_ump45_01");
	static const int GUN_M4A1						= RegisterParticle("weapon_shot_m4a1_01");
	static const int GUN_MP133						= RegisterParticle("weapon_shot_mp133_01");
	static const int GUN_PELLETS					= RegisterParticle("weapon_shot_pellets");
	static const int GUN_MOSIN9130					= RegisterParticle("weapon_shot_mosin9130_01");
	static const int GUN_MOSIN_COMPENSATOR			= RegisterParticle("weapon_shot_mosin_compensator_01");
	static const int GUN_CZ527						= RegisterParticle("weapon_shot_cz527_01");
	static const int GUN_SKS						= RegisterParticle("weapon_shot_sks_01");
	static const int GUN_WINCHESTER70				= RegisterParticle("weapon_shot_winch70_01");
	static const int GUN_VSS						= RegisterParticle("weapon_shot_vss_01");
	static const int GUN_AK74						= RegisterParticle("weapon_shot_ak74_01");
	static const int GUN_AK101						= RegisterParticle("weapon_shot_ak101_01");
	
	static const int GUN_MAGNUM						= RegisterParticle("weapon_shot_magnum_01");
	
	static const int GUN_CZ61						= RegisterParticle("weapon_shot_cz61_01");
	static const int GUN_LONG_WINDED_SMOKE			= RegisterParticle("weapon_shot_winded_smoke");
	static const int GUN_LONG_WINDED_SMOKE_SMALL	= RegisterParticle("weapon_shot_winded_smoke_small");
	static const int SMOKING_BARREL					= RegisterParticle("smoking_barrel");
	static const int SMOKING_BARREL_SMALL			= RegisterParticle("smoking_barrel_small");
	static const int SMOKING_BARREL_HEAVY			= RegisterParticle("smoking_barrel_heavy");
	static const int SMOKING_BARREL_STEAM			= RegisterParticle("smoking_barrel_steam");
	static const int SMOKING_BARREL_STEAM_SMALL		= RegisterParticle("smoking_barrel_steam_small");
	static const int SMARKS_CHAMBER					= RegisterParticle("weapon_shot_chamber_spark");
	
	// BULLET & MELEE IMPACTS
	static const int IMPACT_TEST					= RegisterParticle("impacts/bullet_impact_placeholder");
	static const int IMPACT_DISTANT_DUST			= RegisterParticle("impacts/distant_dust");
	static const int IMPACT_TEST_RICOCHET			= RegisterParticle("impacts/bullet_riochet_placeholder");
	static const int IMPACT_TEST2					= RegisterParticle("_test_orientation_02");
	static const int IMPACT_TEST_ENTER_DEBUG		= RegisterParticle("impacts/_test_impact_enter_debug");
	static const int IMPACT_TEST_RICOCHET_DEBUG		= RegisterParticle("impacts/_test_impact_ricochet_debug");
	static const int IMPACT_TEST_EXIT_DEBUG			= RegisterParticle("impacts/_test_impact_exit_debug");
	static const int IMPACT_TEST_NO_MATERIAL_ERROR	= RegisterParticle("_test_no_material");
	static const int IMPACT_WOOD_ENTER				= RegisterParticle("impacts/hit_wood_ent_01");
	static const int IMPACT_WOOD_RICOCHET			= RegisterParticle("impacts/hit_wood_ric_01");
	static const int IMPACT_WOOD_EXIT				= RegisterParticle("impacts/hit_wood_ext_01");
	static const int IMPACT_CONCRETE_ENTER			= RegisterParticle("impacts/hit_concrete_ent_01");
	static const int IMPACT_CONCRETE_RICOCHET		= RegisterParticle("impacts/hit_concrete_ric_01");
	static const int IMPACT_CONCRETE_EXIT			= RegisterParticle("impacts/hit_concrete_ext_01");
	static const int IMPACT_FOLIAGE_ENTER			= RegisterParticle("impacts/hit_foliage_ent_01");
	static const int IMPACT_FOLIAGE_RICOCHET		= RegisterParticle("impacts/hit_foliage_ric_01");
	static const int IMPACT_FOLIAGE_EXIT			= RegisterParticle("impacts/hit_foliage_ext_01");
	static const int IMPACT_GRASS_ENTER				= RegisterParticle("impacts/hit_grass_ent_01");
	static const int IMPACT_GRASS_RICOCHET			= RegisterParticle("impacts/hit_grass_ric_01");
	static const int IMPACT_DIRT_ENTER				= RegisterParticle("impacts/hit_dirt_ent_01");
	static const int IMPACT_DIRT_RICOCHET			= RegisterParticle("impacts/hit_dirt_ric_01");
	static const int IMPACT_DIRT_EXIT				= RegisterParticle("impacts/hit_dirt_ext_01");
	static const int IMPACT_RUBBER_ENTER			= RegisterParticle("impacts/hit_rubber_ent_01");
	static const int IMPACT_RUBBER_RICOCHET			= RegisterParticle("impacts/hit_rubber_ric_01");
	static const int IMPACT_RUBBER_EXIT				= RegisterParticle("impacts/hit_rubber_ext_01");
	static const int IMPACT_GRAVEL_ENTER			= RegisterParticle("impacts/hit_gravel_ent_01");
	static const int IMPACT_GRAVEL_RICOCHET			= RegisterParticle("impacts/hit_gravel_ric_01");
	static const int IMPACT_GRAVEL_EXIT				= RegisterParticle("impacts/hit_gravel_ext_01");
	static const int IMPACT_PLASTER_ENTER			= RegisterParticle("impacts/hit_plaster_ent_01");
	static const int IMPACT_PLASTER_RICOCHET		= RegisterParticle("impacts/hit_plaster_ric_01");
	static const int IMPACT_PLASTER_EXIT			= RegisterParticle("impacts/hit_plaster_ext_01");
	static const int IMPACT_METAL_ENTER				= RegisterParticle("impacts/hit_metal_ent_01");
	static const int IMPACT_METAL_RICOCHET			= RegisterParticle("impacts/hit_metal_ric_01");
	static const int IMPACT_METAL_EXIT				= RegisterParticle("impacts/hit_metal_ext_01");
	static const int IMPACT_MEAT_ENTER				= RegisterParticle("impacts/hit_meat_ent_01");
	static const int IMPACT_MEAT_RICOCHET			= RegisterParticle("impacts/hit_meat_ric_01");
	static const int IMPACT_MEAT_EXIT				= RegisterParticle("impacts/hit_meat_ext_01");
	static const int IMPACT_MEATBONES_ENTER			= RegisterParticle("impacts/hit_meatbones_ent_01");
	static const int IMPACT_MEATBONES_RICOCHET		= RegisterParticle("impacts/hit_meatbones_ent_01");
	static const int IMPACT_MEATBONES_EXIT			= RegisterParticle("impacts/hit_meatbones_ext_01");
	static const int IMPACT_GLASS_ENTER				= RegisterParticle("impacts/hit_glass_ent_01");
	static const int IMPACT_GLASS_RICOCHET			= RegisterParticle("impacts/hit_glass_ric_01");
	static const int IMPACT_GLASS_EXIT				= RegisterParticle("impacts/hit_glass_ext_01");
	static const int IMPACT_WATER_SMALL_ENTER		= RegisterParticle("impacts/hit_water_ent_01");
	static const int IMPACT_WATER_MEDIUM_ENTER		= RegisterParticle("impacts/hit_water_ent_02");
	static const int IMPACT_WATER_LARGE_ENTER		= RegisterParticle("impacts/hit_water_ent_03");
	static const int IMPACT_TEXTILE_ENTER			= RegisterParticle("impacts/hit_textile_ent_01");
	static const int IMPACT_TEXTILE_RICOCHET		= RegisterParticle("impacts/hit_textile_ric_01");
	static const int IMPACT_TEXTILE_EXIT			= RegisterParticle("impacts/hit_textile_ext_01");
	static const int IMPACT_SAND_ENTER				= RegisterParticle("impacts/hit_sand_ent_01");
	static const int IMPACT_SAND_RICOCHET			= RegisterParticle("impacts/hit_sand_ric_01");
	static const int IMPACT_SAND_EXIT				= RegisterParticle("impacts/hit_sand_ext_01");
	static const int IMPACT_PLASTIC_ENTER			= RegisterParticle("impacts/hit_plastic_ent_01");
	static const int IMPACT_PLASTIC_RICOCHET		= RegisterParticle("impacts/hit_plastic_ric_01");
	static const int IMPACT_PLASTIC_EXIT			= RegisterParticle("impacts/hit_plastic_ext_01");
	
	// EXPLOSIONS
	static const int EXPLOSION_LANDMINE				= RegisterParticle("explosion_landmine_01");
	static const int EXPLOSION_TEST					= RegisterParticle("explosion_placeholder");
	
	// ENVIRO EFX
	static const int SMOKING_HELI_WRECK				= RegisterParticle("smoke_heli_wreck_01");
	static const int SMOKE_GENERIC_WRECK			= RegisterParticle("smoke_generic_wreck");
	static const int SMOKING_CAR_ENGINE				= RegisterParticle("menu_engine_fire");
	static const int EVAPORATION					= RegisterParticle("menu_evaporation");
	
	// VEHICLES
	static const int HATCHBACK_COOLANT_OVERHEATING	= RegisterParticle("Hatchback_coolant_overheating");
	static const int HATCHBACK_COOLANT_OVERHEATED	= RegisterParticle("Hatchback_coolant_overheated");
	static const int HATCHBACK_ENGINE_OVERHEATING	= RegisterParticle("Hatchback_engine_failing");
	static const int HATCHBACK_ENGINE_OVERHEATED	= RegisterParticle("Hatchback_engine_failure");
	static const int HATCHBACK_EXHAUST_SMOKE		= RegisterParticle("Hatchback_exhaust");
	
	// CORPSE DECAY
	static const int ENV_SWARMING_FLIES 			= RegisterParticle("env_fly_swarm_01");
	
	static int RegisterParticle (string file_name)
	{
		return RegisterParticle( GetPathToParticles() , file_name);
	}
	
	//! Registers a particle and returns its ID
	static int RegisterParticle (string root_path, string file_name)
	{
		if (!m_ParticlePaths)
			m_ParticlePaths = new map<int, string>;
	
		string path = root_path + file_name + ".ptc";
		bool file_exist = FileExist(path);
		
		if ( !file_exist )
			Error("ERROR! Particle file not found! <" + path + ">  Correct path to this particle in script file ParticleList.c! Then rebuild 'scripts' and 'graphics' PBOs.");
		
		m_ParticlePaths.Insert(++m_lastID, root_path + file_name);
		
		return m_lastID;
	}
	
	static int GetRegisteredParticlesCount()
	{
		return m_lastID;
	}
	
	//! Returns particle's ID based on its relative path (without .ptc suffix)
	static string GetParticlePath(int particle_id)
	{
		return m_ParticlePaths.Get(particle_id);
	}
	
	//! Returns particle's full path (with .ptc suffix) based on its ID
	static string GetParticleFullPath(int particle_id)
	{
		string path = m_ParticlePaths.Get(particle_id);
		return path + ".ptc";
	}
	
	//! Returns particle's ID based on the filename (without .ptc suffix)
	static int GetParticleID(string particle_file)
	{
		return m_ParticlePaths.GetKeyByValue(particle_file);
	}
	
	//! Returns base path to all particles
	static string GetPathToParticles()
	{
		return "graphics/particles/";
	}
	
	//! Preloads all particles
	static void PreloadParticles()
	{
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ) // client side
		{
			int count = m_ParticlePaths.Count();
			
			//Print("START PRELOAD OF PARTICLES");
			//Print(count);
			
			for (int i = 0; i < count; ++i)
			{
				string path = m_ParticlePaths.Get(i);
				path = path + ".ptc";
				//Print(path);
				vobject vobj;
				vobj = GetObject( path );
				//Print(vobj);
				ReleaseObject(vobj);
			}
			
			//Print("END PARTICLE PRELOAD");
		}
	}
}