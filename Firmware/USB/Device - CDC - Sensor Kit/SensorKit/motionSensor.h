
#ifndef __MOTION_SENSOR_H__
#define __MOTION_SENSOR_H__

/* configure the motion sensor input pin RD6/CN15(PIN83) as digital input C */	
#define MOTION_SENSOR_PIN_NORMAL    0    // motion sensor pin is normal LO


//=====================================================================
// PUBLIC FUNCTION DELCLARATIONS
//=====================================================================
void	motionSensorInit( void ); 
void motionSensorMotionChanged( unsigned int changedMotionState );
BOOL getMotionStateFlag (void);
#endif // __MOTION_SENSOR_H__