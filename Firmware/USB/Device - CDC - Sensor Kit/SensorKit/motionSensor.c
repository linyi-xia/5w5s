/*********************************************************************
 * FileName:        motionSensor.c
 * Description:     The module is used to config motion sensor input
 *                  pin and send an event when a motion detected. This 
 *                  module is initiated by cnModule and has to work with
 *                  cnModule.
 ********************************************************************/
#include "system.h"
#include "motionSensor.h"

#include "ioPorts.h"    // For Testing

BOOL motionState;   // 0 - no motions;   1 - motion detected 

void	motionSensorInit( void )
{
	motionState = FALSE;
}

void motionSensorMotionChanged( unsigned int changedMotionState )
{
//    EventType evt;

	// Put motion detected event in the queue
	if ( changedMotionState == MOTION_SENSOR_PIN_NORMAL )
	{
		//send USB motionInputEndedMsg Here
//		evt.eType = motionInputEndedEvent;
		motionState = FALSE;

mLED_1_Off();  
sendUSBMsg('m');

	}
	else
	{
		// send motionInputDetectedMsg Here
//		evt.eType = motionInputDetectedEvent;
		motionState = TRUE; 
 
mLED_1_On();  
sendUSBMsg('M');
	}

//    putEventIntoEventQueue(&evt);
}

BOOL getMotionStateFlag (void)
{
    return motionState;
}
