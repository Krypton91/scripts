class ChernarusPlusData extends WorldData
{
	//----- weather vars
	const float RAIN_THRESHOLD = 0.6;
	const float STORM_THRESHOLD = 0.9;

	const int OVERCAST_MIN_TIME = 600;
	const int OVERCAST_MAX_TIME = 900;

	const int RAIN_TIME_MIN = 60;
	const int RAIN_TIME_MAX = 120;
	
	const int CLEAR_WEATHER = 1;
	const int CLOUDY_WEATHER = 2;
	const int BAD_WEATHER = 3;
	
	const int CLEAR_WEATHER_CHANCE = 30;
	const int BAD_WEATHER_CHANCE = 80;
		
	protected int m_clearWeatherChance = CLEAR_WEATHER_CHANCE;
	//protected int m_badCloudyChance = 80;
	protected int m_badWeatherChance = BAD_WEATHER_CHANCE;
	
	protected int m_sameWeatherCnt = 0;
	protected int m_stepValue = 5;
	protected int m_chance = 50;

	protected int m_choosenWeather = 1;
	protected int m_lastWeather = 0;

	override void Init()
	{
		super.Init();
		
		m_DayTemperature = 7;
		m_NightTemperature = -2;
	}
	
	override bool WeatherOnBeforeChange( EWeatherPhenomenon type, float actual, float change, float time )
	{

		float phmnValue 0;
		int phmnTime 5;
		int phmnLength 10;

		m_Weather.SetStorm( 1.0, STORM_THRESHOLD, 45 );

		m_Weather.SetRainThresholds( 0.0, 1.0, 60 );
		m_Weather.SetWindMaximumSpeed( 20 );
		m_Weather.SetWindFunctionParams( 0.1, 1.0, 20 );

		switch( type )
		{
			//-----------------------------------------------------------------------------------------------------------------------------
			case EWeatherPhenomenon.OVERCAST:

				//went something goes wrong choose some default random weather	
				phmnValue = Math.RandomFloatInclusive( 0.2, 0.7 );
				phmnTime = Math.RandomIntInclusive( OVERCAST_MIN_TIME, OVERCAST_MAX_TIME );
				phmnLength = Math.RandomIntInclusive( OVERCAST_MIN_TIME, OVERCAST_MAX_TIME );	
			
				//----
				//calculate next weather
				m_chance = Math.RandomIntInclusive( 0, 100 );
				
				//--
				if ( m_lastWeather == CLEAR_WEATHER )
				{
					m_clearWeatherChance -= ( m_stepValue * m_sameWeatherCnt);					//decrease the chance of the same weather
				}

				if ( m_lastWeather == CLOUDY_WEATHER )
				{
					m_clearWeatherChance += ( m_stepValue * m_sameWeatherCnt);					//increase the chance of the better weather
					//m_badWeatherChance += ( m_stepValue * m_sameWeatherCnt);
				}
			
				if ( m_lastWeather == BAD_WEATHER )
				{
					m_clearWeatherChance += m_stepValue;										//increase the chance of the better weather slightly
					m_badWeatherChance += (( m_stepValue * m_sameWeatherCnt ) + m_stepValue  );	//decrease the chance of the same weather rapidly
				}
			
				//----
				if ( m_chance < m_clearWeatherChance )
				{
					m_choosenWeather = CLEAR_WEATHER;
					if ( m_lastWeather == CLEAR_WEATHER )
						m_sameWeatherCnt ++;
				}
				else if ( m_chance > m_badWeatherChance )
				{
					m_choosenWeather = BAD_WEATHER;
					if ( m_lastWeather == BAD_WEATHER )
						m_sameWeatherCnt ++;
				}
				else
				{
					m_choosenWeather = CLOUDY_WEATHER;
					if ( m_lastWeather == CLOUDY_WEATHER )
						m_sameWeatherCnt ++;
				}

				if ( m_choosenWeather != m_lastWeather )
					m_sameWeatherCnt = 0;

				m_clearWeatherChance = CLEAR_WEATHER_CHANCE;
				m_badWeatherChance = BAD_WEATHER_CHANCE;

				//----
				//set choosen weather			
				if ( m_choosenWeather == CLEAR_WEATHER  )
				{
					m_lastWeather = CLEAR_WEATHER;
					
					phmnValue = Math.RandomFloatInclusive( 0.0, 0.3 );
					phmnTime = Math.RandomIntInclusive( OVERCAST_MIN_TIME, OVERCAST_MAX_TIME );
					phmnLength = Math.RandomIntInclusive( OVERCAST_MIN_TIME, OVERCAST_MAX_TIME );
				}

				if ( m_choosenWeather == CLOUDY_WEATHER )
				{
					m_lastWeather = CLOUDY_WEATHER;

					phmnValue = Math.RandomFloatInclusive( 0.3, 0.6 );
					phmnTime = Math.RandomIntInclusive( OVERCAST_MIN_TIME, OVERCAST_MAX_TIME );
					phmnLength = Math.RandomIntInclusive( OVERCAST_MIN_TIME, OVERCAST_MAX_TIME );
				}
			
				if ( m_choosenWeather == BAD_WEATHER )
				{
					m_lastWeather = BAD_WEATHER;

					phmnValue = Math.RandomFloatInclusive( 0.6, 1.0 );
					phmnTime = Math.RandomIntInclusive( OVERCAST_MIN_TIME, OVERCAST_MAX_TIME );
					phmnLength = Math.RandomIntInclusive( OVERCAST_MIN_TIME, OVERCAST_MAX_TIME );
				}

				m_Weather.GetOvercast().Set( phmnValue, phmnTime, phmnLength );

				Print( "Chernarus::Weather::Overcast:: " + "( " + g_Game.GetDayTime() + " ) " + " overcast: " + actual );
				Print( "Chernarus::Weather::Overcast::Rain:: " + "( " + g_Game.GetDayTime() + " ) " + m_Weather.GetRain().GetActual() );

				return true;

			//-----------------------------------------------------------------------------------------------------------------------------
			case EWeatherPhenomenon.RAIN:

				float actualOvercast = m_Weather.GetOvercast().GetActual();
				
				m_chance = Math.RandomIntInclusive( 0, 100 );	
				phmnValue = 0.2;
				phmnTime = 90;
				phmnLength = 30;
			
				if ( actualOvercast <= RAIN_THRESHOLD )
				{
					m_Weather.GetRain().Set( 0.0, RAIN_TIME_MIN, RAIN_TIME_MAX );
					Print( "Chernarus::Weather::Rain::ForceEnd:: " + "( " + g_Game.GetDayTime() + " ) " + actual + " -> " + "0" );
					return true;
				}
			
				if ( actualOvercast > STORM_THRESHOLD )
				{
					phmnValue = Math.RandomFloatInclusive( 0.8, 1.0 );
					phmnTime = Math.RandomInt( RAIN_TIME_MIN, RAIN_TIME_MAX );
					phmnLength = 0;

					m_Weather.GetRain().Set( phmnValue, phmnTime, phmnLength );
					Print( "Chernarus::Weather::Rain::ForceStorm:: " + "( " + g_Game.GetDayTime() + " ) " + actual + " -> " + phmnValue);
					return true;
				}
			
				//make a differnce in "normal rain"
				if ( actualOvercast < 0.75 )
				{
					if ( m_chance < 30 )
					{
						phmnValue = Math.RandomFloatInclusive( 0.1, 0.3 );
						phmnTime = Math.RandomInt( RAIN_TIME_MIN, RAIN_TIME_MAX );
						phmnLength = 0;
					}
					else if ( m_chance < 60 )
					{
						phmnValue = Math.RandomFloatInclusive( 0.2, 0.5 );
						phmnTime = Math.RandomInt( RAIN_TIME_MIN, RAIN_TIME_MAX );
						phmnLength = 0;
					}
					else if ( m_chance < 80 )
					{
						phmnValue = Math.RandomFloatInclusive( 0.0, 0.2 );
						phmnTime = Math.RandomInt( RAIN_TIME_MIN, RAIN_TIME_MAX );
						phmnLength = 0;
					}
					else //also have the chance to not have rain at all
					{
						phmnValue = 0;
						phmnTime = Math.RandomInt( RAIN_TIME_MIN, RAIN_TIME_MAX );
						phmnLength = 120;
					}
				}
				else
				{
					if ( m_chance < 25 )
					{
						phmnValue = Math.RandomFloatInclusive( 0.5, 0.7 );
						phmnTime = Math.RandomInt( RAIN_TIME_MIN, RAIN_TIME_MAX );
						phmnLength = 0;
					}
					else if ( m_chance < 50 )
					{
						phmnValue = Math.RandomFloatInclusive( 0.2, 0.4 );
						phmnTime = Math.RandomInt( RAIN_TIME_MIN, RAIN_TIME_MAX );
						phmnLength = 0;
					}
					else if ( m_chance < 75 )
					{
						phmnValue = Math.RandomFloatInclusive( 0.4, 0.6 );
						phmnTime = Math.RandomInt( RAIN_TIME_MIN, RAIN_TIME_MAX );
						phmnLength = 0;
					}
					else //also have the chance to not have rain at all
					{
						phmnValue = 0;
						phmnTime = Math.RandomInt( RAIN_TIME_MIN, RAIN_TIME_MAX );
						phmnLength = 120;
					}
				}
		
				m_Weather.GetRain().Set( phmnValue, phmnTime, phmnLength );

				Print( "Chernarus::Weather::Rain:: " + "( " + g_Game.GetDayTime() + " ) " + actual );
				return true;

			//-----------------------------------------------------------------------------------------------------------------------------
			case EWeatherPhenomenon.FOG:

				float fogMin = 0.0;
				float fogMax = 0.15;
				float fogTime = 1800.0;

				float fogyMorning = Math.RandomFloatInclusive( 0.0, 1.0 );

				if (  fogyMorning > 0.85 )
				{
					if ( (g_Game.GetDayTime() > 4 && g_Game.GetDayTime() < 7 ) )
					{
						fogMin = 0.10;
						fogMax = 0.35;
						fogTime = 300;
					}
				}

				if ( m_Weather.GetOvercast().GetActual() < 0.3 )
				{
					fogMin = 0.0;
					fogMax = 0.08;
					fogTime = 900.0;
				}

				m_Weather.GetFog().Set( Math.RandomFloatInclusive( fogMin, fogMax ), fogTime, 0);
				
				Print( "Chernarus::Weather::Fog:: " + "( " + g_Game.GetDayTime() + " ) " + actual );
				return true;
		}

		return false;
	}
};