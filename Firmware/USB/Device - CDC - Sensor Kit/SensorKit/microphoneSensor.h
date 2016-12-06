
#ifndef __MICROPHONE_SENSOR_H__
#define __MICROPHONE_SENSOR_H__

//=====================================================================
// PUBLIC FUNCTION DELCLARATIONS
//=====================================================================
void microphoneSensorInit(void);
void microphoneSensorUpdateReading( unsigned int curMicReading );
void microphoneSensorDataProcessing( void );
BOOL mic_isInPartyMode( void );

#endif // __MICROPHONE_SENSOR_H__