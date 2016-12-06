/*********************************************************************
 * FileName:        lightSensor.c
 * Description:     The module is used to config light sensor input
 *                  pin and to check if there is light input detected.
 *
 *
 ********************************************************************/
#include "system.h"
#include "lightSensor.h"


#include "ioPorts.h"    // For Testing

#define LIGHT_DARK_HI_THRESH			610		//reading for 3v of 5v
#define LIGHT_DIM_HI_THRESH				800		//reading for 4v of 5v

typedef enum _lightTypeEnum
{
    LIGHT_IDLE = 0,
    LIGHT_DARK,
    LIGHT_DIM,
    LIGHT_BRIGHT,
	LIGHT_ERROR,
}lightTypeEnum;

 
lightTypeEnum curLightMode;
unsigned int lightReading;

void	lightSensorInit( void )
{
	// Configure Light Sensor Input Pin(PORTC RC2) as Analog In
	TRISCbits.TRISC2 = 1;
	ANSELCbits.ANSC2 = 1;

	lightReading = 0;
	curLightMode = LIGHT_IDLE;
}

void lightSensorUpdateReading( unsigned int curLightReading )
{
	if ( curLightReading > lightReading )
	{
		lightReading = curLightReading;
	}
}

void lightSensorDataProcessing( void )
{
//    EventType evt;

	lightTypeEnum tmpLightMode;

	if (( lightReading >= 0 ) &&  ( lightReading < LIGHT_DARK_HI_THRESH ))
	{
		tmpLightMode = LIGHT_DARK;
	}
	else if (( lightReading >= LIGHT_DARK_HI_THRESH ) &&  ( lightReading < LIGHT_DIM_HI_THRESH ))
	{
		tmpLightMode = LIGHT_DIM;
	}
	else if ( lightReading >= LIGHT_DIM_HI_THRESH )
	{
		tmpLightMode = LIGHT_BRIGHT;
	}
	else
	{
		tmpLightMode = LIGHT_ERROR; // ignor error reading
		return;
	}
	
	if (( curLightMode == LIGHT_DARK ) && ( tmpLightMode == LIGHT_BRIGHT ))
	{
		// Put light on event in the queue
		// send USB lightOnInputDetectedMsg message Here
mLED_S_On();
sendUSBMsg('L');

//        evt.eType = lightOnInputDetectedEvent;
 //       putEventIntoEventQueue(&evt);
		curLightMode = tmpLightMode;
	}
	else if (( curLightMode == LIGHT_BRIGHT ) && ( tmpLightMode == LIGHT_DARK ))
	{
		// send USB lightOffInputDetectedMsg message Here
mLED_S_Off();
sendUSBMsg('l');
		// Put light off event in the queue
 //       evt.eType = lightOffInputDetectedEvent;
 //       putEventIntoEventQueue(&evt);
		curLightMode = tmpLightMode;
	}
	else
	{
		curLightMode = tmpLightMode;
	}

	lightReading = 0;
}
