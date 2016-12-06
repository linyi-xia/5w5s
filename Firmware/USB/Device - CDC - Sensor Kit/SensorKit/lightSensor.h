
#ifndef __LIGHT_SENSOR_H__
#define __LIGHT_SENSOR_H__

//=====================================================================
// PUBLIC FUNCTION DELCLARATIONS
//=====================================================================
void lightSensorInit( void ); 
void lightSensorUpdateReading( unsigned int curLightReading );
void lightSensorDataProcessing( void );

#endif // __LIGHT_SENSOR_H__